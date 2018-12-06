#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.hpp>
#include <string>

using eosio::name;
using eosio::checksum256;
using eosio::indexed_by;
using eosio::const_mem_fun;
using std::string;


class [[eosio::contract]] messenger : public eosio::contract {
public:
   using contract::contract;

   [[eosio::action]]
   void sendmsg( name from, name to, uint64_t msg_id, const string& msg_str ) {
      eosio::print("sendmsg from ", from, "\n");
      // if not authorized then this action is aborted and transaction is rolled back
      // any modifications by other actions in same transaction are undone
      require_auth( from ); // make sure authorized by account

       // Add the specified account to set of accounts to be notified
      require_recipient( to );

      // calculate sha256 from msg_str
      checksum256 cs = eosio::sha256( msg_str.c_str(), msg_str.size() );

      // print for debugging
      eosio::print("sha: ", cs, "\n");

      // message_index is typedef of our multi_index over table address
      // message table is auto "created" if needed
      message_index messages( _self, _self.value ); // code, scope

      // verify does not already exist
      // multi_index find on primary index which in our case is account
      auto itr = messages.find( msg_id );
      if( itr != messages.end() ) {
         std::string err = "Message id already exists: " + std::to_string(msg_id);
         eosio_assert( false, err.c_str() );
      }

      // add to table, first argument is account to bill for storage
      // each entry will be billed to the associated account
      // we could have instead chosen to bill _self for all the storage
      messages.emplace( from /*payer*/, [&]( auto& msg ) {
         msg.msg_id = msg_id;
         msg.from = from;
         msg.to = to;
         msg.msg_sha = cs;
      } );

      // send msgnotify to 'to' account
      eosio::action( eosio::permission_level{_self, "active"_n},
                     to, "msgnotify"_n, std::make_tuple( from, msg_id, cs) ).send();

      // alternative if action_wrapper is used
      // constructor takes two arguments (the code the contract is deployed on and the set of permissions)
      messenger::msgnotify_action(to, {_self, "active"_n}).send(from, msg_id, cs);
   }

   [[eosio::action]]
   void sendsha( name from, name to, uint64_t msg_id, const checksum256& msg_sha ) {
      // if not authorized then this action is aborted and transaction is rolled back
      // any modifications by other actions in same transaction are undone
      require_auth( from ); // make sure authorized by account

      // Add the specified account to set of accounts to be notified
      require_recipient( to );

      // print for debugging
      eosio::print("sha: ", msg_sha, "\n");

      // message_index is typedef of our multi_index over table address
      // message table is auto "created" if needed
      message_index messages( _self, _self.value ); // code, scope

      // verify does not already exist
      // multi_index find on primary index which in our case is account
      auto itr = messages.find( msg_id );
      if( itr != messages.end() ) {
         std::string err = "Message id already exists: " + std::to_string(msg_id);
         eosio_assert( false, err.c_str() );
      }

      // add to table, first argument is account to bill for storage
      // each entry will be billed to the associated account
      // we could have instead chosen to bill _self for all the storage
      messages.emplace( from /*payer*/, [&]( auto& msg ) {
         msg.msg_id = msg_id;
         msg.from = from;
         msg.to = to;
         msg.msg_sha = msg_sha;
      } );

      // send msgnotify to 'to' account
      eosio::action( eosio::permission_level{_self, "active"_n},
                     to, "msgnotify"_n, std::make_tuple( from, msg_id, msg_sha) ).send();
   }

   [[eosio::action]]
   void removemsg( name to, uint64_t msg_id ) {
      require_auth( _self ); // make sure authorized by account

      message_index messages( _self, _self.value ); // code, scope

      // verify already exist
      auto itr = messages.find( msg_id );
      if( itr == messages.end() ) {
         std::string err = "Message does not exist: " + std::to_string(msg_id);
         eosio_assert( false, err.c_str() );
      }

      // verify correct to
      if( itr->to != to ) {
         std::string err = "Message with id " + std::to_string(msg_id) + " is to: " +
               itr->to.to_string() + " not: " + to.to_string();
         eosio_assert( false, err.c_str() );
      }

      messages.erase( itr );
   }

   [[eosio::action]]
   void removesha( const checksum256& msg_sha ) {
      require_auth( _self ); // make sure authorized by account

      message_index messages( _self, _self.value ); // code, scope

      auto index = messages.get_index<"msgsha"_n>();
      auto itr = index.find( msg_sha );
      if( itr == index.end() ) { // verify already exist
         eosio::print("sha: ", msg_sha, "\n");
         eosio_assert( false, "Unable to find msg sha" );
      }

      index.erase( itr );
   }

   [[eosio::action]]
   void printmsg( name other ) {
      message_index messages( other, other.value ); // code, scope

      for (auto& msg : messages) {
         eosio::print("from: ", msg.from, ", to: ", msg.to, "\n");
      }
   }

   [[eosio::action]]
   void addgroup( name group_name, const std::vector<name>& accounts ) {
      require_auth( _self ); // make sure authorized by account

      group_index groups( _self, _self.value ); // code, scope

      auto itr = groups.find( group_name.value );
      if( itr != groups.end() ) {
         // found, so update
         groups.modify( itr, eosio::same_payer, [&]( auto& group ) {
            group.accounts = accounts;
         });
      } else {
         groups.emplace( _self /*payer*/, [&]( auto& group ) {
            group.group_name = group_name;
            group.accounts = accounts;
         } );
      }
   }

   [[eosio::action]]
   void removegroup( name group_name ) {
      require_auth( _self ); // make sure authorized by account

      group_index groups( _self, _self.value ); // code, scope

      auto itr = groups.find( group_name.value );
      if( itr == groups.end() ) {
         std::string err = "group does not exist: " + group_name.to_string();
         eosio_assert( false, err.c_str() );
      }

      groups.erase( itr );
   }

   [[eosio::action]]
   void sendgroup( name from, name group_name, uint64_t msg_id, const string& msg_str ) {
      require_auth( from );

      group_index groups( _self, _self.value ); // code, scope

      // calculate sha256 from msg_str
      checksum256 cs = eosio::sha256( msg_str.c_str(), msg_str.size() );

      // print for debugging
      eosio::print("sha: ", cs, "\n");

      // message_index is typedef of our multi_index over table address
      // message table is auto "created" if needed
      message_index messages( _self, _self.value ); // code, scope

      // verify does not already exist
      // multi_index find on primary index which in our case is account
      if( messages.find( msg_id ) != messages.end() ) {
         std::string err = "Message id already exists: " + std::to_string(msg_id);
         eosio_assert( false, err.c_str() );
      }

      // add to table, first argument is account to bill for storage
      // each entry will be billed to the associated account
      // we could have instead chosen to bill _self for all the storage
      messages.emplace( from /*payer*/, [&]( auto& msg ) {
         msg.msg_id = msg_id;
         msg.from = from;
         msg.to = group_name;
         msg.msg_sha = cs;
      } );

      auto itr = groups.find( group_name.value );
      if( itr != groups.end() ) {
         // found, so send
         for (auto& a : itr->accounts ) {

            // send msgnotify to 'to' account
            eosio::action( eosio::permission_level{_self, "active"_n},
                           a, "msgnotify"_n, std::make_tuple( from, msg_id, cs) ).send();
         }
      } else {
         std::string err = "group does not exist: " + group_name.to_string();
         eosio_assert( false, err.c_str() );
      }
   }

   [[eosio::action]]
   void addblacklist( name n) {
      require_auth( _self );

      blacklist_index blacklist( _self, _self.value );

      auto itr = blacklist.find( n.value );
      if( itr != blacklist.end() ) {
         // found, no need to waste cpu/bandwidth so assert
         eosio_assert( false, "Already in blacklist" );
      } else {
         blacklist.emplace( _self /*payer*/, [&]( auto& b ) {
            b.account = n;
         } );
      }
   }

   [[eosio::action]]
   void rmblacklist( name n ) {
      require_auth( _self );

      blacklist_index blacklist( _self, _self.value );

      auto itr = blacklist.find( n.value );
      if( itr != blacklist.end() ) {
         blacklist.erase( itr );
      } else {
         eosio_assert( false, "Not in blacklist" );
      }
   }

   [[eosio::action]]
   void spam( name from, name to, uint64_t msg_id, const string& msg_str, uint32_t delay_sec ) {
      require_auth( from );

      if( msg_id > 10 )
         return;

      {
         eosio::transaction out;
         out.actions.emplace_back( eosio::permission_level{_self, "active"_n}, to, "sendmsg"_n,
                                   std::make_tuple( from, to, ++msg_id, msg_str ));
         out.delay_sec = delay_sec;
         out.send( msg_id, _self );
      }
      {
         eosio::transaction out;
         out.actions.emplace_back( eosio::permission_level{_self, "active"_n}, to, "spam"_n,
                                   std::make_tuple( from, to, ++msg_id, msg_str, delay_sec ));
         out.delay_sec = delay_sec;
         out.send( msg_id, _self );
      }
   }

   [[eosio::action]]
   void msgnotify( name from, uint64_t /*msg_id*/, const checksum256& /*msg_sha*/ ) {
      eosio::print("message from ", from, "\n");
      blacklist_index blacklist( _self, _self.value );

      // if on blacklist assert
      auto itr = blacklist.find( from.value );
      if( itr != blacklist.end() ) {
         eosio_assert(false, "Account on blacklist");
      }
   }

   using msgnotify_action = eosio::action_wrapper<"msgnotify"_n, &messenger::msgnotify>;

private:

   struct [[eosio::table]] message {
      uint64_t msg_id;      // unique identifier for message
      name from;            // account message sent from
      name to;              // account message sent to
      checksum256 msg_sha;  // sha256 of message string

      uint64_t primary_key() const { return msg_id; }

      const checksum256& by_msg_sha() const { return msg_sha; }
      uint64_t by_from() const { return from.value; }
   };

   typedef eosio::multi_index<"message"_n, message,
         indexed_by<"msgsha"_n, const_mem_fun<message, const checksum256&, &message::by_msg_sha>>,
         indexed_by<"from"_n, const_mem_fun<message, uint64_t, &message::by_from>>
   > message_index;


   struct [[eosio::table]] group {
      name group_name;
      std::vector<name> accounts; /// The accounts that are member of this group

      uint64_t primary_key() const { return group_name.value; }
   };

   typedef eosio::multi_index<"group"_n, group> group_index;

   struct [[eosio::table]] blacklist {
      name account;

      uint64_t primary_key() const { return account.value; }
   };

   typedef eosio::multi_index<"blacklist"_n, blacklist> blacklist_index;

};

EOSIO_DISPATCH( messenger, (sendmsg)( sendsha )( removemsg )( removesha )( printmsg )
                           ( addgroup )( removegroup )( sendgroup )
                           ( spam )
                           ( addblacklist )( rmblacklist )
                           ( msgnotify  ))
