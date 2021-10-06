#include <eosio/eosio.hpp>
    
using namespace std;
using namespace eosio;
    
CONTRACT explicit_nested_tests : public contract {
   public:
   using contract::contract;

   explicit_nested_tests(name receiver, name code, datastream<const char *> ds):
      contract(receiver, code, ds),test_table(receiver, receiver.value){}
    
    ACTION nothing() {
     std::array<std::vector<std::optional<float>>, 4> arrv4;
     std::vector<std::array<std::optional<float>, 4>> varr4;
   }; 
   typedef std::optional<float> opt_float;
   typedef std::vector<opt_float> vec_opt_float; 

   TABLE testdata {
      uint64_t  id;
      std::map<uint64_t, std::optional<int> > data; 
      std::vector<std::optional<std::pair<uint64_t,float>>> data_vec;
      std::vector<std::pair<uint64_t, opt_float >> data_vec2;
      vec_opt_float data_vec3;
      std::tuple<uint64_t, std::optional<float>, std::vector<int>> tup1;
      std::variant<uint64_t, std::optional<std::pair<int, float>>, std::vector<int>> var1;
      auto primary_key() const { return id; }

      EOSLIB_SERIALIZE(testdata, (id)(data)(data_vec)(data_vec2)(data_vec3)(tup1)(var1));
   };
   using test_data_idx = multi_index<"testdata"_n, testdata>;

   [[eosio::action]]
    //usage: cleos -v push action eosio vvstr '[[["abc", "cde"],["def","fgh"]]]' -p eosio@active
   std::vector<std::vector<std::string>> vvstr(std::vector<std::vector<std::string>> input) {
      std::vector<std::vector<std::string>> output = input;
      for(auto & row : output) {
         for(auto & word : row) {
            eosio::cout << " " << word;
         }
         eosio::cout << "\n";
      }
      return output;
   } 
   [[eosio::action]]
   // usage : cleos -v push action eosio vvvstr '[[[["abc", "cde"],["def","fgh"]]]]' -p eosio@active
   std::vector<std::vector<std::vector<std::string>>> vvvstr(std::vector<std::vector<std::vector<std::string>>> input) {
      std::vector<std::vector<std::vector<std::string>>> output = input;
      for(auto & vecvec : output){
         for(auto & row : vecvec) {
            for(auto & word : row) {
               eosio::cout << " " << word;
            }
            eosio::cout << "\n";
         }
      }
      return output;
   } 

   [[eosio::action]]
   //usage : cleos -v push action eosio mapo '[[{"key":1,"value":2.0},{"key":2,"value":3.9}]]'  -p eosio@active
   std::map<uint64_t, std::optional<float> >  mapo(std::map<uint64_t, std::optional<float>>  input){
      std::map<uint64_t, std::optional<float>> output = input;
      return output;
   }

   [[eosio::action]]
   // usage : cleos -v push action eosio vecop '[[{"first":1,"second":2.0},{"first":2,"second":3.9}]]'  -p eosio@active
   std::vector<std::optional<std::pair<uint64_t,float>>>  vecop(std::vector<std::optional<std::pair<uint64_t,float>>>  input){
      std::vector<std::optional<std::pair<uint64_t,float>>> output = input;
      return output;
   }

   [[eosio::action]]
    // usage : cleos -v push action eosio vecpo '[[{"first":1,"second":2.0},{"first":2,"second":3.9}]]'  -p eosio@active
   std::vector<std::pair<uint64_t, opt_float >>  vecpo(std::vector<std::pair<uint64_t, opt_float >>  input){
      std::vector<std::pair<uint64_t, opt_float >> output = input;
      return output;
   }

   [[eosio::action]]
   // usage : cleos -v push action eosio vecpoe '[[{"first":1,"second":2.0},{"first":2,"second":3.9}]]'  -p eosio@active
   std::vector<std::pair<uint64_t, std::optional<float> >>  vecpoe(std::vector<std::pair<uint64_t, std::optional<float> >>  input){
      std::vector<std::pair<uint64_t, std::optional<float> >> output = input;
      return output;
   }

   [[eosio::action]]
   // usage : cleos -v push action eosio tup '[{"field_0":1,"field_1":2.0,"field_2":[4,5,6,7]}]'  -p eosio@active
   std::tuple<uint64_t, std::optional<float>, std::vector<int>>  tup(std::tuple<uint64_t, std::optional<float>, std::vector<int>>  input){
      std::tuple<uint64_t, std::optional<float>, std::vector<int>> output = input;
      return output;
   }

   [[eosio::action]]
    // usage : cleos -v push action eosio var '[["uint64",8]]'  -p eosio@active
    // usage : cleos -v push action eosio var '[["B_optional_B_pair_int32_float32_E_E", {"first":1,"second":2.0}]]'  -p eosio@active
    // usage : cleos -v push action eosio var '[["B_vector_int32_E", [2,3,4]]]'  -p eosio@active
   std::variant<uint64_t, std::optional<std::pair<int, float>>, std::vector<int>>  var(std::variant<uint64_t, std::optional<std::pair<int, float>>, std::vector<int>>  input){
      std::variant<uint64_t, std::optional<std::pair<int, float>>, std::vector<int>> output = input;
      return output;
   }

   test_data_idx test_table;
};
