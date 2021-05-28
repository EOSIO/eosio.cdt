#include <eosio/eosio.hpp>

using namespace eosio;
using std::map;
using std::string;

class [[eosio::contract]] nested_container : public contract {
public:
   using contract::contract;

    [[eosio::action]] 
    void map2map(map<string, string> m, map<string, map<string, string>> m2m) {}
};
