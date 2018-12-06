#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>
#include <string>

using eosio::name;
using eosio::checksum256;

class [[eosio::contract]] message : public eosio::contract {
public:
   using contract::contract;

   [[eosio::action]]
   void sendmsg(name from, name to, uint64_t msg_id, const std::string& msg_str) {
      eosio::print("sendmsg from ", from, "\n");

      require_auth( from );

      require_recipient( to );

      checksum256 cs = eosio::sha256( msg_str.c_str(), msg_str.size() );

      eosio::print("sha ", cs, "\n");

      mess_index m( _self, _self.value );

      auto itr = m.find( msg_id );
      if (itr != m.end()) {
         std::string err = "Message id already exists: " + std::to_string(msg_id);
         eosio_assert(false, err.c_str());
      }

      m.emplace( from, [&](mess& msg) {
         msg.msg_id = msg_id;
         msg.from = from;
         msg.to = to;
         msg.msg_sha = cs;
      } );

   }

   [[eosio::action]]
   void sendsha(name from, name to, uint64_t msg_id, const checksum256& msg_sha) {
      eosio::print("sendsha from ", from, "\n");

      require_auth( from );

      require_recipient( to );

      mess_index m( _self, _self.value );

      auto itr = m.find( msg_id );
      if (itr != m.end()) {
         std::string err = "Message id already exists: " + std::to_string(msg_id);
         eosio_assert(false, err.c_str());
      }

      m.emplace( from /*payer*/, [&](mess& msg) {
         msg.msg_id = msg_id;
         msg.from = from;
         msg.to = to;
         msg.msg_sha = msg_sha;
      } );

      eosio::action a( eosio::permission_level{_self, "active"_n}, to, "msgnotify"_n,
            std::make_tuple( from, msg_id ) );
      a.send();

      message::msgnotify_action a2(to, {_self, "active"_n});
      a2.send(from, msg_id);
   }

   [[eosio::action]]
   void removemsg( uint64_t msg_id ) {
      require_auth( _self );

      mess_index m( _self, _self.value );

      auto itr = m.find( msg_id );
      if (itr == m.end()) {
         std::string err = "Message id does not exists: " + std::to_string(msg_id);
         eosio_assert(false, err.c_str());
      }

      m.erase( itr );
   }

   [[eosio::action]]
   void removesha( const checksum256& msg_sha ) {
      require_auth( _self );

      mess_index m( _self, _self.value );

      auto index = m.get_index<"msgsha"_n>();

      auto itr = index.find( msg_sha );
      if (itr == index.end()) {
         eosio::print("sha ", msg_sha, "\n");
         eosio_assert(false, "Message sha does not exists");
      }

      index.erase( itr );
   }

   [[eosio::action]]
   void msgnotify( name from, uint64_t msg_id ) {
      blacklist_index b( _self, _self.value );

      auto itr = b.find( from.value );
      if( itr != b.end() ) {
         eosio_assert(false, "Account in blacklist");
      }
   }

   using msgnotify_action = eosio::action_wrapper<"msgnotify"_n, &message::msgnotify>;

   [[eosio::action]]
   void addblacklist( name n ) {
      require_auth( _self );

      blacklist_index b(_self, _self.value);

      auto itr = b.find( n.value );
      if (itr == b.end()) {
         b.emplace( _self /*payer*/, [&](auto& b){ b.account = n;});
      }
   }

   [[eosio::action]]
   void addgroup( name group_name, const std::vector<name>& accounts ) {
      require_auth( _self );

      group_index g( _self, _self.value );

      auto itr = g.find( group_name.value );
      if (itr != g.end()) {
         g.modify(itr, eosio::same_payer, [&](auto& group) {
            group.accounts = accounts;
         });
      } else {
         g.emplace(_self/*payer*/, [&](auto& group) {
            group.group_name = group_name;
            group.accounts = accounts;
         });
      }
   }

   [[eosio::action]]
   void removegroup(name group_name) {
      require_auth( _self );

      group_index g( _self, _self.value );

      auto itr = g.find( group_name.value );
      if (itr == g.end()) {
         eosio_assert(false, "group does not exist");
      }

      g.erase( itr );
   }

   [[eosio::action]]
   void sendgroup(name from, name group_name, uint64_t msg_id, const checksum256& cs) {
      eosio::print("sendgroup from ", from, "\n");

      require_auth( from );

      group_index g( _self, _self.value );

      auto itr = g.find( group_name.value );
      eosio_assert(itr != g.end(), "group does not exist");

      for (auto& a : itr->accounts) {
         message::msgnotify_action a2(a, {_self, "active"_n});
         a2.send(from, msg_id);
      }

      mess_index m( _self, _self.value );

      {
         auto itr = m.find( msg_id );
         if( itr != m.end() ) {
            std::string err = "Message id already exists: " + std::to_string( msg_id );
            eosio_assert( false, err.c_str() );
         }

         m.emplace( from /*payer*/, [&]( mess& msg ) {
            msg.msg_id = msg_id;
            msg.from = from;
            msg.to = group_name;
            msg.msg_sha = cs;
         } );
      }
   }

   [[eosio::action]]
   void spam(name from, name to, uint64_t msg_id, const std::string& msg_str, uint32_t delay_sec) {
      require_auth( from );

      if (msg_id > 10)
         return;

      {
         eosio::transaction out;
         out.actions.emplace_back( eosio::permission_level{_self, "active"_n}, to, "sendmsg"_n,
                                   std::make_tuple( from, to, ++msg_id, msg_str ) );
         out.delay_sec = delay_sec;
         out.send( msg_id, _self );
      }

      {
         eosio::transaction out;
         out.actions.emplace_back( eosio::permission_level{_self, "active"_n}, _self, "spam"_n,
                                   std::make_tuple( from, to, ++msg_id, msg_str, delay_sec ) );
         out.delay_sec = delay_sec;
         out.send( msg_id, _self );
      }
   }

private:

   struct [[eosio::table]] mess {
      uint64_t    msg_id;
      name        from;
      name        to;
      checksum256 msg_sha;

      uint64_t primary_key() const { return msg_id; }
      const checksum256& by_msg_sha() const { return msg_sha; }
      uint64_t by_from() const { return from.value; }
   };

   typedef eosio::multi_index<"mess"_n, mess,
       eosio::indexed_by<"msgsha"_n, eosio::const_mem_fun<mess, const checksum256&, &mess::by_msg_sha> >,
       eosio::indexed_by<"from"_n, eosio::const_mem_fun<mess, uint64_t, &mess::by_from> >
   > mess_index;


   struct [[eosio::table]] blacklist {
      name account;

      uint64_t primary_key() const { return account.value; }
   };

   typedef eosio::multi_index<"blacklist"_n, blacklist> blacklist_index;

   struct [[eosio::table]] group {
      name group_name;
      std::vector<name> accounts;

      uint64_t primary_key() const { return group_name.value; }
   };

   typedef eosio::multi_index<"group"_n, group> group_index;


};

EOSIO_DISPATCH( message, (sendmsg)(sendsha)(removemsg)(removesha)(msgnotify)
                         (addblacklist)
                         (addgroup)(removegroup)(sendgroup)
                         (spam))
