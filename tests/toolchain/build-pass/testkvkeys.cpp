/* Purpose:
 *       Explore the use of CDT_REFLECT(...) and types of the key of kv::map
 *       This file is a continuation of nestcont2kv.cpp  and tupletestkv.cpp
 *
 * After adding CDT_REFLECT(...) in related eosio-defined types of eosio.cdt/, then the following types of keys are tested here,
 * and the JOSN format of each type is also explained:
 *      1) eosio::symbol, eosio::symbol_code, eosio::extended_symbol in eosio/symbol.hpp
 *      2) eosio::asset, eosio::extended_asset in eosio/asset.hpp
 *      3) eosio::time_point, eosio::time_point_sec, eosio::microseconds, eosio::microseconds in eosio/time.hpp
 *
 * After fixing a bug in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp, all STL containers and nested containers should
 * be able to serve as the key of kv::map, the following cases are verified here:
 *      4) single-layer STL : vector, set, map, optional, list, deque
 *            where list and deque are used and tested here for the first time
 *
 *      5) two-layer nested containers:
 *              vector<vector<T> >
 *              optional<vector<T> >
 *          There are 6*6=36 possible combinations of nested contasiners, to make this .cpp short, other combinations of nested contasiners
 *          when serving as the key of kv::map should work, please refer to nestcont2a.cpp, nestcont2kv.cpp, tupletest.cpp, tupletestkv.cpp
 *
 *      6) Here I show how a complex struct of struct  mystructrefl2  uses  CDT_REFLECT(...)
 *      7) Also I have verified structs wrapping CDT_REFLECT(...) can also be the VALUE part of kv::map
 *      8) When the key of kv::map is a type involving struct,map,pair, the key can have a shortcut JSON input format,
 *              in short, in kv::map, if its key part or value part is a type involving struct,map,pair,
 *              then shortcut JSON input formats can be used.
 *              See nestcontn2kv.cpp for more examples
 *
 * Attention:
 *   1) Please follow https://github.com/EOSIO/eos/tree/develop/contracts/enable-kv to enable key-value support of nodeos first
 *   2) To get to the point right away, here authentication/permission checking is skipped for each action in this contract,
 *      however in practice, suitable permission checking such as require_auth(user) has to be used!
 *
 * Expected printout:
 *      For each setx action, the printed result on the cleos console is given in its corresponding prntx action.
 *
 */

#include <eosio/eosio.hpp>
#include <eosio/map.hpp>
#include <eosio/asset.hpp>

#include <vector>
#include <set>
#include <deque>
#include <optional>
#include <map>
#include <tuple>
#include <list>

using namespace eosio;
using namespace std;


#define SETCONTAINERVAL(x) do { \
        person2kv obj = mymap[id]; \
        obj.x = x; \
        mymap[id] =  obj; \
    }while(0)

//when the keys of kv::map are of different types, the following sets values of mymap2, mymap3, mymap4 etc.
#define SETCONTAINERVALN(num, x) do { \
        person2kv obj = mymap ## num[id]; \
        obj.x = x; \
        mymap ## num[id] =  obj; \
    }while(0)

//SETCONTAINERVALN2(num,x) is used to set the 2nd table wrapping struct tbl2
#define SETCONTAINERVALN2(num, x) do { \
        tbl2 obj = mymap ## num[id]; \
        obj.x = x; \
        mymap ## num[id] =  obj; \
    }while(0)

//GETITEM(num) is used in the following getx() actions
#define GETITEM(num) do { \
        auto iter = mymap ## num.find(id); \
        check(iter != mymap ## num.end(), "Record does not exist"); \
        const auto& item_found = iter->second(); \
        return item_found; \
    }while(0)

//PRNTVAL(num) is used in the following prntvx() actions
#define PRNTVAL(num) do { \
        const auto& psn = get ## num(id); \
        eosio::print("size of stored v:", psn.v.size()," vals:"); \
        for (const auto & ele : psn.v) \
            eosio::print(ele, ",");  \
    }while(0)

struct mystruct
{
    uint64_t   _count;
    string     _strID;
};

//mystruct2 has embeded mystruct
struct mystruct2
{
    mystruct   _structfld;
    string     _strID2;
};

//mystructrefl has CDT_REFLECT wrapper over above mystruct, it is the key of following my_map_t4,
//if the key of an kv::map is a self-defined struct, the struct has to be self-reflective
struct mystructrefl
{
    uint64_t   _count;
    string     _strID;
    CDT_REFLECT(_count, _strID);
};

struct mystructrefl2
{//mystructrefl2 has a reflected struct, here both mystructrefl2 and its embedded mystructrefl are 'reflected' using CDT_REFLECT
    mystructrefl _structfld;
    string     _strID2;
    CDT_REFLECT(_structfld, _strID2);
};


// use typedefs to make multi-layer nested containers work!
typedef vector<uint16_t> vec_uint16;


struct person2kv {
    vector<uint16_t> v;
};


struct tbl2 {
    //Intentionally use the 2nd struct  to be wrapped by kv::map to verify multi-table-operations are allowed in kv::map,
    //new types of keys such as eosio::asset, eosio::symbol, vector<T> etc. are apppied to tbl2
    vector<uint16_t> v;

    //The following data members of tbl2 are structs that wrap CDT_REFLECT(...)
    eosio::name ename;
    eosio::asset easset;
    mystructrefl myval;
};


class [[eosio::contract("testkvkeys")]] testkvkeys : public eosio::contract {
    using my_map_t = eosio::kv::map<"kvtb1"_n, int, person2kv>;

    /*
     * The following kv::map have different key types: std::string, eosio::name, mystructrefl, eosio::asset
     * eosio does NOT accept 0 and 6-9, so "kvtb000", "kvtb6" are NOT acceptable names
     * on the other hand, it is ok to do
     *      using my_map_t6 = eosio::kv::map<"kvtb11"_n, eosio::symbol, tbl2>;
     * but my_map_t11 is used to avoid confusion.
     *
     * Also the first several my_map_t types wrap the first struct person2kv, my_map_t types starting from my_map_t5
     * wrap the second struct tbl2
     */

    using my_map_t2 = eosio::kv::map<"kvtb2"_n, std::string, person2kv>;
    using my_map_t3 = eosio::kv::map<"kvtb3"_n, eosio::name, person2kv>;
    using my_map_t4 = eosio::kv::map<"kvtb4"_n, mystructrefl, person2kv>;



    /* In the following my_map_t5 to my_map_t23, the keys are defined using the types in  eosio/symbol.hpp, asset.hpp and time.hpp
     * which can be :
     *  - eosio::symbol, eosio::symbol_code, eosio::extended_symbol in eosio/symbol.hpp
     *  - eosio::asset, eosio::extended_asset in eosio/asset.hpp
     *  - eosio::time_point, eosio::time_point_sec, eosio::microseconds, eosio::microseconds in eosio/time.hpp
     */
    using my_map_t5 = eosio::kv::map<"kvtb5"_n, eosio::asset, tbl2>;


    using my_map_t11 = eosio::kv::map<"kvtb11"_n, eosio::symbol, tbl2>;
    using my_map_t12 = eosio::kv::map<"kvtb12"_n, eosio::symbol_code, tbl2>;
    using my_map_t13 = eosio::kv::map<"kvtb13"_n, eosio::time_point, tbl2>;
    using my_map_t14 = eosio::kv::map<"kvtb14"_n, eosio::time_point_sec, tbl2>;

    //eosio::extended_symbol, eosio::microseconds are NOT built-in types of eosio.cdt and eos/
    using my_map_t15 = eosio::kv::map<"kvtb15"_n, eosio::extended_symbol, tbl2>;
    using my_map_t21 = eosio::kv::map<"kvtb21"_n, eosio::microseconds, tbl2>;

    //eosio::extended_asset and eosio::block_timestamp are built_in types of eosio.cdt and eos/,
    //  but there are no from_json(), to_json() implementation in eos/
    using my_map_t22 = eosio::kv::map<"kvtb22"_n, eosio::extended_asset, tbl2>;
    using my_map_t23 = eosio::kv::map<"kvtb23"_n, eosio::block_timestamp, tbl2>;

    /* In the following my_map_t, different kinds of STL containers serve as the keys of kv::map,
     * as specified by various to_key() specializations in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp,
     * There are many different kinds of first-layer, two-layer combinations, but for simpilicity and just illustrate
     * nested containers can serve as the keys of kv::map, the following are shown here:
     *      optional<T>
     *      vector<T>
     *      set<T>
     *      list<T>
     *      map<K,V>
     *      deque<T>
     *      vector<vector<T> >
     *      optional<vector<T> >
     *
     * Please note: std:list<T>, std::deque<list> have the same JSON input format as std::set<T> and std::vector<T> !
     */
    using my_map_t24 = eosio::kv::map<"kvtb24"_n, std::optional<uint16_t>, tbl2>;
    using my_map_t25 = eosio::kv::map<"kvtb25"_n, std::vector<uint16_t>, tbl2>;
    using my_map_t25a = eosio::kv::map<"kvtb25a"_n, std::set<uint16_t>, tbl2>;
    using my_map_t35 = eosio::kv::map<"kvtb35"_n, std::list<uint16_t>, tbl2>;
    using my_map_t31 = eosio::kv::map<"kvtb31"_n, std::map<uint16_t, uint16_t>, tbl2>;
    using my_map_t32 = eosio::kv::map<"kvtb32"_n, std::deque<uint16_t>, tbl2>;
    using my_map_t33 = eosio::kv::map<"kvtb33"_n, std::vector<vec_uint16>, tbl2>;
    using my_map_t34 = eosio::kv::map<"kvtb34"_n, std::optional<vec_uint16>, tbl2>;



    //The following uses mystructrefl2, which is a struct of structs as the key of kv::map
    using my_map_t41= eosio::kv::map<"kvtb41"_n, mystructrefl2, tbl2>;


    /* The following map my_map_t55 is used to verify it is ok to have a kv::map whose VALUE part is a struct
     * which has fields that use CDT_REFLECT.
     *
     * Image the VALUE part of your kv::map has a field that is a self-defined struct/class say myclsx,
     * now add some methods into myclsx, without changing data members of myclsx, kv::map should still work,
     * adding CDT_REFLECT(...) into myclsx means adding 5 member functions expanded from CDT_REFLECT(...),
     * so kv::map should still work with no issue.
     */
    using  my_map_t55= eosio::kv::map<"kvtb55"_n, int, tbl2>;

    private:
        my_map_t mymap{};
        my_map_t2 mymap2{};
        my_map_t3 mymap3{};
        my_map_t4 mymap4{};
        my_map_t5 mymap5{};


        my_map_t11 mymap11{};
        my_map_t12 mymap12{};
        my_map_t13 mymap13{};
        my_map_t14 mymap14{};
        my_map_t15 mymap15{};

        my_map_t21 mymap21{};
        my_map_t22 mymap22{};
        my_map_t23 mymap23{};
        my_map_t24 mymap24{};
        my_map_t25 mymap25{};
        my_map_t25a mymap25a{};

        my_map_t31 mymap31{};
        my_map_t32 mymap32{};
        my_map_t33 mymap33{};
        my_map_t34 mymap34{};
        my_map_t35 mymap35{};

        my_map_t41 mymap41{};

        my_map_t55 mymap55{};


    public:
        using contract::contract;

        testkvkeys(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

        // === Use int as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get '[1]' -p alice@active
        [[eosio::action]]
        person2kv get(int id)
        {
            auto iter = mymap.find(id);
            check(iter != mymap.end(), "Record does not exist");
            const auto& item_found = iter->second();
            return item_found;
        }

         /*Examples:
          * cleos --verbose push action testkvkeys setv '[1,[100,200,300,600]]' -p alice@active
          * cleos --verbose push action testkvkeys setv '[2, []]' -p bob@active
          */
        [[eosio::action]]
        void setv(int id, const vector<uint16_t>& v)
        {
            SETCONTAINERVAL(v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
         *  cleos --verbose push action testkvkeys prntv '[1]' -p alice@active
         *      output: >> size of stored v:4 vals:100,200,300,600,
         *
         *  cleos --verbose push action testkvkeys prntv '[2]' -p bob@active
         *      output: >> size of stored v:0 vals:
         */
        [[eosio::action]]
        void prntv(int id)
        {
            const auto& psn = get(id);
            eosio::print("size of stored v:", psn.v.size()," vals:");
            for (const auto & ele : psn.v)
                eosio::print(ele, ",");
        }

        // === Use std::string as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get2 '["str1"]' -p alice@active
        [[eosio::action]]
        person2kv get2(std::string id)
        {
            // auto iter = mymap2.find(id);
            // check(iter != mymap2.end(), "Record does not exist");
            // const auto& item_found = iter->second();
            // return item_found;
            GETITEM(2);
        }

        /*Examples:
        *  cleos --verbose push action testkvkeys setv2 '["str1",[102,202,302,602]]' -p alice@active
        *  cleos --verbose push action testkvkeys setv2 '["str2", []]' -p bob@active
        *
        * Note:
        *   std::string is a STL struct, obviously std::string does NOT have our eosio.cdt-defined CDT_REFLECT(...),
        *   however std::string can still serve as the key of kv::map,
        *   because std::string and many other types have specialized to_key() implementations,
        *   see eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp, which has defined
        *       void to_key(const std::string& obj, datastream<S>& stream)
        */
        [[eosio::action]]
        void setv2(std::string id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN(2, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
        *  cleos --verbose push action testkvkeys prntv2 '["str1"]' -p alice@active
        *      output: >> size of stored v:4 vals:102,202,302,602,
        *
        *  cleos --verbose push action testkvkeys prntv2 '["str2"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv2(std::string id)
        {
            // const auto& psn = get2(id);
            // eosio::print("size of stored v:", psn.v.size()," vals:");
            // for (const auto & ele : psn.v)
            //     eosio::print(ele, ",");
            PRNTVAL(2);
        }


        // === Use eosio::name as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get3 '["name1"]' -p alice@active
        [[eosio::action]]
        person2kv get3(eosio::name id)
        {
           GETITEM(3);
        }

        /*Examples:
        * cleos --verbose push action testkvkeys setv3 '["name1",[103,203,303,603]]' -p alice@active
        * cleos --verbose push action testkvkeys setv3 '["name2", []]' -p bob@active
        */
        [[eosio::action]]
        void setv3(eosio::name id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN(3, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
        *  cleos --verbose push action testkvkeys prntv3 '["name1"]' -p alice@active
        *      output: >> size of stored v:4 vals:103,203,303,603,
        *
        *  cleos --verbose push action testkvkeys prntv3 '["name2"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv3(eosio::name id)
        {
          PRNTVAL(3);
        }


        // === Use self-defined mystructrefl as the key of kv::map
        /*Example:
         *
         *      cleos --verbose push action testkvkeys get4 '[{"_count":18, "_strID":"dumstr"}]' -p alice@active
         *  or shortcut:
         *      cleos --verbose push action testkvkeys get4 '[[18, "dumstr"]]' -p alice@active
         */
        [[eosio::action]]
        person2kv get4(mystructrefl id)
        {
            GETITEM(4);
        }

        /*Examples:
        *   cleos --verbose push action testkvkeys setv4 '[{"_count":18, "_strID":"dumstr"},[104,204,304,604]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv4 '[{"_count":11, "_strID":"dumstr1"}, []]' -p bob@active
        * or shortcut:
        *   cleos --verbose push action testkvkeys setv4 '[[18, "dumstr"],[104,204,304,604]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv4 '[[11, "dumstr1"], []]' -p bob@active
        */
        [[eosio::action]]
        void setv4(mystructrefl id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN(4, v);
            eosio::print("vector<uint16_t> stored successfully");
        }


        /*Examples:
        *  cleos --verbose push action testkvkeys prntv4 '[{"_count":18, "_strID":"dumstr"}]' -p alice@active
        *   or its shortcut: cleos --verbose push action testkvkeys prntv4 '[[18, "dumstr"]]' -p alice@active
        *      output: >> size of stored v:4 vals:104,204,304,604,
        *
        *  cleos --verbose push action testkvkeys prntv4 '[{"_count":11, "_strID":"dumstr1"}]' -p bob@active
        *   or its shortcut: cleos --verbose push action testkvkeys prntv4 '[[11, "dumstr1"]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv4(mystructrefl id)
        {
           PRNTVAL(4);
        }

        // === Use eosio::asset as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get5 '["200.0000 SYS"]' -p alice@active
        [[eosio::action]]
        tbl2 get5(eosio::asset id)
        {
            GETITEM(5);
        }

       /* Examples:
        *   cleos --verbose push action testkvkeys setv5 '["1236 DOLLAR",[105,205,305,605]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv5 '["200.0000 SYS", []]' -p bob@active
        *
        * Note:
        *   Valid eosio::asset JSON strings accepted by nodeos are like "1236 DOLLAR", "200.0000 SYS",
        *   documented in function asset_to_string() in eos/libraries/abieos/include/eosio/chain_conversions.hpp,
        *   and eosio.cdt/tests/unit/asset_tests.cpp
        */
        [[eosio::action]]
        void setv5(eosio::asset id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(5, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
        *  cleos --verbose push action testkvkeys prntv5 '["1236 DOLLAR"]' -p alice@active
        *      output: >> size of stored v:4 vals:105,205,305,605,
        *
        *  cleos --verbose push action testkvkeys prntv5 '["200.0000 SYS"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv5(eosio::asset id)
        {
            PRNTVAL(5);
        }

        // === Use eosio::symbol as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get11 '["18,ABCDEFG"]' -p alice@active
        [[eosio::action]]
        tbl2 get11(eosio::symbol id) //use name get11, because eosio-cpp does NOT accept action names with 0, 6,7,8,9
        {
            GETITEM(11);
        }

        /* Examples:
        *   cleos --verbose push action testkvkeys setv11 '["18,ABCDEFG",[106,206,306,606]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv11 '["3,MSFT", []]' -p bob@active
        *
        * Notes:
        *   1) Valid eosio::symbol JSON strings accepted by nodeos are like "18,ABCDEFG", "3,MSFT",
        *      documented in function symbol_to_string() in eos/libraries/abieos/include/eosio/chain_conversions.hpp,
        *      and eosio.cdt/tests/unit/symbol_tests.cpp
        *   2) The number before comma ',' has to be at most 18, otherwise an exception would be thrown,
        *      as documented in class symbol : fc::reflect_init, which has the following statement
        *      in eos/libraries/chain/include/eosio/chain/symbol.hpp:
        *           static constexpr uint8_t max_precision = 18;
        */
        [[eosio::action]]
        void setv11(eosio::symbol id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(11, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv11 '["18,ABCDEFG"]' -p alice@active
        *      output: >> size of stored v:4 vals:106,206,306,606,
        *
        *  cleos --verbose push action testkvkeys prntv11 '["3,MSFT"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv11(eosio::symbol id)
        {
            PRNTVAL(11);
        }

        // === Use eosio::symbol_code as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get12 '["ABCDEFG"]' -p alice@active
        [[eosio::action]]
        tbl2 get12(eosio::symbol_code id)
        {
            GETITEM(12);
        }

        /* Examples:
        *   cleos --verbose push action testkvkeys setv12 '["ABCDEFG",[107,207,307,607]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv12 '["MSFT", []]' -p bob@active
        *
        * Note:
        *   Valid eosio::symbol_code JSON strings accepted by nodeos are like "ABCDEFG", "MSFT",
        *   documented in function string_to_symbol_code() in eos/libraries/abieos/include/eosio/chain_conversions.hpp,
        *   and eosio.cdt/tests/unit/symbol_tests.cpp
        *
        */
        [[eosio::action]]
        void setv12(eosio::symbol_code id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(12, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv12 '["ABCDEFG"]' -p alice@active
        *      output: >> size of stored v:4 vals:107,207,307,607,
        *
        *  cleos --verbose push action testkvkeys prntv12 '["MSFT"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv12(eosio::symbol_code id)
        {
            PRNTVAL(12);
        }

        // === Use eosio::time_point as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get13 '["2021-07-16T19:20:30.987"]' -p alice@active
        [[eosio::action]]
        tbl2 get13(eosio::time_point id)
        {
            GETITEM(13);
        }

        /* Examples:
        *   cleos --verbose push action testkvkeys setv13 '["2021-07-16T19:20:30.987",[108,208,308,608]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv13 '["2021-09-16T00:18:36.306", []]' -p bob@active
        *
        * Note:
        *   Valid eosio::time_point JSON strings accepted by nodeos are like "2021-07-16T19:20:30.987",
        *   documented in function microseconds_to_str() in eos/libraries/abieos/include/eosio/chain_conversions.hpp,
        *   and eosio.cdt/tests/unit/time_tests.cpp
        *
        */
        [[eosio::action]]
        void setv13(eosio::time_point id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(13, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv13 '["2021-07-16T19:20:30.987"]' -p alice@active
        *      output: >> size of stored v:4 vals:108,208,308,608,
        *
        *  cleos --verbose push action testkvkeys prntv13 '["2021-09-16T00:18:36.306"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv13(eosio::time_point id)
        {
            PRNTVAL(13);
        }

        // === Use eosio::time_point_sec as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get14 '["2021-07-16T19:20:30"]' -p alice@active
        [[eosio::action]]
        tbl2 get14(eosio::time_point_sec id)
        {
            GETITEM(14);
        }

        /* Examples:
        *   cleos --verbose push action testkvkeys setv14 '["2021-07-16T19:20:30",[109,209,309,609]]' -p alice@active
        *   cleos --verbose push action testkvkeys setv14 '["2021-09-16T00:18:36", []]' -p bob@active
        *
        * Note:
        *   Valid eosio::time_point_sec JSON strings accepted by nodeos are like "2021-07-16T19:20:30",
        *   documented in function string_to_utc_seconds() in eos/libraries/abieos/include/eosio/chain_conversions.hpp,
        *   and eosio.cdt/tests/unit/time_tests.cpp
        *
        */
        [[eosio::action]]
        void setv14(eosio::time_point_sec id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(14, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv14 '["2021-07-16T19:20:30"]' -p alice@active
        *      output: >> size of stored v:4 vals:109,209,309,609,
        *
        *  cleos --verbose push action testkvkeys prntv14 '["2021-09-16T00:18:36"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv14(eosio::time_point_sec id)
        {
            PRNTVAL(14);
        }

        // === Use eosio::extended_symbol as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get15 '[{"sym":"18,GOOG", "contract":"ctname1"}]' -p alice@active
        [[eosio::action]]
        tbl2 get15(eosio::extended_symbol id)
        {
            GETITEM(15);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv15 '[{"sym":"18,GOOG", "contract":"ctname1"},[110,210,310,610]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv15 '[{"sym":"3,MSFT", "contract":"ctname2"},[]]' -p alice@active
        *
        * Notes:
        *  1)eosio::extended_symbol  is NOT a built-in type of eos/ or eosio.cdt/, as seen in is_builtin_type() of eosio.cdt/tools/include/eosio/gen.hpp
        *    and abi_serializer::configure_built_in_types() of eos/libraries/chain/abi_serializer.cpp,
        *    and also the generated testkvkeys.abi has the fields definition of  "extended_symbol"
        *
        *    So here the JSON input format of eosio::extended_symbol is like that of above self-defined mystructrefl
        *    where the fields names sym, contract of eosio::extended_symbol have to be specified explicitly in the cleos command
        *
        *  2)sym and contract are the PRIVATE fields of class eosio::extended_symbol,
        *    however you can use these private fields directly in the above cleos command
        */
        [[eosio::action]]
        void setv15(eosio::extended_symbol id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(15, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv15 '[{"sym":"18,GOOG", "contract":"ctname1"}]' -p alice@active
        *      output: >> size of stored v:4 vals:110,210,310,610,
        *
        *  cleos --verbose push action testkvkeys prntv15 '[{"sym":"3,MSFT", "contract":"ctname2"}]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv15(eosio::extended_symbol id)
        {
            PRNTVAL(15);
        }


        // === Use eosio::microseconds as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get21 '[{"_count":20}]' -p alice@active
        [[eosio::action]]
        tbl2 get21(eosio::microseconds id)
        {
            GETITEM(21);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv21 '[{"_count":20},[111,211,311,611]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv21 '[{"_count":21},[]]' -p alice@active
        *
        * Note:
        *   Similar to above eosio::extended_symbol, eosio::microseconds is NOT a built-in type of eos/ or eosio.cdt/,
        *   its JSON input format is like that of above self-defined mystructrefl
        *   where the fields name _count of eosio::extended_symbol has to be specified explicitly in the cleos command
        *
        */
        [[eosio::action]]
        void setv21(eosio::microseconds id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(21, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv21 '[{"_count":20}]' -p alice@active
        *      output: >> size of stored v:4 vals:111,211,311,611,
        *
        *  cleos --verbose push action testkvkeys prntv21 '[{"_count":21}]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv21(eosio::microseconds id)
        {
            PRNTVAL(21);
        }


         // === Use eosio::extended_asset as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get22 '[{"quantity":"236.0000 SYS", "contract":"ctname1"}]' -p alice@active
        [[eosio::action]]
        tbl2 get22(eosio::extended_asset id)
        {
            GETITEM(22);
        }

        /* Examples:
        *       cleos --verbose push action testkvkeys setv22 '[{"quantity":"236.0000 SYS", "contract":"ctname1"},[112,212,312,612]]' -p alice@active
        *       cleos --verbose push action testkvkeys setv22 '[{"quantity":"321 DOALLAR", "contract":"ctname2"},[]]' -p bob@active
        *
        * Note:
        *   eosio::extended_asset is a built-in type in eos/ and eosio.cdt, examples of its json input format are
        *   given in eos/libraries/abieos/src/test.cpp, here its json input format is the same as that of previous self-defined mystructrefl
        *   since eosio::extended_asset has no from_json(), to_json() implementations in  eos/
        */
        [[eosio::action]]
        void setv22(eosio::extended_asset id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(22, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv22 '[{"quantity":"236.0000 SYS", "contract":"ctname1"}]' -p alice@active
        *      output: >> size of stored v:4 vals:112,212,312,612,
        *
        *  cleos --verbose push action testkvkeys prntv22 '[{"quantity":"321 DOALLAR", "contract":"ctname2"}]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv22(eosio::extended_asset id)
        {
            PRNTVAL(22);
        }

        // === Use eosio::block_timestamp as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get23 '["2000-01-01T00:00:00.500"]' -p alice@active
        [[eosio::action]]
        tbl2 get23(eosio::block_timestamp id)
        {
            GETITEM(23);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv23 '["2000-01-01T00:00:00.500",[116,216,316,616]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv23 '["2021-01-21T18:31:28.000",[]]' -p alice@active
        *
        * Note:
        *   eosio:block_timestamp is a built-in type in eos/ and eosio.cdt, it has epoch =Year 2000,
        *   and has block_interval_ms = 500 millisecond or 0.5 sec, see eos/libraries/abieos/src/test.cpp for examples
        *
        */
        [[eosio::action]]
        void setv23(eosio::block_timestamp id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(23, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv23 '["2000-01-01T00:00:00.500"]' -p alice@active
        *      output: >> size of stored v:4 vals:116,216,316,616,
        *
        *  cleos --verbose push action testkvkeys prntv23 '["2021-01-21T18:31:28.000"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv23(eosio::block_timestamp id)
        {
            PRNTVAL(23);
        }

        // Above shows the eosio-defined types can serve as the key of kv::map,
        // the following shows STL components and nested containers can also be the key of kv::map !

        // === Use std::optional<T> as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get24 '[null]' -p alice@active
        [[eosio::action]]
        tbl2 get24(std::optional<uint16_t> id)
        {
            GETITEM(24);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv24 '[null,[117,217,317,617]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv24 '[12,[]]' -p bob@active
        *
        * Note:
        *   std::optional<T> can be the key of kv::map, because the following is coded
        *   in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp:
        *       void to_key(const std::optional<T>& obj, datastream<S>& stream)
        *
        */
        [[eosio::action]]
        void setv24(std::optional<uint16_t> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(24, v);
            eosio::print("vector<uint16_t> is stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv24 '[null]' -p alice@active
        *      output: >> size of stored v:4 vals:117,217,317,617,
        *
        *  cleos --verbose push action testkvkeys prntv24 '[12]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv24(std::optional<uint16_t> id)
        {
            PRNTVAL(24);
        }


         // === Use std::vector<T> as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get25 '[[11,21]]' -p alice@active
        [[eosio::action]]
        tbl2 get25(std::vector<uint16_t> id)
        {
            GETITEM(25);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv25 '[[11,21],[118,218,318,618]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv25 '[[12,22,32],[]]' -p bob@active
        *
        * Note:
        *   std::vector<T> can be the key of kv::map, because the following is added
        *   in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp:
        *       void to_key(const std::vector<T>& obj, datastream<S>& stream)
        *
        */
        [[eosio::action]]
        void setv25(std::vector<uint16_t> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(25, v);
            eosio::print("both the key and value are of type vector<uint16_t>, the value is stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv25 '[[11,21]]' -p alice@active
        *      output: >> size of stored v:4 vals:118,218,318,618,
        *
        *  cleos --verbose push action testkvkeys prntv25 '[[12,22,32]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv25(std::vector<uint16_t> id)
        {
            PRNTVAL(25);
        }

        // === Use std::set<T> as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get25a '[[1,2]]' -p alice@active
        [[eosio::action]]
        tbl2 get25a(std::set<uint16_t> id)
        {
            GETITEM(25a);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv25a '[[1,2],[318,328,338,638]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv25a '[[10,20,30],[]]' -p bob@active
        *
        * Note:
        *   std::set<T> can be the key of kv::map, because the following is added
        *   in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp:
        *       void to_key(const std::set<T>& obj, datastream<S>& stream)
        *
        */
        [[eosio::action]]
        void setv25a(std::set<uint16_t> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(25a, v);
            eosio::print("vector<uint16_t> value stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv25a '[[1,2]]' -p alice@active
        *      output: >> size of stored v:4 vals:318,328,338,638,
        *
        *  cleos --verbose push action testkvkeys prntv25a '[[10,20,30]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv25a(std::set<uint16_t> id)
        {
            PRNTVAL(25a);
        }


        // === Use std::list<T> as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get35 '[[1,2]]' -p alice@active
        [[eosio::action]]
        tbl2 get35(std::list<uint16_t> id)
        {
            GETITEM(35);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv35 '[[1,2],[158,258,358,658]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv35 '[[10,20,30],[]]' -p bob@active
        *
        * Note:
        *   std::list<T> can be the key of kv::map, because the following is added
        *   in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp:
        *       void to_key(const std::list<T>& obj, datastream<S>& stream)
        *
        */
        [[eosio::action]]
        void setv35(std::list<uint16_t> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(35, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv35 '[[1,2]]' -p alice@active
        *      output: >> size of stored v:4 vals:158,258,358,658,
        *
        *  cleos --verbose push action testkvkeys prntv35 '[[10,20,30]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv35(std::list<uint16_t> id)
        {
            PRNTVAL(35);
        }

        // === Use std::map<K,V> as the key of kv::map
        /*Example:
         *      cleos --verbose push action testkvkeys get31 '[[{"key":11,"value":101}, {"key":12,"value":102}]]' -p alice@active
         * or shortcut:
         *      cleos --verbose push action testkvkeys get31 '[[[11,101], [12, 102]]]' -p alice@active
         */
        [[eosio::action]]
        tbl2 get31(std::map<uint16_t, uint16_t> id)
        {
            GETITEM(31);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv31 '[[{"key":11,"value":101}, {"key":12,"value":102}],[119,219,319,619]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv31 '[[{"key":21,"value":201}, {"key":22,"value":202}],[]]' -p bob@active
        * or shortcut:
        *    cleos --verbose push action testkvkeys setv31 '[[[11,101], [17,107]],[119,219,319,619]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv31 '[[[21,201], [23,203]],[]]' -p bob@active
        * Notes:
        *   1) std::map<K,V> can be the key of kv::map, because the following is coded
        *      in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp:
        *           void to_key(const std::map<T, U>& obj, datastream<S>& stream)
        *   2) std::map<K,V> is regarded as an array/vector of "key"-"value" pairs
        *
        */
        [[eosio::action]]
        void setv31(std::map<uint16_t, uint16_t> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(31, v);
            eosio::print("vector<uint16_t> is stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv31 '[[{"key":11,"value":101}, {"key":12,"value":102}]]' -p alice@active
        *   or its shortcut: cleos --verbose push action testkvkeys prntv31 '[[ [11,101], [12,102] ]]' -p alice@active
        *      output: >> size of stored v:4 vals:119,219,319,619,
        *
        *  cleos --verbose push action testkvkeys prntv31 '[[{"key":21,"value":201}, {"key":22,"value":202}]]' -p bob@active
        *    or its shortcut: cleos --verbose push action testkvkeys prntv31 '[[[21,201], [22,202]]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv31(std::map<uint16_t, uint16_t> id)
        {
            PRNTVAL(31);
        }

         // === Use std::deque<T> as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get32 '[[10,11]]' -p alice@active
        [[eosio::action]]
        tbl2 get32(std::deque<uint16_t> id)
        {
            GETITEM(32);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv32 '[[10,11],[120,220,320,620]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv32 '[[20,21,22],[]]' -p bob@active
        *
        * Notes:
        *   1) std::deque<T> can be the key of kv::map, because the following is added
        *       in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp:
        *               void to_key(const std::deque<T>& obj, datastream<S>& stream)
        *   2) std::deque<T> has the JOSN same input as std::vector<T>, std::set<T>
        *
        */
        [[eosio::action]]
        void setv32(std::deque<uint16_t> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(32, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv32 '[[10,11]]' -p alice@active
        *      output: >> size of stored v:4 vals:120,220,320,620,
        *
        *  cleos --verbose push action testkvkeys prntv32 '[[20,21,22]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv32(std::deque<uint16_t> id)
        {
            PRNTVAL(32);
        }

        // === Use std::vector<vector<T> > as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get33 '[[[1,2],[30], [40,50,60]]]' -p alice@active
        [[eosio::action]]
        tbl2 get33(std::vector<vec_uint16> id)
        {
            GETITEM(33);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv33 '[[[1,2],[30], [40,50,60]],[126,226,326,626]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv33 '[[[11,21],[31], [41,51,61]],[]]' -p bob@active
        *
        * Notes:
        *   1) Each to_key() function in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp is a ***recursive*** call
        *           so two-layer nested container such as std::vector<vec_uint16> can be the key of kv::map
        *   2) Above [[1,2],[30], [40,50,60]] is JSON format of std::vector<vec_uint16>, it is a vector of 3 vec_uint16 elements
        *
        */
        [[eosio::action]]
        void setv33(std::vector<vec_uint16> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(33, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv33 '[[[1,2],[30], [40,50,60]]]' -p alice@active
        *      output: >> size of stored v:4 vals:126,226,326,626,
        *
        *  cleos --verbose push action testkvkeys prntv33 '[[[11,21],[31], [41,51,61]]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv33(std::vector<vec_uint16> id)
        {
            PRNTVAL(33);
        }

        // === Use std::optional<vector<T> > as the key of kv::map
        //Example: cleos --verbose push action testkvkeys get34 '[[11, 21]]' -p alice@active
        [[eosio::action]]
        tbl2 get34(std::optional<vec_uint16> id)
        {
            GETITEM(34);
        }

        /* Examples:
        *    cleos --verbose push action testkvkeys setv34 '[[11, 21],[127,227,327,627]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv34 '[null,[]]' -p bob@active
        *
        * Note:
        *   Each to_key() function in eosio.cdt/libraries/eosiolib/core/eosio/key_utils.hpp is a ***recursive*** call
        *           so two-layer nested container such as std::optional<vec_uint16> can be the key of kv::map
        *
        */
        [[eosio::action]]
        void setv34(std::optional<vec_uint16> id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(34, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntv34 '[[11, 21]]' -p alice@active
        *      output: >> size of stored v:4 vals:127,227,327,627,
        *
        *  cleos --verbose push action testkvkeys prntv34 '[null]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv34(std::optional<vec_uint16> id)
        {
            PRNTVAL(34);
        }


        // === Use mystructrefl2, which is a struct of struct as the key of kv::map
        /*Example:
         *      cleos --verbose push action testkvkeys get41 '[{"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
         * or shortcut:
         *      cleos --verbose push action testkvkeys get41 '[[[18, "dumstr"], "dumstr2"]]' -p alice@active
         */
        [[eosio::action]]
        tbl2 get41(mystructrefl2 id)
        {
            GETITEM(41);
        }

        /*Examples:
        *    cleos --verbose push action testkvkeys setv41 '[{"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"},[118,218,318,618]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv41 '[{"_structfld":{"_count":19, "_strID":"dumstrONE"}, "_strID2":"dumstrTWO2"}, []]' -p bob@active
        * or shortcut:
        *    cleos --verbose push action testkvkeys setv41 '[[ [18, "dumstr"], "dumstr2x"],[118,218,318,618]]' -p alice@active
        *    cleos --verbose push action testkvkeys setv41 '[[ [19, "dumstrONE"], "dumstrTWO2x"], []]' -p bob@active
        */
        [[eosio::action]]
        void setv41(mystructrefl2 id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN2(41, v);
            eosio::print("vector<uint16_t> stored successfully");
        }


        /*Examples:
        *  cleos --verbose push action testkvkeys prntv41 '[{"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
        *   or its shortcut:  cleos --verbose push action testkvkeys prntv41 '[[ [18, "dumstr"], "dumstr2"]]' -p alice@active
        *      output: >> size of stored v:4 vals:118,218,318,618,
        *
        *  cleos --verbose push action testkvkeys prntv41 '[{"_structfld":{"_count":19, "_strID":"dumstrONE"}, "_strID2":"dumstrTWO2"}]' -p bob@active
        *    or its shortcut: cleos --verbose push action testkvkeys prntv41 '[[ [19, "dumstrONE"], "dumstrTWO2"]]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv41(mystructrefl2 id)
        {
           PRNTVAL(41);
        }


        // === In the following case, CDT_REFLECT(...) is in the VALUE part of kv::map
        // Example: cleos --verbose push action testkvkeys get55 '[12]' -p alice@active
        [[eosio::action]]
        tbl2 get55(int id)
        {
            GETITEM(55);
        }

         /* Examples:
        *    cleos --verbose push action testkvkeys setvals55 '[11,"name1", "1236 DOLLAR", {"_count":26, "_strID":"dumstr1"}]' -p alice@active
        *    cleos --verbose push action testkvkeys setvals55 '[12,"name2", "120.000 SYS", {"_count":27, "_strID":"dumstr2"}]' -p bob@active
        *
        *  Note:
        *   Here tbl2 wrapped by kv::map has fields of types eosio::name, eosio::asset, mystructrefl which all have CDT_REFLECT(...)
        *   field v  of tbl12 is std::vector<T> NOT wrapping CDT_REFLECT(...),
        *   so the struct wrapped by kv::map can have fields which have CDT_REFLECT(...), or does not have CDT_REFLECT(...)
        */
        [[eosio::action]]
        void setvals55(int id, const eosio::name& ename, const eosio::asset& easset,  const mystructrefl& myval)
        {
            tbl2 obj = mymap55[id];
            obj.ename = ename;
            obj.easset = easset;
            obj.myval= myval;
            mymap55[id] =  obj;
            eosio::print("fields ename,easset, myval of tbl2 are stored successfully");
        }

         /*Examples:
        *  cleos --verbose push action testkvkeys prntvals55 '[11]' -p alice@active
        *      output:  >> stored ename  = name1
        *               >> stored easset =1236 DOLLAR
        *               >> stored mystructrefl val:26,dumstr1
        *
        *  cleos --verbose push action testkvkeys prntvals55 '[12]' -p bob@active
        *      output:  >> stored ename  = name2
        *               >> stored easset =120.000 SYS
        *               >> stored mystructrefl val:27,dumstr2
        */
        [[eosio::action]]
        void prntvals55(int id)
        {
            const auto& psn = get55(id);
            eosio::print("stored ename  = ", psn.ename,"\n");
            eosio::print("stored easset =", psn.easset,"\n");
            eosio::print("stored mystructrefl val:", psn.myval._count,",", psn.myval._strID);

        }

         //The following are needed for eosio-cpp compiler to identify the actions in generated .abi
        using get_action = eosio::action_wrapper<"get"_n, &testkvkeys::get>;
        using setv_action = eosio::action_wrapper<"setv"_n, &testkvkeys::setv>;
        using prntv_action = eosio::action_wrapper<"prntv"_n, &testkvkeys::prntv>;

        using get2_action = eosio::action_wrapper<"get2"_n, &testkvkeys::get2>;
        using setv2_action = eosio::action_wrapper<"setv2"_n, &testkvkeys::setv2>;
        using prntv2_action = eosio::action_wrapper<"prntv2"_n, &testkvkeys::prntv2>;
        using get3_action = eosio::action_wrapper<"get3"_n, &testkvkeys::get3>;
        using setv3_action = eosio::action_wrapper<"setv3"_n, &testkvkeys::setv3>;
        using prntv3_action = eosio::action_wrapper<"prntv3"_n, &testkvkeys::prntv3>;
        using get4_action = eosio::action_wrapper<"get4"_n, &testkvkeys::get4>;
        using setv4_action = eosio::action_wrapper<"setv4"_n, &testkvkeys::setv4>;
        using prntv4_action = eosio::action_wrapper<"prntv4"_n, &testkvkeys::prntv4>;
        using get5_action = eosio::action_wrapper<"get5"_n, &testkvkeys::get5>;
        using setv5_action = eosio::action_wrapper<"setv5"_n, &testkvkeys::setv5>;
        using prntv5_action = eosio::action_wrapper<"prntv5"_n, &testkvkeys::prntv5>;

        using get11_action = eosio::action_wrapper<"get11"_n, &testkvkeys::get11>;
        using setv11_action = eosio::action_wrapper<"setv11"_n, &testkvkeys::setv11>;
        using prntv11_action = eosio::action_wrapper<"prntv11"_n, &testkvkeys::prntv11>;
        using get12_action = eosio::action_wrapper<"get12"_n, &testkvkeys::get12>;
        using setv12_action = eosio::action_wrapper<"setv12"_n, &testkvkeys::setv12>;
        using prntv12_action = eosio::action_wrapper<"prntv12"_n, &testkvkeys::prntv12>;
        using get13_action = eosio::action_wrapper<"get13"_n, &testkvkeys::get13>;
        using setv13_action = eosio::action_wrapper<"setv13"_n, &testkvkeys::setv13>;
        using prntv13_action = eosio::action_wrapper<"prntv13"_n, &testkvkeys::prntv13>;
        using get14_action = eosio::action_wrapper<"get14"_n, &testkvkeys::get14>;
        using setv14_action = eosio::action_wrapper<"setv14"_n, &testkvkeys::setv14>;
        using prntv14_action = eosio::action_wrapper<"prntv14"_n, &testkvkeys::prntv14>;
        using get15_action = eosio::action_wrapper<"get15"_n, &testkvkeys::get15>;
        using setv15_action = eosio::action_wrapper<"setv15"_n, &testkvkeys::setv15>;
        using prntv15_action = eosio::action_wrapper<"prntv15"_n, &testkvkeys::prntv15>;

        using get21_action = eosio::action_wrapper<"get21"_n, &testkvkeys::get21>;
        using setv21_action = eosio::action_wrapper<"setv21"_n, &testkvkeys::setv21>;
        using prntv21_action = eosio::action_wrapper<"prntv21"_n, &testkvkeys::prntv21>;
        using get22_action = eosio::action_wrapper<"get22"_n, &testkvkeys::get22>;
        using setv22_action = eosio::action_wrapper<"setv22"_n, &testkvkeys::setv22>;
        using prntv22_action = eosio::action_wrapper<"prntv22"_n, &testkvkeys::prntv22>;
        using get23_action = eosio::action_wrapper<"get23"_n, &testkvkeys::get23>;
        using setv23_action = eosio::action_wrapper<"setv23"_n, &testkvkeys::setv23>;
        using prntv23_action = eosio::action_wrapper<"prntv23"_n, &testkvkeys::prntv23>;
        using get24_action = eosio::action_wrapper<"get24"_n, &testkvkeys::get24>;
        using setv24_action = eosio::action_wrapper<"setv24"_n, &testkvkeys::setv24>;
        using prntv24_action = eosio::action_wrapper<"prntv24"_n, &testkvkeys::prntv24>;
        using get25_action = eosio::action_wrapper<"get25"_n, &testkvkeys::get25>;
        using setv25_action = eosio::action_wrapper<"setv25"_n, &testkvkeys::setv25>;
        using prntv25_action = eosio::action_wrapper<"prntv25"_n, &testkvkeys::prntv25>;
        using get25a_action = eosio::action_wrapper<"get25a"_n, &testkvkeys::get25a>;
        using setv25a_action = eosio::action_wrapper<"setv25a"_n, &testkvkeys::setv25a>;
        using prntv25a_action = eosio::action_wrapper<"prntv25a"_n, &testkvkeys::prntv25a>;

        using get31_action = eosio::action_wrapper<"get31"_n, &testkvkeys::get31>;
        using setv31_action = eosio::action_wrapper<"setv31"_n, &testkvkeys::setv31>;
        using prntv31_action = eosio::action_wrapper<"prntv31"_n, &testkvkeys::prntv31>;
        using get32_action = eosio::action_wrapper<"get32"_n, &testkvkeys::get32>;
        using setv32_action = eosio::action_wrapper<"setv32"_n, &testkvkeys::setv32>;
        using prntv32_action = eosio::action_wrapper<"prntv32"_n, &testkvkeys::prntv32>;
        using get33_action = eosio::action_wrapper<"get33"_n, &testkvkeys::get33>;
        using setv33_action = eosio::action_wrapper<"setv33"_n, &testkvkeys::setv33>;
        using prntv33_action = eosio::action_wrapper<"prntv33"_n, &testkvkeys::prntv33>;
        using get34_action = eosio::action_wrapper<"get34"_n, &testkvkeys::get34>;
        using setv34_action = eosio::action_wrapper<"setv34"_n, &testkvkeys::setv34>;
        using prntv34_action = eosio::action_wrapper<"prntv34"_n, &testkvkeys::prntv34>;
        using get35_action = eosio::action_wrapper<"get35"_n, &testkvkeys::get35>;
        using setv35_action = eosio::action_wrapper<"setv35"_n, &testkvkeys::setv35>;
        using prntv35_action = eosio::action_wrapper<"prntv35"_n, &testkvkeys::prntv35>;

        using get41_action = eosio::action_wrapper<"get41"_n, &testkvkeys::get41>;
        using setv41_action = eosio::action_wrapper<"setv41"_n, &testkvkeys::setv41>;
        using prntv41_action = eosio::action_wrapper<"prntv41"_n, &testkvkeys::prntv41>;

        using get55_action = eosio::action_wrapper<"get55"_n, &testkvkeys::get55>;
        using setvals55_action = eosio::action_wrapper<"setvals55"_n, &testkvkeys::setvals55>;
        using prntvals55_action = eosio::action_wrapper<"prntvals55"_n, &testkvkeys::prntvals55>;

};