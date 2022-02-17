#include <blanc/utils.hpp>
#include <blanc/tokenize.hpp>
#include <boost/ut.hpp>

using namespace blanc;
using namespace boost::ut;

int main() {
   "tokenize"_test = [] {
      auto tokens = tokenize("foo::bar(hello), baz(world)");
      expect(
         tokens[0] == "foo::bar(hello)" &&
         tokens[1] == "baz(world)"
      );

      tokens = tokenize(remove_quotes("foo -bar=a --baz b -Wl,-qux=c -Dquux=\"d\""), R"(\s+)");
      expect(
         tokens[0] == "foo" &&
         tokens[1] == "-bar=a" &&
         tokens[2] == "--baz" &&
         tokens[3] == "b" &&
         tokens[4] == "-Wl,-qux=c" &&
         tokens[5] == "-Dquux=d"
      );
   };

   return 0;
}
