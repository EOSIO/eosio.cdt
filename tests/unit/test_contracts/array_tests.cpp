#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>
#include <eosio/table.hpp>

using namespace eosio;

class [[eosio::contract]] array_tests : public contract {
   public:
      using contract::contract;

   TABLE tests {
      uint64_t                    id;
      std::array<uint8_t,32>      str;
      uint64_t primary_key()      const { return id; }
   };
  
   struct info {
      int age;
      std::string name;
   };

   typedef multi_index<name("tests"), tests> tests_table;
   
   struct my_struct {
      uint32_t id;
      std::array<std::array<std::string,4>,2> aastr;

      bool operator==(const my_struct& b) const {
         return id == b.id &&
                aastr == b.aastr;
      }
   };

   struct [[eosio::table]] my_table_array : eosio::kv::table<my_struct, "arraykv"_n> {
      KV_NAMED_INDEX("id"_n, id)

      my_table_array(eosio::name contract_name) {
         init(contract_name, id);
      }
   };
   my_struct s1;
   my_struct s2;
    // test nested std::array used in kv talbe
   [[eosio::action]]
   void testkv() {
      my_table_array tarr{get_self()};

      s1.id = 1;
      s1.aastr[0] = {"abc","bcd","cde", "def"};
      s1.aastr[1] = {"hij","ijk","jkl", "klm"};
      s2.id = 2,
      s2.aastr[0] = {"opq","pqr","qrs", "rst"};
      s2.aastr[1] = {"uvw","vwx","wxy", "xyz"};

      tarr.put(s1, get_self());
      tarr.put(s2, get_self());
      auto itarr = tarr.id.begin();
      auto itarr_e = tarr.id.end();
      eosio::cout << "print table:: \n";
      while(itarr != itarr_e){
         auto row = itarr.value();
         eosio::cout << "id=" << row.id << "\n";
         for(int i = 0; i < row.aastr.size(); ++i) {
             for(int j = 0; j < row.aastr[i].size(); ++j){
                 eosio::cout << row.aastr[i][j] << " ";
             }
             eosio::cout << "\n";
         } 
         ++itarr;
      }
   }
    
   // test inside using std::array
   [[eosio::action]]
   void testin(std::string message) {
      tests_table _tests(get_self(), get_self().value);

      std::array<uint8_t, 32> str = {'a','a','a','a','a','a','a','a',
                                        'a','a','a','a','a','a','a','a',
                                        'a','a','a','a','a','a','a','a',
                                        'a','a','a','a','a','a','a','a' };
      int len =  message.length() < 32 ? message.length() : 32;
      for(int i = 0; i < len ; ++i){
         str[i] = (uint8_t)message[i]; 
      }   
         
      std::array<uint8_t, 32> str2 = str;
      eosio::cout << "size of std::array str is : " << str.size() << "\n";    
      for(int i = 0; i < 32; ++i){
         eosio::cout << str[i] << " ";
      }
      eosio::cout << "\n";
      for(int i = 0; i < 32; ++i){
         eosio::cout << str2[i] << " ";
      }
      eosio::cout << "\n";
      std::array<info, 2> info_arr;
      info_arr[0].age = 20;  
      info_arr[0].name = "abc";
      info_arr[1].age = 21;
      info_arr[1].name = "cde"; 
      for(int i = 0; i < 2; ++i){
         eosio::cout << info_arr[i].age << " " << info_arr[i].name << "\n";
      } 
   }

   // test parameter  using std::array
   // not supported so far
   [[eosio::action]]
   void testpa(std::array<int,4> input){
      std::array<int,4> arr = input;
      for(int i = 0; i < 4; ++i){
         eosio::cout << arr[i] << " ";
      }
      eosio::cout << "\n";
   }
 
   // test return using std::array, not supported so far
   [[eosio::action]]
   // cleos -v push action eosio testre '[[1,2,3,4]]' -p eosio@active
   std::array<int,4> testre(std::array<int,4> input){
      std::array<int,4> arr = input;
      for(auto & v : arr) v += 1; 
      return arr;
   }

   // test return using std::vector
   [[eosio::action]]
   // cleos -v push action eosio testrev '[[1,2,3,4]]' -p eosio@active
   std::vector<int> testrev(std::vector<int> input){
      std::vector<int> vec = input;
      for(auto & v : vec) v += 1;
      return vec;
   }

   // test nested array
   [[eosio::action]]
   void testne() {
      std::array<tests,2> nest;
      std::array<uint8_t, 32> str = {'a','a','a','a','a','a','a','a',
                                     'a','a','a','a','a','a','a','a',
                                     'a','a','a','a','a','a','a','a',
                                     'a','a','a','a','a','a','a','a' };
      
      nest[0].id = 1;
      nest[0].str = str;
      nest[1].id = 2;
      nest[1].str = str;
      for(int i = 0; i < nest.size(); ++i){
         eosio::cout << nest[i].id << "   " ;
         for(int j = 0; j < nest[i].str.size(); ++j) {
            eosio::cout << nest[i].str[j] + i << " ";
         }
         eosio::cout << "\n";
      }
      std::array<std::array<std::string, 5>, 3> nest2;
      for(int i = 0; i < nest2.size(); ++i){
         for(int j = 0; j < nest2[i].size(); ++j) {
            nest2[i][j] = "test nested ";
            eosio::cout << nest2[i][j] << " ";
         }
         eosio::cout << "\n";
      }      
   }
 
   // test complex data
   [[eosio::action]]
   void testcom(name user) {
      require_auth(user);
      tests_table _tests(get_self(), get_self().value);

      std::array<uint8_t, 32> str = {'a','a','a','a','a','a','a','a',
                                     'a','a','a','a','a','a','a','a',
                                     'a','a','a','a','a','a','a','a',
                                     'a','a','a','a','a','a','a','a' };
      _tests.emplace(user, [&](auto& t) {
         t.id = user.value + std::time(0);  // primary key can't be same 
         t.str = str;
      });
      auto it = _tests.begin();
      auto ite = _tests.end();
      while(it != ite){
         eosio::cout << "id = " << it->id << "\n";
         for(int i = 0; i < it->str.size(); ++i) {
            eosio::cout << it->str[i] << " ";
         }
         eosio::cout << "\n";
         ++it;    
      }
   }  
  
};
