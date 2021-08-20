#include <eosio/eosio.hpp>
#include <tuple>
#include <vector>

using namespace eosio;
using std::map;
using std::string;
using std::tuple;
using std::vector;

class [[eosio::contract]] nested_container : public contract {
public:
   using contract::contract;

    [[eosio::action]] 
    void map2map(map<string, string> m, map<string, map<string, string>> m2m) {}

    [[eosio::action]]
    void settuple2(name user, const tuple  <int, double, string, vector<int> >& tp2) {}
};
