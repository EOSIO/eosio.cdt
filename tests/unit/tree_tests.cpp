#include <eosiolib/eosio.hpp>
#include <eosiolib/tree.hpp>
#include <eosiolib/string.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio;
using namespace eosio::native;

EOSIO_TEST_BEGIN(tree_test)
   //silence_output(true);
   binary_search_tree<int> bt; 
   bt.insert(32);
   bt.insert(12);
   bt.insert(50);
   bt.insert(7);
   std::vector<int> inorder = bt.inorder();
   std::vector<int> inorder_test = {7,12,32,50};
   
   REQUIRE_EQUAL(inorder.size(), inorder_test.size());

   for (int i=0; i < inorder.size(); i++) {
      REQUIRE_EQUAL(inorder[i], inorder_test[i]);
   }
   eosio::print_f("BST %\n", bt.clone());

   CHECK_EQUAL(bt.has(3), false);
   CHECK_EQUAL(bt.has(7), true);
   CHECK_EQUAL(bt.has(12), true);
   CHECK_EQUAL(bt.has(32), true);
   CHECK_EQUAL(bt.has(50), true);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test)
   //silence_output(true);
   //ctstring str0("hello");
   //ctstring str1(", world");
   //ctstring str2 = str0 + str1;
   //ctstring str2 = ctstring("hello").cat(ctstring(", world")); //+ctstring("\n");
   //eosio::print(str2.c_str());
   rope r("hello");
   //r.print();
   r.append(", world");
   r.append(" and more!");
   r += " please!";
   r += " some more ";
   r.print();
   //return;
   //r.append(" please!");
   //r.print();
   {
      uint64_t t0 = __builtin_readcyclecounter();
      rope r("hello");
      r += ", world";
      r += " please!";
      r += " some more";
      r += " text";
      r += ", world";
      r += " please!";
      r += " some more";
      r += " text";
      r += ", world";
      r += " please!";
      r += " some more";
      r += " text";

      //r.print();
      //char buff[r.size()];
      //r.c_str(buff);
      //eosio::print(buff);
      uint64_t t1 = __builtin_readcyclecounter();
      eosio::print("TIME1 : ", t1-t0, "\n");

      t0 = __builtin_readcyclecounter();
      std::string s("hello");
      s += ", world";
      s += " please!";
      s = s + " some more " + "text";
      s += ", world";
      s += " please!";
      s = s + " some more " + "text";
      s += ", world";
      s += " please!";
      s = s + " some more " + "text";

      t1 = __builtin_readcyclecounter();
      eosio::print("TIME2 : ", t1-t0, "\n");
   }

EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(tree_test);
   EOSIO_TEST(string_test);
   return has_failed();
}
