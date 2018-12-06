#include <eosiolib/eosio.hpp>
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

   }

   [[eosio::action]]
   void removemsg() {

   }

private:

   struct [[eosio::table]] mess {
      uint64_t    msg_id;
      name        from;
      name        to;
      checksum256 msg_sha;

      uint64_t primary_key() const { return msg_id; }
      const checksum256& by_msg_sha() const { return msg_sha; }
   };

   typedef eosio::multi_index<"mess"_n, mess,
       eosio::indexed_by<"msgsha"_n, eosio::const_mem_fun<mess, const checksum256&, &mess::by_msg_sha> >
   > mess_index;

};

EOSIO_DISPATCH( message, (sendmsg)(sendsha)(removemsg) )
