#include <eosio.system/eosio.system.hpp>
#include <eosio/action.hpp>
#include <eosio.system/powerup.results.hpp>
#include <algorithm>
#include <cmath>

namespace eosiosystem {

void update_weight(time_point_sec now, powerup_state_resource& res, int64_t& delta_available);

/**
 *  @pre  now >= res.utilization_timestamp
 *  @post res.utilization <= new res.adjusted_utilization
 *  @post if res.utilization < old res.adjusted_utilization, then new res.adjusted_utilization <= old res.adjusted_utilization
 *  @post if res.utilization >= old res.adjusted_utilization, then new res.adjusted_utilization == res.utilization
 */
void update_utilization(time_point_sec now, powerup_state_resource& res);

void system_contract::adjust_resources(name payer, name account, symbol core_symbol, int64_t net_delta,
                                       int64_t cpu_delta, bool must_not_be_managed) {
   if (!net_delta && !cpu_delta)
      return;

   user_resources_table totals_tbl(get_self(), account.value);
   auto                 tot_itr = totals_tbl.find(account.value);
   if (tot_itr == totals_tbl.end()) {
      tot_itr = totals_tbl.emplace(payer, [&](auto& tot) {
         tot.owner      = account;
         tot.net_weight = asset{ net_delta, core_symbol };
         tot.cpu_weight = asset{ cpu_delta, core_symbol };
      });
   } else {
      totals_tbl.modify(tot_itr, same_payer, [&](auto& tot) {
         tot.net_weight.amount += net_delta;
         tot.cpu_weight.amount += cpu_delta;
      });
   }
   check(0 <= tot_itr->net_weight.amount, "insufficient staked total net bandwidth");
   check(0 <= tot_itr->cpu_weight.amount, "insufficient staked total cpu bandwidth");

   {
      bool ram_managed = false;
      bool net_managed = false;
      bool cpu_managed = false;

      auto voter_itr = _voters.find(account.value);
      if (voter_itr != _voters.end()) {
         ram_managed = has_field(voter_itr->flags1, voter_info::flags1_fields::ram_managed);
         net_managed = has_field(voter_itr->flags1, voter_info::flags1_fields::net_managed);
         cpu_managed = has_field(voter_itr->flags1, voter_info::flags1_fields::cpu_managed);
      }

      if (must_not_be_managed)
         eosio::check(!net_managed && !cpu_managed, "something is managed which shouldn't be");

      if (!(net_managed && cpu_managed)) {
         int64_t ram_bytes, net, cpu;
         get_resource_limits(account, ram_bytes, net, cpu);
         set_resource_limits(
               account, ram_managed ? ram_bytes : std::max(tot_itr->ram_bytes + ram_gift_bytes, ram_bytes),
               net_managed ? net : tot_itr->net_weight.amount, cpu_managed ? cpu : tot_itr->cpu_weight.amount);
      }
   }

   if (tot_itr->is_empty()) {
      totals_tbl.erase(tot_itr);
   }
} // system_contract::adjust_resources

void system_contract::process_powerup_queue(time_point_sec now, symbol core_symbol, powerup_state& state,
                                           powerup_order_table& orders, uint32_t max_items, int64_t& net_delta_available,
                                           int64_t& cpu_delta_available) {
   update_utilization(now, state.net);
   update_utilization(now, state.cpu);
   auto idx = orders.get_index<"byexpires"_n>();
   while (max_items--) {
      auto it = idx.begin();
      if (it == idx.end() || it->expires > now)
         break;
      net_delta_available += it->net_weight;
      cpu_delta_available += it->cpu_weight;
      adjust_resources(get_self(), it->owner, core_symbol, -it->net_weight, -it->cpu_weight);
      idx.erase(it);
   }
   state.net.utilization -= net_delta_available;
   state.cpu.utilization -= cpu_delta_available;
   update_weight(now, state.net, net_delta_available);
   update_weight(now, state.cpu, cpu_delta_available);
}

void update_weight(time_point_sec now, powerup_state_resource& res, int64_t& delta_available) {
   if (now >= res.target_timestamp) {
      res.weight_ratio = res.target_weight_ratio;
   } else {
      res.weight_ratio = res.initial_weight_ratio + //
                         int128_t(res.target_weight_ratio - res.initial_weight_ratio) *
                               (now.utc_seconds - res.initial_timestamp.utc_seconds) /
                               (res.target_timestamp.utc_seconds - res.initial_timestamp.utc_seconds);
   }
   int64_t new_weight    = res.assumed_stake_weight * int128_t(powerup_frac) / res.weight_ratio - res.assumed_stake_weight;
   delta_available += new_weight - res.weight;
   res.weight = new_weight;
}

void update_utilization(time_point_sec now, powerup_state_resource& res) {
   if (now <= res.utilization_timestamp) return;

   if (res.utilization >= res.adjusted_utilization) {
      res.adjusted_utilization = res.utilization;
   } else {
      int64_t diff  = res.adjusted_utilization - res.utilization;
      int64_t delta = diff * std::exp(-double(now.utc_seconds - res.utilization_timestamp.utc_seconds) / double(res.decay_secs));
      delta = std::clamp( delta, 0ll, diff);
      res.adjusted_utilization = res.utilization + delta;
   }
   res.utilization_timestamp = now;
}

void system_contract::cfgpowerup(powerup_config& args) {
   require_auth(get_self());
   time_point_sec         now         = eosio::current_time_point();
   auto                   core_symbol = get_core_symbol();
   powerup_state_singleton state_sing{ get_self(), 0 };
   auto                   state = state_sing.get_or_default();

   eosio::check(eosio::is_account(reserv_account), "eosio.reserv account must first be created");

   int64_t net_delta_available = 0;
   int64_t cpu_delta_available = 0;
   if (state_sing.exists()) {
      update_utilization(now, state.net);
      update_utilization(now, state.cpu);
      update_weight(now, state.net, net_delta_available);
      update_weight(now, state.cpu, cpu_delta_available);
   } else {
      state.net.utilization_timestamp = now;
      state.cpu.utilization_timestamp = now;
   }

   auto is_default_asset = []( const eosio::asset& a ) -> bool {
      return a.amount == 0 && a.symbol == symbol{};
   };

   auto update = [&](auto& state, auto& args) {
      if (!args.current_weight_ratio) {
         if (state.weight_ratio) {
            *args.current_weight_ratio = state.weight_ratio;
         } else {
            *args.current_weight_ratio = state.initial_weight_ratio;
         }
      }

      if (!args.target_weight_ratio) {
         *args.target_weight_ratio = state.target_weight_ratio;
      }

      if (!args.assumed_stake_weight) {
         eosio::check(state.assumed_stake_weight != 0, "assumed_stake_weight does not have a default value");
         *args.assumed_stake_weight = state.assumed_stake_weight;
      }

      if (*args.current_weight_ratio == *args.target_weight_ratio) {
         *args.target_timestamp = now;
      } else {
         if (!args.target_timestamp) {
            eosio::check(state.target_timestamp.utc_seconds != 0, "target_timestamp does not have a default value");
            *args.target_timestamp = state.target_timestamp;
         }
         eosio::check(*args.target_timestamp > now, "target_timestamp must be in the future");
      }

      if (!args.exponent) {
         *args.exponent = state.exponent;
      }

      if (!args.decay_secs) {
         *args.decay_secs = state.decay_secs;
      }

      if (!args.max_price) {
         eosio::check(!is_default_asset(state.max_price), "max_price does not have a default value");
         *args.max_price = state.max_price;
      }

      if (!args.min_price) {
         if (is_default_asset(state.min_price)) {
            *args.min_price = *args.max_price; // just to copy symbol of max_price
            args.min_price->amount = 0;        // min_price has a default of zero.
         } else {
            *args.min_price = state.min_price;
         }
      }

      eosio::check(*args.current_weight_ratio > 0, "current_weight_ratio is too small");
      eosio::check(*args.current_weight_ratio <= powerup_frac, "current_weight_ratio is too large");
      eosio::check(*args.target_weight_ratio > 0, "target_weight_ratio is too small");
      eosio::check(*args.target_weight_ratio <= *args.current_weight_ratio, "weight can't grow over time");
      eosio::check(*args.assumed_stake_weight >= 1,
                   "assumed_stake_weight must be at least 1; a much larger value is recommended");
      eosio::check(*args.assumed_stake_weight * int128_t(powerup_frac) / *args.target_weight_ratio <=
                         std::numeric_limits<int64_t>::max(),
                   "assumed_stake_weight/target_weight_ratio is too large");
      eosio::check(*args.exponent >= 1.0, "exponent must be >= 1");
      eosio::check(*args.decay_secs >= 1, "decay_secs must be >= 1");
      eosio::check(args.max_price->symbol == core_symbol, "max_price doesn't match core symbol");
      eosio::check(args.max_price->amount > 0, "max_price must be positive");
      eosio::check(args.min_price->symbol == core_symbol, "min_price doesn't match core symbol");
      eosio::check(args.min_price->amount >= 0, "min_price must be non-negative");
      eosio::check(args.min_price->amount <= args.max_price->amount, "min_price cannot exceed max_price");
      if (*args.exponent == 1.0) {
         eosio::check(args.min_price->amount == args.max_price->amount, "min_price and max_price must be the same if the exponent is 1");
      }

      state.assumed_stake_weight = *args.assumed_stake_weight;
      state.initial_weight_ratio = *args.current_weight_ratio;
      state.target_weight_ratio  = *args.target_weight_ratio;
      state.initial_timestamp    = now;
      state.target_timestamp     = *args.target_timestamp;
      state.exponent             = *args.exponent;
      state.decay_secs           = *args.decay_secs;
      state.min_price            = *args.min_price;
      state.max_price            = *args.max_price;
   };

   if (!args.powerup_days) {
      *args.powerup_days = state.powerup_days;
   }

   if (!args.min_powerup_fee) {
      eosio::check(!is_default_asset(state.min_powerup_fee), "min_powerup_fee does not have a default value");
      *args.min_powerup_fee = state.min_powerup_fee;
   }

   eosio::check(*args.powerup_days > 0, "powerup_days must be > 0");
   eosio::check(args.min_powerup_fee->symbol == core_symbol, "min_powerup_fee doesn't match core symbol");
   eosio::check(args.min_powerup_fee->amount > 0, "min_powerup_fee must be positive");

   state.powerup_days    = *args.powerup_days;
   state.min_powerup_fee = *args.min_powerup_fee;

   update(state.net, args.net);
   update(state.cpu, args.cpu);

   update_weight(now, state.net, net_delta_available);
   update_weight(now, state.cpu, cpu_delta_available);
   eosio::check(state.net.weight >= state.net.utilization, "weight can't shrink below utilization");
   eosio::check(state.cpu.weight >= state.cpu.utilization, "weight can't shrink below utilization");
   state.net.adjusted_utilization = std::min(state.net.adjusted_utilization, state.net.weight);
   state.cpu.adjusted_utilization = std::min(state.cpu.adjusted_utilization, state.cpu.weight);

   adjust_resources(get_self(), reserv_account, core_symbol, net_delta_available, cpu_delta_available, true);
   state_sing.set(state, get_self());
} // system_contract::configpower

/**
 *  @pre 0 <= state.min_price.amount <= state.max_price.amount
 *  @pre 0 < state.max_price.amount
 *  @pre 1.0 <= state.exponent
 *  @pre 0 <= state.utilization <= state.adjusted_utilization <= state.weight
 *  @pre 0 <= utilization_increase <= (state.weight - state.utilization)
 */
int64_t calc_powerup_fee(const powerup_state_resource& state, int64_t utilization_increase) {
   if( utilization_increase <= 0 ) return 0;

   // Let p(u) = price as a function of the utilization fraction u which is defined for u in [0.0, 1.0].
   // Let f(u) = integral of the price function p(x) from x = 0.0 to x = u, again defined for u in [0.0, 1.0].

   // In particular we choose f(u) = min_price * u + ((max_price - min_price) / exponent) * (u ^ exponent).
   // And so p(u) = min_price + (max_price - min_price) * (u ^ (exponent - 1.0)).

   // Returns f(double(end_utilization)/state.weight) - f(double(start_utilization)/state.weight) which is equivalent to
   // the integral of p(x) from x = double(start_utilization)/state.weight to x = double(end_utilization)/state.weight.
   // @pre 0 <= start_utilization <= end_utilization <= state.weight
   auto price_integral_delta = [&state](int64_t start_utilization, int64_t end_utilization) -> double {
      double coefficient = (state.max_price.amount - state.min_price.amount) / state.exponent;
      double start_u     = double(start_utilization) / state.weight;
      double end_u       = double(end_utilization) / state.weight;
      return state.min_price.amount * end_u - state.min_price.amount * start_u +
               coefficient * std::pow(end_u, state.exponent) - coefficient * std::pow(start_u, state.exponent);
   };

   // Returns p(double(utilization)/state.weight).
   // @pre 0 <= utilization <= state.weight
   auto price_function = [&state](int64_t utilization) -> double {
      double price = state.min_price.amount;
      // state.exponent >= 1.0, therefore the exponent passed into std::pow is >= 0.0.
      // Since the exponent passed into std::pow could be 0.0 and simultaneously so could double(utilization)/state.weight,
      // the safest thing to do is handle that as a special case explicitly rather than relying on std::pow to return 1.0
      // instead of triggering a domain error.
      double new_exponent = state.exponent - 1.0;
      if (new_exponent <= 0.0) {
         return state.max_price.amount;
      } else {
         price += (state.max_price.amount - state.min_price.amount) * std::pow(double(utilization) / state.weight, new_exponent);
      }

      return price;
   };

   double  fee = 0.0;
   int64_t start_utilization = state.utilization;
   int64_t end_utilization   = start_utilization + utilization_increase;

   if (start_utilization < state.adjusted_utilization) {
      fee += price_function(state.adjusted_utilization) *
               std::min(utilization_increase, state.adjusted_utilization - start_utilization) / state.weight;
      start_utilization = state.adjusted_utilization;
   }

   if (start_utilization < end_utilization) {
      fee += price_integral_delta(start_utilization, end_utilization);
   }

   return std::ceil(fee);
}

void system_contract::powerupexec(const name& user, uint16_t max) {
   require_auth(user);
   powerup_state_singleton state_sing{ get_self(), 0 };
   powerup_order_table     orders{ get_self(), 0 };
   eosio::check(state_sing.exists(), "powerup hasn't been initialized");
   auto           state       = state_sing.get();
   time_point_sec now         = eosio::current_time_point();
   auto           core_symbol = get_core_symbol();

   int64_t net_delta_available = 0;
   int64_t cpu_delta_available = 0;
   process_powerup_queue(now, core_symbol, state, orders, max, net_delta_available, cpu_delta_available);

   adjust_resources(get_self(), reserv_account, core_symbol, net_delta_available, cpu_delta_available, true);
   state_sing.set(state, get_self());
}

void system_contract::powerup(const name& payer, const name& receiver, uint32_t days, int64_t net_frac, int64_t cpu_frac,
                             const asset& max_payment) {
   require_auth(payer);
   powerup_state_singleton state_sing{ get_self(), 0 };
   powerup_order_table     orders{ get_self(), 0 };
   eosio::check(state_sing.exists(), "powerup hasn't been initialized");
   auto           state       = state_sing.get();
   time_point_sec now         = eosio::current_time_point();
   auto           core_symbol = get_core_symbol();
   eosio::check(max_payment.symbol == core_symbol, "max_payment doesn't match core symbol");
   eosio::check(days == state.powerup_days, "days doesn't match configuration");
   eosio::check(net_frac >= 0, "net_frac can't be negative");
   eosio::check(cpu_frac >= 0, "cpu_frac can't be negative");
   eosio::check(net_frac <= powerup_frac, "net can't be more than 100%");
   eosio::check(cpu_frac <= powerup_frac, "cpu can't be more than 100%");

   int64_t net_delta_available = 0;
   int64_t cpu_delta_available = 0;
   process_powerup_queue(now, core_symbol, state, orders, 2, net_delta_available, cpu_delta_available);

   eosio::asset fee{ 0, core_symbol };
   auto         process = [&](int64_t frac, int64_t& amount, powerup_state_resource& state) {
      if (!frac)
         return;
      amount = int128_t(frac) * state.weight / powerup_frac;
      eosio::check(state.weight, "market doesn't have resources available");
      eosio::check(state.utilization + amount <= state.weight, "market doesn't have enough resources available");
      int64_t f = calc_powerup_fee(state, amount);
      eosio::check(f > 0, "calculated fee is below minimum; try powering up with more resources");
      fee.amount += f;
      state.utilization += amount;
   };

   int64_t net_amount = 0;
   int64_t cpu_amount = 0;
   process(net_frac, net_amount, state.net);
   process(cpu_frac, cpu_amount, state.cpu);
   if (fee > max_payment) {
      std::string error_msg = "max_payment is less than calculated fee: ";
      error_msg += fee.to_string();
      eosio::check(false, error_msg);
   }
   eosio::check(fee >= state.min_powerup_fee, "calculated fee is below minimum; try powering up with more resources");

   orders.emplace(payer, [&](auto& order) {
      order.id         = orders.available_primary_key();
      order.owner      = receiver;
      order.net_weight = net_amount;
      order.cpu_weight = cpu_amount;
      order.expires    = now + eosio::days(days);
   });
   net_delta_available -= net_amount;
   cpu_delta_available -= cpu_amount;

   adjust_resources(payer, receiver, core_symbol, net_amount, cpu_amount, true);
   adjust_resources(get_self(), reserv_account, core_symbol, net_delta_available, cpu_delta_available, true);
   channel_to_rex(payer, fee, true);
   state_sing.set(state, get_self());

   // inline noop action
   powup_results::powupresult_action powupresult_act{ reserv_account, std::vector<eosio::permission_level>{ } };
   powupresult_act.send( fee, net_amount, cpu_amount );
}

} // namespace eosiosystem
