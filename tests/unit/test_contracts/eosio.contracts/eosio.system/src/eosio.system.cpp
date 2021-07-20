#include <eosio.system/eosio.system.hpp>
#include <eosio.token/eosio.token.hpp>

#include <eosio/crypto.hpp>
#include <eosio/dispatcher.hpp>

#include <cmath>

namespace eosiosystem {

   using eosio::current_time_point;
   using eosio::token;

   double get_continuous_rate(int64_t annual_rate) {
      return std::log1p(double(annual_rate)/double(100*inflation_precision));
   }

   system_contract::system_contract( name s, name code, datastream<const char*> ds )
   :native(s,code,ds),
    _voters(get_self(), get_self().value),
    _producers(get_self(), get_self().value),
    _producers2(get_self(), get_self().value),
    _global(get_self(), get_self().value),
    _global2(get_self(), get_self().value),
    _global3(get_self(), get_self().value),
    _global4(get_self(), get_self().value),
    _rammarket(get_self(), get_self().value),
    _rexpool(get_self(), get_self().value),
    _rexretpool(get_self(), get_self().value),
    _rexretbuckets(get_self(), get_self().value),
    _rexfunds(get_self(), get_self().value),
    _rexbalance(get_self(), get_self().value),
    _rexorders(get_self(), get_self().value)
   {
      _gstate  = _global.exists() ? _global.get() : get_default_parameters();
      _gstate2 = _global2.exists() ? _global2.get() : eosio_global_state2{};
      _gstate3 = _global3.exists() ? _global3.get() : eosio_global_state3{};
      _gstate4 = _global4.exists() ? _global4.get() : get_default_inflation_parameters();
   }

   eosio_global_state system_contract::get_default_parameters() {
      eosio_global_state dp;
      get_blockchain_parameters(dp);
      return dp;
   }

   eosio_global_state4 system_contract::get_default_inflation_parameters() {
      eosio_global_state4 gs4;
      gs4.continuous_rate      = get_continuous_rate(default_annual_rate);
      gs4.inflation_pay_factor = default_inflation_pay_factor;
      gs4.votepay_factor       = default_votepay_factor;
      return gs4;
   }

   symbol system_contract::core_symbol()const {
      const static auto sym = get_core_symbol( _rammarket );
      return sym;
   }

   system_contract::~system_contract() {
      _global.set( _gstate, get_self() );
      _global2.set( _gstate2, get_self() );
      _global3.set( _gstate3, get_self() );
      _global4.set( _gstate4, get_self() );
   }

   void system_contract::setram( uint64_t max_ram_size ) {
      require_auth( get_self() );

      check( _gstate.max_ram_size < max_ram_size, "ram may only be increased" ); /// decreasing ram might result market maker issues
      check( max_ram_size < 1024ll*1024*1024*1024*1024, "ram size is unrealistic" );
      check( max_ram_size > _gstate.total_ram_bytes_reserved, "attempt to set max below reserved" );

      auto delta = int64_t(max_ram_size) - int64_t(_gstate.max_ram_size);
      auto itr = _rammarket.find(ramcore_symbol.raw());

      /**
       *  Increase the amount of ram for sale based upon the change in max ram size.
       */
      _rammarket.modify( itr, same_payer, [&]( auto& m ) {
         m.base.balance.amount += delta;
      });

      _gstate.max_ram_size = max_ram_size;
   }

   void system_contract::update_ram_supply() {
      auto cbt = eosio::current_block_time();

      if( cbt <= _gstate2.last_ram_increase ) return;

      auto itr = _rammarket.find(ramcore_symbol.raw());
      auto new_ram = (cbt.slot - _gstate2.last_ram_increase.slot)*_gstate2.new_ram_per_block;
      _gstate.max_ram_size += new_ram;

      /**
       *  Increase the amount of ram for sale based upon the change in max ram size.
       */
      _rammarket.modify( itr, same_payer, [&]( auto& m ) {
         m.base.balance.amount += new_ram;
      });
      _gstate2.last_ram_increase = cbt;
   }

   void system_contract::setramrate( uint16_t bytes_per_block ) {
      require_auth( get_self() );

      update_ram_supply();
      _gstate2.new_ram_per_block = bytes_per_block;
   }

   void system_contract::setparams( const eosio::blockchain_parameters& params ) {
      require_auth( get_self() );
      (eosio::blockchain_parameters&)(_gstate) = params;
      check( 3 <= _gstate.max_authority_depth, "max_authority_depth should be at least 3" );
      set_blockchain_parameters( params );
   }

   void system_contract::setpriv( const name& account, uint8_t ispriv ) {
      require_auth( get_self() );
      set_privileged( account, ispriv );
   }

   void system_contract::setalimits( const name& account, int64_t ram, int64_t net, int64_t cpu ) {
      require_auth( get_self() );

      user_resources_table userres( get_self(), account.value );
      auto ritr = userres.find( account.value );
      check( ritr == userres.end(), "only supports unlimited accounts" );

      auto vitr = _voters.find( account.value );
      if( vitr != _voters.end() ) {
         bool ram_managed = has_field( vitr->flags1, voter_info::flags1_fields::ram_managed );
         bool net_managed = has_field( vitr->flags1, voter_info::flags1_fields::net_managed );
         bool cpu_managed = has_field( vitr->flags1, voter_info::flags1_fields::cpu_managed );
         check( !(ram_managed || net_managed || cpu_managed), "cannot use setalimits on an account with managed resources" );
      }

      set_resource_limits( account, ram, net, cpu );
   }

   void system_contract::setacctram( const name& account, const std::optional<int64_t>& ram_bytes ) {
      require_auth( get_self() );

      int64_t current_ram, current_net, current_cpu;
      get_resource_limits( account, current_ram, current_net, current_cpu );

      int64_t ram = 0;

      if( !ram_bytes ) {
         auto vitr = _voters.find( account.value );
         check( vitr != _voters.end() && has_field( vitr->flags1, voter_info::flags1_fields::ram_managed ),
                "RAM of account is already unmanaged" );

         user_resources_table userres( get_self(), account.value );
         auto ritr = userres.find( account.value );

         ram = ram_gift_bytes;
         if( ritr != userres.end() ) {
            ram += ritr->ram_bytes;
         }

         _voters.modify( vitr, same_payer, [&]( auto& v ) {
            v.flags1 = set_field( v.flags1, voter_info::flags1_fields::ram_managed, false );
         });
      } else {
         check( *ram_bytes >= 0, "not allowed to set RAM limit to unlimited" );

         auto vitr = _voters.find( account.value );
         if ( vitr != _voters.end() ) {
            _voters.modify( vitr, same_payer, [&]( auto& v ) {
               v.flags1 = set_field( v.flags1, voter_info::flags1_fields::ram_managed, true );
            });
         } else {
            _voters.emplace( account, [&]( auto& v ) {
               v.owner  = account;
               v.flags1 = set_field( v.flags1, voter_info::flags1_fields::ram_managed, true );
            });
         }

         ram = *ram_bytes;
      }

      set_resource_limits( account, ram, current_net, current_cpu );
   }

   void system_contract::setacctnet( const name& account, const std::optional<int64_t>& net_weight ) {
      require_auth( get_self() );

      int64_t current_ram, current_net, current_cpu;
      get_resource_limits( account, current_ram, current_net, current_cpu );

      int64_t net = 0;

      if( !net_weight ) {
         auto vitr = _voters.find( account.value );
         check( vitr != _voters.end() && has_field( vitr->flags1, voter_info::flags1_fields::net_managed ),
                "Network bandwidth of account is already unmanaged" );

         user_resources_table userres( get_self(), account.value );
         auto ritr = userres.find( account.value );

         if( ritr != userres.end() ) {
            net = ritr->net_weight.amount;
         }

         _voters.modify( vitr, same_payer, [&]( auto& v ) {
            v.flags1 = set_field( v.flags1, voter_info::flags1_fields::net_managed, false );
         });
      } else {
         check( *net_weight >= -1, "invalid value for net_weight" );

         auto vitr = _voters.find( account.value );
         if ( vitr != _voters.end() ) {
            _voters.modify( vitr, same_payer, [&]( auto& v ) {
               v.flags1 = set_field( v.flags1, voter_info::flags1_fields::net_managed, true );
            });
         } else {
            _voters.emplace( account, [&]( auto& v ) {
               v.owner  = account;
               v.flags1 = set_field( v.flags1, voter_info::flags1_fields::net_managed, true );
            });
         }

         net = *net_weight;
      }

      set_resource_limits( account, current_ram, net, current_cpu );
   }

   void system_contract::setacctcpu( const name& account, const std::optional<int64_t>& cpu_weight ) {
      require_auth( get_self() );

      int64_t current_ram, current_net, current_cpu;
      get_resource_limits( account, current_ram, current_net, current_cpu );

      int64_t cpu = 0;

      if( !cpu_weight ) {
         auto vitr = _voters.find( account.value );
         check( vitr != _voters.end() && has_field( vitr->flags1, voter_info::flags1_fields::cpu_managed ),
                "CPU bandwidth of account is already unmanaged" );

         user_resources_table userres( get_self(), account.value );
         auto ritr = userres.find( account.value );

         if( ritr != userres.end() ) {
            cpu = ritr->cpu_weight.amount;
         }

         _voters.modify( vitr, same_payer, [&]( auto& v ) {
            v.flags1 = set_field( v.flags1, voter_info::flags1_fields::cpu_managed, false );
         });
      } else {
         check( *cpu_weight >= -1, "invalid value for cpu_weight" );

         auto vitr = _voters.find( account.value );
         if ( vitr != _voters.end() ) {
            _voters.modify( vitr, same_payer, [&]( auto& v ) {
               v.flags1 = set_field( v.flags1, voter_info::flags1_fields::cpu_managed, true );
            });
         } else {
            _voters.emplace( account, [&]( auto& v ) {
               v.owner  = account;
               v.flags1 = set_field( v.flags1, voter_info::flags1_fields::cpu_managed, true );
            });
         }

         cpu = *cpu_weight;
      }

      set_resource_limits( account, current_ram, current_net, cpu );
   }

   void system_contract::activate( const eosio::checksum256& feature_digest ) {
      require_auth( get_self() );
      preactivate_feature( feature_digest );
   }

   void system_contract::rmvproducer( const name& producer ) {
      require_auth( get_self() );
      auto prod = _producers.find( producer.value );
      check( prod != _producers.end(), "producer not found" );
      _producers.modify( prod, same_payer, [&](auto& p) {
            p.deactivate();
         });
   }

   void system_contract::updtrevision( uint8_t revision ) {
      require_auth( get_self() );
      check( _gstate2.revision < 255, "can not increment revision" ); // prevent wrap around
      check( revision == _gstate2.revision + 1, "can only increment revision by one" );
      check( revision <= 1, // set upper bound to greatest revision supported in the code
             "specified revision is not yet supported by the code" );
      _gstate2.revision = revision;
   }

   void system_contract::setinflation( int64_t annual_rate, int64_t inflation_pay_factor, int64_t votepay_factor ) {
      require_auth(get_self());
      check(annual_rate >= 0, "annual_rate can't be negative");
      if ( inflation_pay_factor < pay_factor_precision ) {
         check( false, "inflation_pay_factor must not be less than " + std::to_string(pay_factor_precision) );
      }
      if ( votepay_factor < pay_factor_precision ) {
         check( false, "votepay_factor must not be less than " + std::to_string(pay_factor_precision) );
      }
      _gstate4.continuous_rate      = get_continuous_rate(annual_rate);
      _gstate4.inflation_pay_factor = inflation_pay_factor;
      _gstate4.votepay_factor       = votepay_factor;
      _global4.set( _gstate4, get_self() );
   }

   /**
    *  Called after a new account is created. This code enforces resource-limits rules
    *  for new accounts as well as new account naming conventions.
    *
    *  Account names containing '.' symbols must have a suffix equal to the name of the creator.
    *  This allows users who buy a premium name (shorter than 12 characters with no dots) to be the only ones
    *  who can create accounts with the creator's name as a suffix.
    *
    */
   void native::newaccount( const name&       creator,
                            const name&       newact,
                            ignore<authority> owner,
                            ignore<authority> active ) {

      if( creator != get_self() ) {
         uint64_t tmp = newact.value >> 4;
         bool has_dot = false;

         for( uint32_t i = 0; i < 12; ++i ) {
           has_dot |= !(tmp & 0x1f);
           tmp >>= 5;
         }
         if( has_dot ) { // or is less than 12 characters
            auto suffix = newact.suffix();
            if( suffix == newact ) {
               name_bid_table bids(get_self(), get_self().value);
               auto current = bids.find( newact.value );
               check( current != bids.end(), "no active bid for name" );
               check( current->high_bidder == creator, "only highest bidder can claim" );
               check( current->high_bid < 0, "auction for name is not closed yet" );
               bids.erase( current );
            } else {
               check( creator == suffix, "only suffix may create this account" );
            }
         }
      }

      user_resources_table  userres( get_self(), newact.value );

      userres.emplace( newact, [&]( auto& res ) {
        res.owner = newact;
        res.net_weight = asset( 0, system_contract::get_core_symbol() );
        res.cpu_weight = asset( 0, system_contract::get_core_symbol() );
      });

      set_resource_limits( newact, 0, 0, 0 );
   }

   void native::setabi( const name& acnt, const std::vector<char>& abi ) {
      eosio::multi_index< "abihash"_n, abi_hash >  table(get_self(), get_self().value);
      auto itr = table.find( acnt.value );
      if( itr == table.end() ) {
         table.emplace( acnt, [&]( auto& row ) {
            row.owner = acnt;
            row.hash = eosio::sha256(const_cast<char*>(abi.data()), abi.size());
         });
      } else {
         table.modify( itr, same_payer, [&]( auto& row ) {
            row.hash = eosio::sha256(const_cast<char*>(abi.data()), abi.size());
         });
      }
   }

   void system_contract::init( unsigned_int version, const symbol& core ) {
      require_auth( get_self() );
      check( version.value == 0, "unsupported version for init action" );

      auto itr = _rammarket.find(ramcore_symbol.raw());
      check( itr == _rammarket.end(), "system contract has already been initialized" );

      auto system_token_supply   = eosio::token::get_supply(token_account, core.code() );
      check( system_token_supply.symbol == core, "specified core symbol does not exist (precision mismatch)" );

      check( system_token_supply.amount > 0, "system token supply must be greater than 0" );
      _rammarket.emplace( get_self(), [&]( auto& m ) {
         m.supply.amount = 100000000000000ll;
         m.supply.symbol = ramcore_symbol;
         m.base.balance.amount = int64_t(_gstate.free_ram());
         m.base.balance.symbol = ram_symbol;
         m.quote.balance.amount = system_token_supply.amount / 1000;
         m.quote.balance.symbol = core;
      });

      token::open_action open_act{ token_account, { {get_self(), active_permission} } };
      open_act.send( rex_account, core, get_self() );
   }

} /// eosio.system
