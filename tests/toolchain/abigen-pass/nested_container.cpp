#include <eosio/eosio.hpp>

using namespace eosio;
using std::map;
using std::string;

#define  SETCONTAINERVAL(x) do { \
        require_auth(user); \
        address_index tblIndex(get_self(), get_first_receiver().value); \
        auto iter = tblIndex.find(user.value); \
        if (iter == tblIndex.end()) \
        { \
            tblIndex.emplace(user, [&](auto &row) { \
                        row.key = user; \
                        row.x = x; \
                    }); \
        } \
        else \
        { \
            tblIndex.modify(iter, user, [&]( auto& row ) { \
                        row.x = x; \
                    }); \
        } \
    }while(0)

class [[eosio::contract]] nested_container : public contract {
    private:
        struct [[eosio::table]] person {
            name key;
            map<string, string> m;
            // map<string, map<string, string>> m2m;

            uint64_t primary_key() const { return key.value;}
        };
    
        using address_index = eosio::multi_index<"people"_n, person>;

    public:
        using contract::contract;

        nested_container(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}

        [[eosio::action]] 
        void setm(name user, const map<string, string> &m) {
            SETCONTAINERVAL(m);
            eosio::print("map<string,string> stored successfully");
        }
        
        // [[eosio::action]]
        // void setmm(name user, const map<string, map<string, string>> &m2m) {
        //     SETCONTAINERVAL(m2m);
        //     eosio::print("map<string, map<string, string>> stored successfully");
        // }
};
