#include <blanc/utils.hpp>

int main(int argc, char** argv) {
   std::vector<std::string> args{argv+1, argv+argc};

   if (!blanc::has_option(args, "target")) {
      args.insert(args.begin(), "--target=wasm32");
   }
   return blanc::exec_subprogram(TOOL_BACKEND, args, true);
}
