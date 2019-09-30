/*
 * Regression test for https://github.com/EOSIO/eosio.cdt/issues/527.
 *
 * Verifies that a table can reference its own type.
 */

#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT self_referential_table : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm );
      using hi_action = action_wrapper<"hi"_n, &self_referential_table::hi>;

   private:


      TABLE testtab {
         uint64_t	id;
         name	acc;
         std::string	mdata;

         std::vector<testtab> container;

         auto primary_key() const {
            return id;
         }

      };

      typedef eosio::multi_index< "testtabs"_n, testtab > testtabs;

};
ACTION self_referential_table::hi( name nm ) {
   /* fill in action body */
   // print_f("Name : %\n",nm);

}
