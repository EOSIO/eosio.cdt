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

   "get_option_value"_test = [] {
      auto cmd = tokenize("foo -bar=a --baz=b -qux c --quux d -quuz");

      auto bar = get_option_value(cmd, "bar");
      expect(bar && *bar == "a");

      auto baz = get_option_value(cmd, "baz");
      expect(baz && *baz == "b");

      auto qux = get_option_value(cmd, "qux");
      expect(qux && *qux == "c");

      auto quux = get_option_value(cmd, "quux");
      expect(quux && *quux == "d");

      auto quuz = get_option_value(cmd, "quuz");
      expect(!quuz);

      auto waldo = get_option_value(cmd, "waldo");
      expect(!waldo);
   };

   "extract_option"_test = [] {
      auto cmd = tokenize("foo -bar=a --baz=b -qux c --quux d -quuz");

      auto bar = get_option_value(cmd, "bar");
      expect((bool)bar);

      bar = extract_option(cmd, true, "bar");
      expect(bar && *bar == "a");

      bar = get_option_value(cmd, "bar");
      expect(!bar);
   };

   return 0;
}
