
#include <eosio/tester.hpp>

int main(int argc, char** argv);

extern "C" {
__attribute__((eosio_wasm_import)) void initialize(int64_t, int64_t, int64_t) {}
__attribute__((eosio_wasm_import)) int start(void (*f)()) {
  std::vector<std::string> args = eosio::get_args();
   char buf[] = "eosio-tester";
   std::vector<char*> argv;
   argv.push_back(buf);
   for(std::string& s : args) {
      argv.push_back(const_cast<char*>(s.data()));
   }
   return main(argv.size(), argv.data());
}
}
