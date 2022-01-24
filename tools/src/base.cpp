#include <blanc/utils.hpp>

int main(int argc, char** argv) {
   std::vector<std::string> backends = { TOOL_BACKEND, TOOL_BACKEND_FALLBACK };

   return blanc::exec_subprogram(backends, {argv+1,argv+argc});
}
