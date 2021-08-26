/* Verify the support of nested containers for the new Key-value table eosio::kv::map
 * For each action, an example regarding how to use the action with the cleos command line is given.
 *
 * std:pair<T1,T2> is a struct with 2 fields first and second,
 * std::map<K,V> is handled as an array/vector of pairs/structs by EOSIO with implicit fields key, value,
 * the cases of combined use of key/value and first/second involving map,pair in the cleos are documented here,
 * so handling of std::pair is NOT the same as the handling of a general struct such as struct mystruct!
 *
 * When assigning data input with cleos:
 *      [] represents an empty vector<T>/set<T> or empty map<T1,T2> where T, T1, T2 can be any composite types
 *      null represents an uninitialized std::optional<T> where T can be any composite type
 *      BUT [] or null can NOT be used to represent an empty struct or empty std::pair
 *
 * Attention:
 *   1) Please follow https://github.com/EOSIO/eos/tree/develop/contracts/enable-kv to enable key-value support of nodeos first
 *   2) To get to the point right away, here authentication/permission checking is skipped for each action in this contract,
 *      however in practice, suitable permission checking such as require_auth(user) has to be used!
 *
 * Expected printout:
 *      For each setx action, the printed result on the cleos console is given in its corresponding prntx action.
 */

#include <eosio/eosio.hpp>
#include <eosio/map.hpp>

#include <vector>
#include <set>
#include <optional>
#include <map>
#include <tuple>

using namespace eosio;
using namespace std;


#define SETCONTAINERVAL(x) do { \
        person2kv obj = mymap[id]; \
        obj.x = x; \
        mymap[id] =  obj; \
    }while(0)

//when the keys of kv::map are of different types, the following sets values of mymap2, mymap3, mymap4
#define SETCONTAINERVALN(num, x) do { \
        person2kv obj = mymap ## num[id]; \
        obj.x = x; \
        mymap ## num[id] =  obj; \
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

// use typedefs to make multi-layer nested containers work!
typedef vector<uint16_t> vec_uint16;
typedef set<uint16_t> set_uint16;
typedef optional<uint16_t> op_uint16;
typedef map<uint16_t, uint16_t> mp_uint16;
typedef pair<uint16_t, uint16_t> pr_unit16;

typedef vector<uint32_t> vec_uint32;
typedef vector<vec_uint32> vecvec_uint32;

//using dummy_kvmap = eosio::kv::map<"dumkv"_n, int, int>;

struct person2kv {
    //  Each container/object is represented by one letter: v-vector, m-map, s-mystruct,o-optional, p-pair,
    //  with exceptions:     s2 - mystruct2,    st - set

    vector<uint16_t> v;
    map<string,string> m;
    mystruct s;
    mystruct2 s2;
    set<uint16_t> st;
    optional<string> o;
    pair<uint16_t, uint16_t> p;
    vector<mystruct> vs;

    //The following are 2-layer nested containers involving vector/optional/map
    vector<vec_uint16> vv;
    optional<op_uint16> oo;
    map<uint16_t, mp_uint16> mm;
    set<set_uint16> stst;

    vector<set_uint16> vst;
    set<vec_uint16> stv;

    vector<op_uint16> vo;
    optional<vec_uint16> ov;
    set<op_uint16> sto;
    optional<set_uint16> ost;

    vector<mp_uint16> vm;
    map<uint16_t, vec_uint16> mv;

    set<mp_uint16> stm;
    map<uint16_t, set_uint16> mst;

    optional<mp_uint16> om;
    map<uint16_t, op_uint16> mo;

    //The following are composite types involving pair and containers
    vector<pair<uint32_t, uint32_t> > vp;
    pair<uint32_t, vec_uint16> pv;

    set<pair<uint32_t, uint32_t> > stp;
    pair<uint32_t, set_uint16> pst;

    optional<pr_unit16> op;
    pair<uint32_t, op_uint16> po;

    map<uint16_t, pr_unit16> mp;
    pair<uint16_t, mp_uint16> pm;

    pair<uint16_t, pr_unit16>  pp;

    //The following is a 3-layer nested containers for motivation
    optional<vecvec_uint32> ovv;
};


class [[eosio::contract("nestcontn2kv")]] nestcontn2kv : public eosio::contract {
    using my_map_t = eosio::kv::map<"people2kv"_n, int, person2kv>;

    //The following kv::map have different key types: std::string, eosio::name, mystructrefl
    using my_map_t2 = eosio::kv::map<"people2kv2"_n, string, person2kv>;
    using my_map_t3 = eosio::kv::map<"people2kv3"_n, name, person2kv>;
    using my_map_t4 = eosio::kv::map<"people2kv4"_n, mystructrefl, person2kv>;

    private:
        my_map_t mymap{};
        my_map_t2 mymap2{};
        my_map_t3 mymap3{};
        my_map_t4 mymap4{};

    public:
        using contract::contract;

        nestcontn2kv(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

        //[[eosio::action]] void settuple(name user, const tuple<uint16_t>& tp) {}
        //[[eosio::action]] void settuple(name user, const tuple<uint16_t, uint32_t, int16_t>& tp) {}
        //  eosio-cpp compile error for std::tuple: Tried to get a nested template type of a template not containing one

        //[[eosio::action]] void setdumkv(name user, const dummy_kvmap& tp) {}
        // eosio-cpp compile error for above dummy_kvmap: Tried to get a non-existent template argument

        //Example: cleos --verbose push action nestcontn2kv get '[1]' -p alice@active
        [[eosio::action]]
        person2kv get(int id)
        {
            //This action will be used by all prntx actions to retrieve item_found for given id
            auto iter = mymap.find(id);
            check(iter != mymap.end(), "Record does not exist");
            const auto& item_found = iter->second();
            return item_found;
        }

         /*Examples:
          * cleos --verbose push action nestcontn2kv setv '[1,[100,200,300,600]]' -p alice@active
          * cleos --verbose push action nestcontn2kv setv '[2, []]' -p bob@active
          */
        [[eosio::action]]
        void setv(int id, const vector<uint16_t>& v)
        {
            SETCONTAINERVAL(v);
            eosio::print("vector<uint16_t> stored successfully");
        }

         /*Examples:
         *  cleos --verbose push action nestcontn2kv prntv '[1]' -p alice@active
         *      output: >> size of stored v:4 vals:100,200,300,600,
         *
         *  cleos --verbose push action nestcontn2kv prntv '[2]' -p bob@active
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


        /*Examples:
         *  cleos --verbose push action nestcontn2kv setst '[1, [101,201,301]]' -p alice@active
         *  cleos --verbose push action nestcontn2kv setst '[2, []]' -p bob@active
         */
        [[eosio::action]]
        void setst(int id, const set<uint16_t> & st)
        {
            SETCONTAINERVAL(st);
            eosio::print("set<uint16_t> stored successfully");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv prntst '[1]' -p alice@active
         *      output: >> size of stored st:3 vals:101,201,301,
         *
         *  cleos --verbose push action nestcontn2kv prntst '[2]' -p bob@active
         *      output: >> size of stored st:0 vals:
         */
        [[eosio::action]]
        void prntst(int id)
        {
            const auto& psn = get(id);
            eosio::print("size of stored st:", psn.st.size()," vals:");
            for (const auto & ele : psn.st)
                eosio::print(ele, ",");

        }

        /*Examples:
         * To use shortcut notation:
         *   cleos --verbose push action nestcontn2kv setm '[1, [{"key":"str1","value":"str1val"}, {"key":"str3","value":"str3val"}]]' -p alice@active
         *
         * To use full JSON notation:
         *   cleos --verbose push action nestcontn2kv setm '{"id":2, "m":[{"key":"str4", "value":"str4val"}, {"key":"str6", "value":"str6val"}]}' -p jane@active
         *
         * To pass an empty map:
         *   cleos --verbose push action nestcontn2kv setm '[3, []]' -p bob@active
         */
        [[eosio::action]]
        void setm(int id, const map<string,string>  & m)
        {
            SETCONTAINERVAL(m);
            eosio::print("map<string,string> stored successfully");
        }

        /* Examples:
         *  cleos --verbose push action nestcontn2kv prntm '[1]' -p alice@active
         *      output: >> size of stored m:2 vals of m:str1:str1val  str3:str3val
         *
         *  cleos --verbose push action nestcontn2kv prntm '[2]' -p jane@active
         *      output: >> size of stored m:2 vals of m:str4:str4val  str6:str6val
         *
         *  cleos --verbose push action nestcontn2kv prntm '[3]' -p bob@active
         *      output: >> size of stored m:0 vals of m:
        */
        [[eosio::action]]
        void prntm(int id)
        {
            const auto& psn = get(id);
            eosio::print("size of stored m:", psn.m.size()," vals of m:");
            for (auto it2 = psn.m.begin(); it2 != psn.m.end(); ++it2)
                eosio::print(it2->first, ":", it2->second, "  ");

        }

        //Example: cleos --verbose push action nestcontn2kv sets '[1, {"_count":18, "_strID":"dumstr"}]' -p alice@active
        [[eosio::action]]
        void sets(int id, const mystruct& s)
        {
            SETCONTAINERVAL(s);
            eosio::print("mystruct stored successfully");
        }

        /*Example:
         * cleos --verbose push action nestcontn2kv  prnts '[1]' -p alice@active
         *      output: >> stored mystruct val:18,dumstr
         */
        [[eosio::action]]
        void prnts(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored mystruct val:", psn.s._count,",", psn.s._strID);
        }

        //Example: cleos --verbose push action nestcontn2kv sets2 '[1, {"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
        [[eosio::action]]
        void sets2(int id, const mystruct2& s2)
        {
            SETCONTAINERVAL(s2);
            eosio::print("complex mystruct2 stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prnts2 '[1]' -p alice@active
         *      output: >> stored mystruct2 val:18,dumstr,dumstr2
         */
        [[eosio::action]]
        void prnts2(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored mystruct2 val:", psn.s2._structfld._count,",", psn.s2._structfld._strID, ",", psn.s2._strID2);
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv setvs '[1, [{"_count":18, "_strID":"dumstr"},{"_count":19, "_strID":"dumstr2"}]]' -p alice@active
         */
        [[eosio::action]]
        void setvs(int id, const vector<mystruct>& vs)
        {
            SETCONTAINERVAL(vs);
            eosio::print("vector<mystruct> stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntvs '[1]' -p alice@active
         *      output: >> stored vector<mystruct>  size=2:
         *              >> 18,dumstr
         *              >> 19,dumstr2
         */
        [[eosio::action]]
        void prntvs(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<mystruct>  size=",psn.vs.size(), ":\n");
            for (const auto& ele: psn.vs)
            {
                eosio::print(ele._count,",", ele._strID,"\n");
            }
        }

        /*Examples
         *  To pass a null value:
         *      cleos --verbose push action nestcontn2kv seto '[1, null]' -p bob@active
         *
         *  To pass a non-null value:
         *      cleos --verbose push action nestcontn2kv seto '[2,"hello strval22"]' -p alice@active
         *
         */
        [[eosio::action]]
        void seto(int id, const optional<string>& o)
        {
            SETCONTAINERVAL(o);
            eosio::print("optional<string> stored successfully");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prnto '[1]' -p bob@active
         *      output: >> stored optional<string>  has null value!
         *
         *  cleos --verbose push action nestcontn2kv  prnto '[2]' -p alice@active
         *      output: >> stored optional<string> =hello strval22
         */
        [[eosio::action]]
        void prnto(int id)
        {
            const auto& psn = get(id);
            if (psn.o)
                eosio::print("stored optional<string> =", psn.o.value());
            else
                eosio::print("stored optional<string>  has null value!");
        }

        //Example: cleos --verbose push action nestcontn2kv setp '[1, {"first":183, "second":269}]' -p alice@active
        [[eosio::action]]
        void setp(int id, const pair<uint16_t, uint16_t>& p)
        {
            SETCONTAINERVAL(p);
            eosio::print("pair<uint16_t, uint16_t> stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntp '[1]' -p alice@active
         *      output: tored pair<uint16_t,uint16_t> val:183,269
         */
        [[eosio::action]]
        void prntp(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored pair<uint16_t,uint16_t> val:", psn.p.first,",", psn.p.second);
        }

        //============Two-layer nest containers start here===========
         //=== 1. Try set - set,vector,optional,map,pair

        //Example: cleos --verbose push action nestcontn2kv setstst '[1, [[10,20],[3], [400,500,600]]]' -p alice@active
        [[eosio::action]]
        void setstst(int id, const set<set_uint16>& stst)
        {
            SETCONTAINERVAL(stst);
            eosio::print("type defined set<set_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntstst '[1]' -p alice@active
         *      output: >> stored set<set<T>>:size=3 vals:
         *              >> 3
         *              >> 10 20
         *              >> 400 500 600
         */
        [[eosio::action]]
        void prntstst(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored set<set<T>>:size=", psn.stst.size(), " vals:\n");
            for (auto it1=psn.stst.begin(); it1!= psn.stst.end(); it1++)
            {
                for (auto it2=it1->begin(); it2!= it1->end(); it2++)
                    eosio::print(*it2, " ");
                eosio::print("\n");
            }
        }

         //Example: cleos --verbose push action nestcontn2kv setstv '[1, [[16,26],[36], [46,506,606]]]' -p alice@active
        [[eosio::action]]
        void setstv(int id, const set<vec_uint16>& stv)
        {
            SETCONTAINERVAL(stv);
            eosio::print("type defined set<vec_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntstv '[1]' -p alice@active
         *      output: >> stored set<vector<T>>:size=3 vals:
         *              >> 16 26
         *              >> 36
         *              >> 46 506 606
         */
        [[eosio::action]]
        void prntstv(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored set<vector<T>>:size=", psn.stv.size(), " vals:\n");
            for (auto it1=psn.stv.begin(); it1!= psn.stv.end(); it1++)
            {
                for (auto it2=it1->begin(); it2!= it1->end(); it2++)
                    eosio::print(*it2, " ");
                eosio::print("\n");
            }
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv setsto '[1, [101, null, 201, 301]]' -p alice@active
         *      *****user data can NOT be pushed into the chain, cleos get kv_table will not work if using setsto
         *  set<optional<T> > is  NOT supported currently!
         */
        [[eosio::action]]
        void setsto(int id, const set<op_uint16>& sto)
        {
            SETCONTAINERVAL(sto);
            eosio::print("type defined set<optional<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntsto '[1]' -p alice@active
         *      NOT supported
         */
        [[eosio::action]]
        void prntsto(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<op_uint16> vals:\n");
            for (const auto& ele : psn.sto)
            {
                if (ele)
                    eosio::print(ele.value(), " ");
                else
                    eosio::print("NULL", " ");
            }
        }

        /*Example:
         * cleos --verbose push action nestcontn2kv setstm '[1, [ [{"first":30,"second":300},{"first":31,"second":301}], [{"first":60,"second":600},{"first":61,"second":601}] ]]' -p alice@active
         */
        [[eosio::action]]
        void setstm(int id, const set<mp_uint16>& stm)
        {
            SETCONTAINERVAL(stm);
            eosio::print("type defined set<map<K, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntstm '[1]' -p alice@active
         *      output: >> stored set<mp_uint16>: size=2 content:
         *              >>
         *              >> Element 0--->
         *              >> 	30:300  31:301
         *              >> Element 1--->
         *              >> 	60:600  61:601
         */
        [[eosio::action]]
        void prntstm(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored set<mp_uint16>: size=", psn.stm.size()," content:\n");
            int count=0;
            for (const auto& mpval : psn.stm)
            {
                eosio::print("\nElement ", count++, "--->\n\t");
                for (auto it2 = mpval.begin(); it2 != mpval.end(); ++it2)
                    eosio::print(it2->first, ":", it2->second, "  ");
            }
        }


        /*Example:
         *  cleos --verbose push action nestcontn2kv setstp '[1, [{"first":68, "second":128}, {"first":69, "second":129}]]' -p alice@active
         */
        [[eosio::action]]
        void setstp(int id, const set<pair<uint32_t, uint32_t> >& stp)
        {
            SETCONTAINERVAL(stp);
            eosio::print("set<pair<T1,T2 >> stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntstp '[1]' -p alice@active
         *      output: >> stored set<pair<uint32_t, uint32_t> >  size=2:
         *              >> 68,128
         *              >> 69,129
         */
        [[eosio::action]]
        void prntstp(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored set<pair<uint32_t, uint32_t> >  size=", psn.stp.size(), ":\n");
            for (const auto& ele: psn.stp)
            {
                eosio::print(ele.first,",", ele.second,"\n");
            }
        }

        //=== 2. Try vector - set,vector,optional,map,pair

        //Example: cleos --verbose push action nestcontn2kv setvst '[1, [[10,20],[3], [400,500,600]]]' -p alice@active
        [[eosio::action]]
        void setvst(int id, const vector<set_uint16>& vst)
        {
            SETCONTAINERVAL(vst);
            eosio::print("type defined vector<set_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntvst '[1]' -p alice@active
         *      output: >> stored vector<set<T>>:size=3 vals:
         *              >> 10 20
         *              >> 3
         *              >> 400 500 600
         */
        [[eosio::action]]
        void prntvst(int id)
        {

            const auto& psn = get(id);
            eosio::print("stored vector<set<T>>:size=", psn.vst.size(), " vals:\n");
            for (auto it1=psn.vst.begin(); it1!= psn.vst.end(); it1++)
            {
                for (auto it2=it1->begin(); it2!= it1->end(); it2++)
                    eosio::print(*it2, " ");
                eosio::print("\n");
            }
        }


        //Example: cleos --verbose push action nestcontn2kv setvv '[1, [[1,2],[30], [40,50,60]]]' -p alice@active
        [[eosio::action]]
        void setvv(int id, const vector<vec_uint16>& vv)
        {
            SETCONTAINERVAL(vv);
            eosio::print("type defined vector<vec_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntvv '[1]' -p alice@active
         *      output: >> stored vector<vector<T>> vals:
         *              >> 1 2
         *              >> 30
         *              >> 40 50 60
         */
        [[eosio::action]]
        void prntvv(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<vector<T>> vals:\n");
            for (int i=0 ; i < psn.vv.size(); i++)
            {
                for (int j=0; j < psn.vv[i].size(); j++)
                    eosio::print(psn.vv[i][j], " ");
                eosio::print("\n"); //--- use cleos --verbose to show new lines !!!
            }
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv setvo '[1, [100, null, 200, null, 300]]' -p alice@active
         *      *****user data can NOT be pushed into the chain, cleos get kv_table will not work if using setvo
         *  vector<optional<T> > is  NOT supported currently!
         */
        [[eosio::action]]
        void setvo(int id, const vector<op_uint16>& vo)
        {
            SETCONTAINERVAL(vo);
            eosio::print("type defined vector<optional<T> > stored successfully!");
        }

        //Example: cleos --verbose push action nestcontn2kv  prntvo '[1]' -p alice@active
        //          NOT supported
        [[eosio::action]]
        void prntvo(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<op_uint16> vals:\n");
            for (const auto& ele : psn.vo)
            {
                if (ele)
                    eosio::print(ele.value(), " ");
                else
                    eosio::print("NULL", " ");
            }
        }


        /*Example:
         *  cleos --verbose push action nestcontn2kv setvm '[1, [ [{"first":10,"second":100},{"first":11,"second":101}], [{"first":80,"second":800},{"first":81,"second":9009}] ]]' -p alice@active
         */
        [[eosio::action]]
        void setvm(int id, const vector<mp_uint16>& vm)
        {
            SETCONTAINERVAL(vm);
            eosio::print("type defined vector<map<K, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntvm '[1]' -p alice@active
         *      output: >> stored vector<mp_uint16>: size=2 content:
         *              >>
         *              >> Element 0--->
         *              >> 	10:100  11:101
         *              >> Element 1--->
         *              >> 	80:800  81:9009
         */
        [[eosio::action]]
        void prntvm(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<mp_uint16>: size=", psn.vm.size()," content:\n");
            int count=0;
            for (const auto& mpval : psn.vm)
            {
                eosio::print("\nElement ", count++, "--->\n\t");
                for (auto it2 = mpval.begin(); it2 != mpval.end(); ++it2)
                    eosio::print(it2->first, ":", it2->second, "  ");
            }

        }


        /*Example:
         *  cleos --verbose push action nestcontn2kv setvp '[1, [{"first":18, "second":28}, {"first":19, "second":29}]]' -p alice@active
         */
        [[eosio::action]]
        void setvp(int id, const vector<pair<uint32_t, uint32_t> >& vp)
        {
            SETCONTAINERVAL(vp);
            eosio::print("vector<pair<uint32_t, uint32_t> > stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntvp '[1]' -p alice@active
         *      output: >> stored vector<pair<uint32_t, uint32_t> >  size=2:
         *              >> 18,28
         *              >> 19,29
         */
        [[eosio::action]]
        void prntvp(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<pair<uint32_t, uint32_t> >  size=", psn.vp.size(), ":\n");
            for (const auto& ele: psn.vp)
            {
                eosio::print(ele.first,",", ele.second,"\n");
            }
        }

        //=== 3. Try optional - set,vector,optional,map,pair


        /*Examples:
         *  cleos --verbose push action nestcontn2kv setost '[1, null]' -p bob@active
         *  cleos --verbose push action nestcontn2kv setost '[2, [1006,2006, 3006]]' -p alice@active
         */
        [[eosio::action]]
        void setost(int id, const optional<set_uint16>& ost)
        {
            SETCONTAINERVAL(ost);
            eosio::print("type defined optional<set<T> > stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prntost '[1]' -p bob@active
         *      output: >> stored optional<set_uint16>  vals:
         *              >> NULL or no value
         *
         *  cleos --verbose push action nestcontn2kv  prntost '[2]' -p alice@active
         *      output: >> stored optional<set_uint16>  vals:
         *              >> 1006 2006 3006
         */
        [[eosio::action]]
        void prntost(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored optional<set_uint16>  vals:\n");
            if (psn.ost)
            {
                for (const auto& ele : psn.ost.value())
                    eosio::print(ele, " ");
            }
            else
                eosio::print("NULL or no value");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv setov '[1, null]' -p bob@active
         *
         *  cleos --verbose push action nestcontn2kv setov '[2, [1001,2001, 3001]]' -p alice@active
         */
        [[eosio::action]]
        void setov(int id, const optional<vec_uint16>& ov)
        {
            SETCONTAINERVAL(ov);
            eosio::print("type defined optional<vector<T> > stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prntov '[1]' -p bob@active
         *      output: >> stored optional<vec_uint16>  vals:
         *              >> NULL or no value
         *
         *  cleos --verbose push action nestcontn2kv  prntov '[2]' -p alice@active
         *      output: >> stored optional<vec_uint16>  vals:
         *              >> 1001 2001 3001
         */
        [[eosio::action]]
        void prntov(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored optional<vec_uint16>  vals:\n");
            if (psn.ov)
            {
                for (const auto& ele : psn.ov.value())
                    eosio::print(ele, " ");
            }
            else
                eosio::print("NULL or no value");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv setoo '[1, null]' -p bob@active
         *
         *  cleos --verbose push action nestcontn2kv setoo '[2,123]' -p alice@active
         */
        [[eosio::action]]
        void setoo(int id, const optional<op_uint16>& oo)
        {
            SETCONTAINERVAL(oo);
            eosio::print("type defined optional<op_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prntoo '[1]' -p bob@active
         *      ouput: >> stored optional<optional<T>> val:null or no real value stored
         *
         *  cleos --verbose push action nestcontn2a  prntoo '[2]' -p alice@active
         *      output: >> stored optional<optional<T>> val:123
         */
        [[eosio::action]]
        void prntoo(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored optional<optional<T>> val:");
            if (psn.oo && psn.oo.value())
                eosio::print(psn.oo.value().value(),"\n");
            else
                eosio::print("null or no real value stored");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  setom '[1,[{"first":10,"second":1000},{"first":11,"second":1001}] ]' -p alice@active
         *
         *  cleos --verbose push action nestcontn2kv  setom '[2, null ]' -p bob@active
         */
        [[eosio::action]]
        void setom(int id, const optional<mp_uint16>& om)
        {
            SETCONTAINERVAL(om);
            eosio::print("type defined optional<map<K, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntom '[1]' -p alice@active
         *      output: >> size of stored om:2 vals:10:1000  11:1001
         *
         *  cleos --verbose push action nestcontn2kv  prntom '[2]' -p bob@active
         *      output: >> optional<mp_uint16> has NULL value
         */
        [[eosio::action]]
        void prntom(int id)
        {
            const auto& psn = get(id);
            if (psn.om)
            {
                auto mpval = psn.om.value();
                eosio::print("size of stored om:", mpval.size()," vals:");
                for (auto it2 = mpval.begin(); it2 != mpval.end(); ++it2)
                    eosio::print(it2->first, ":", it2->second, "  ");

            }
            else
                eosio::print("optional<mp_uint16> has NULL value\n");
        }


        /*Examples:
         *  cleos --verbose push action nestcontn2kv setop '[1, {"first":60, "second":61}]' -p alice@active
         *
         *  cleos --verbose push action nestcontn2kv setop '[2, null]' -p bob@active
         */
        [[eosio::action]]
        void setop(int id, const optional<pr_unit16> & op)
        {
            SETCONTAINERVAL(op);
            eosio::print("type-defined optional-pair optional<pr_unit16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prntop '[1]' -p alice@active
         *      output: >> stored optional<pr_unit16> data:60 61
         *
         *  cleos --verbose push action nestcontn2kv  prntop '[2]' -p bob@active
         *      output: >> optional<pr_unit16> has NULL value
         */
        [[eosio::action]]
        void prntop(int id)
        {
            const auto& psn = get(id);
            if (psn.op)
                eosio::print("stored optional<pr_unit16> data:", psn.op.value().first, " ", psn.op.value().second);
            else
                eosio::print("optional<pr_unit16> has NULL value\n");
        }

        //=== 4. Try map - set,vector,optional,map,pair

        /*Example:
         *  cleos --verbose push action nestcontn2kv setmst '[1, [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmst(int id, const map<uint16_t, set_uint16> & mst)
        {
            SETCONTAINERVAL(mst);
            eosio::print("type-defined map<K, set<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntmst '[1]' -p alice@active
         *      output: >> stored map<uint16_t, set_uint16>: size=2 content:
         *              >> 1:vals 10 11 12 16
         *              >> 2:vals 200 300
         */
        [[eosio::action]]
        void prntmst(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored map<uint16_t, set_uint16>: size=", psn.mst.size()," content:\n");
            for (auto it2 = psn.mst.begin(); it2 != psn.mst.end(); ++it2)
            {
                    eosio::print(it2->first, ":", "vals ");
                    for (const auto& ele: it2->second)
                    {
                        eosio::print(ele," ");
                    }
                    eosio::print("\n");
            }
        }



        /*Example:
         * cleos --verbose push action nestcontn2kv setmv '[1, [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmv(int id, const map<uint16_t, vec_uint16>& mv)
        {
            SETCONTAINERVAL(mv);
            eosio::print("type defined map<K, vector<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntmv '[1]' -p alice@active
         *      output: >> stored map<uint16_t, vec_uint16>: size=2 content:
         *              >> 1:vals 10 11 12 16
         *              >> 2:vals 200 300
         */
        [[eosio::action]]
        void prntmv(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored map<uint16_t, vec_uint16>: size=", psn.mv.size()," content:\n");
            for (auto it2 = psn.mv.begin(); it2 != psn.mv.end(); ++it2)
            {
                    eosio::print(it2->first, ":", "vals ");
                    for (const auto& ele: it2->second)
                    {
                        eosio::print(ele," ");
                    }
                    eosio::print("\n");
            }
        }


        /*Example:
         *  cleos --verbose push action nestcontn2kv setmo '[1, [{"key":10,"value":1000},{"key":11,"value":null}]]' -p alice@active
         */
        [[eosio::action]]
        void setmo(int id, const map<uint16_t, op_uint16>& mo)
        {
            SETCONTAINERVAL(mo);
            eosio::print("type defined map<K, optional<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntmo '[1]' -p alice@active
         *      output: >> size of stored mo:2 vals
         *              >> 10:1000 11:NULL
         */
        [[eosio::action]]
        void prntmo(int id)
        {
            const auto& psn = get(id);
            eosio::print("size of stored mo:", psn.mo.size()," vals\n");
            for (auto it2 = psn.mo.begin(); it2 != psn.mo.end(); ++it2)
            {
                eosio::print(it2->first, ":");
                if (it2->second)
                    eosio::print(it2->second.value()," ");
                else
                    eosio::print("NULL ");
            }
        }

        /*Example:
         *  cleos push action nestcontn2kv setmm '[1, [{"key":10,"value":[{"first":200,"second":2000}, {"first":201,"second":2001}] }, {"key":11,"value":[{"first":300,"second":3000}, {"first":301,"second":3001}] } ]]' -p alice@active
         *       Attention: please note the cleos input of mm or map<K1, map<K2, V> > is a combination of strings key/value and first/second !
         *       i.e the "value" part of mm is an array of <first,second> pairs!
         */
        [[eosio::action]]
        void setmm(int id, const map<uint16_t, mp_uint16>& mm)
        {
            SETCONTAINERVAL(mm);
            eosio::print("type defined map<K1,map<K2, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntmm '[1]' -p alice@active
         *      output: >> stored map<uint16_t, mp_uint16> val:size=2 vals are the following:
         *              >> 10--->
         *              >> 	200:2000
         *              >> 	201:2001
         *              >> 11--->
         *              >> 	300:3000
         *              >> 	301:3001
         */
        [[eosio::action]]
        void prntmm(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored map<uint16_t, mp_uint16> val:");
            eosio::print("size=", psn.mm.size()," vals are the following:\n");
            for (auto it2 = psn.mm.begin(); it2 != psn.mm.end(); ++it2)
            {
                eosio::print(it2->first,"--->\n");
                const auto& temp = it2->second;
                for (auto it3 = temp.begin(); it3 != temp.end(); ++it3)
                    eosio::print("\t",it3->first, ":", it3->second, "\n");
            }
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv setmp '[1, [{"key":36,"value":{"first":300, "second":301}}, {"key":37,"value":{"first":600, "second":601}} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmp(int id, const map<uint16_t, pr_unit16> & mp)
        {
            SETCONTAINERVAL(mp);
            eosio::print("type-defined map-pair map<uint16_t, pr_unit16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntmp '[1]' -p alice@active
         *      output: >> size of stored mp:2 vals:
         *              >> 36:300 301
         *              >> 37:600 601
         */
        [[eosio::action]]
        void prntmp(int id)
        {
            const auto& psn = get(id);
            eosio::print("size of stored mp:", psn.mp.size()," vals:\n");
            for (auto it2 = psn.mp.begin(); it2 != psn.mp.end(); ++it2)
                eosio::print(it2->first, ":", it2->second.first, " ", it2->second.second, "\n");
        }


        //=== 5. Try pair - set,vector,optional,map,pair

        /*Example:
         *  cleos --verbose push action nestcontn2kv setpst '[1, {"first":20, "second":[200,201,202]}]' -p alice@active
         */
        [[eosio::action]]
        void setpst(int id, const pair<uint32_t, set_uint16>& pst)
        {
            SETCONTAINERVAL(pst);
            eosio::print("type-defined pair-set pair<uint32_t, set_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntpst '[1]' -p alice@active
         *      output: >> content of stored pair<uint32_t, set_uint16>: first=20
         *              >> second=200 201 202
         */
        [[eosio::action]]
        void prntpst(int id)
        {
            const auto& psn = get(id);
            eosio::print("content of stored pair<uint32_t, set_uint16>: first=", psn.pst.first, "\nsecond=");
            for (const auto& ele: psn.pst.second)
                eosio::print(ele, " ");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv setpv '[1, {"first":10, "second":[100,101,102]}]' -p alice@active
         */
        [[eosio::action]]
        void setpv(int id, const pair<uint32_t, vec_uint16>& pv)
        {
            SETCONTAINERVAL(pv);
            eosio::print("type-defined pair-vector pair<uint32_t, vec_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntpv '[1]' -p alice@active
         *      output: >> content of stored pair<uint32_t, vec_uint16>: first=10
         *              >> second=100 101 102
         */
        [[eosio::action]]
        void prntpv(int id)
        {
            const auto& psn = get(id);
            eosio::print("content of stored pair<uint32_t, vec_uint16>: first=", psn.pv.first, "\nsecond=");
            for (const auto& ele: psn.pv.second)
                eosio::print(ele, " ");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv setpo '[1, {"first":70, "second":71}]' -p alice@active
         *
         *  cleos --verbose push action nestcontn2kv setpo '[2, {"first":70, "second":null}]' -p bob@active
         */
        [[eosio::action]]
        void setpo(int id, const pair<uint32_t, op_uint16> & po)
        {
            SETCONTAINERVAL(po);
            eosio::print("type-defined pair-optional pair<uint32_t, op_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prntpo '[1]' -p alice@active
         *      output: >> content of stored pair<uint16_t, op_unit16>: first=70
         *              >> second=71
         *
         *  cleos --verbose push action nestcontn2a  prntpo '[2]' -p bob@active
         *      output: >> content of stored pair<uint16_t, op_unit16>: first=70
         *              >> second=NULL
         */
        [[eosio::action]]
        void prntpo(int id)
        {
            const auto& psn = get(id);
            eosio::print("content of stored pair<uint16_t, op_unit16>: first=", psn.po.first, "\nsecond=");
            if (psn.po.second)
                eosio::print(psn.po.second.value(), " ");
            else
                eosio::print("NULL ");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv setpm '[1, {"key":6, "value":[{"first":20,"second":300}, {"first":21,"second":301}] }]' -p alice@active
         *      Remark: the data input for pm uses a combination of key/vale and first/second
         */
        [[eosio::action]]
        void setpm(int id, const pair<uint16_t, mp_uint16> & pm)
        {
            SETCONTAINERVAL(pm);
            eosio::print("type-defined pair-map pair<uint16_t, mp_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2kv  prntpm '[1]' -p alice@active
         *      output: >> content of stored pair<uint16_t, mp_uint16>: first=6
         *              >> second=20:300  21:301
         */
        [[eosio::action]]
        void prntpm(int id)
        {
            const auto& psn = get(id);
            eosio::print("content of stored pair<uint16_t, mp_uint16>: first=", psn.pm.first, "\nsecond=");
            for (auto it2 = psn.pm.second.begin(); it2 != psn.pm.second.end(); ++it2)
                eosio::print(it2->first, ":", it2->second, "  ");
        }


        /*Example:
         *  cleos --verbose push action nestcontn2kv setpp '[1, {"key":30, "value":{"first":301, "second":302} }]' -p alice@active
         *      Remark: input data for pp or pair-pair is a combination of key/value and first/second
         */
        [[eosio::action]]
        void setpp(int id, const pair<uint16_t, pr_unit16> & pp)
        {
            SETCONTAINERVAL(pp);
            eosio::print("type-defined pair-pair pair<uint16_t, pr_unit16> stored successfully!");
        }

        /*Example:
         * cleos --verbose push action nestcontn2kv  prntpp '[1]' -p alice@active
         *      output: >> content of stored pair<uint16_t, pr_unit16>: first=30
         *              >> second=301 302
         */
        [[eosio::action]]
        void prntpp(int id)
        {
            const auto& psn = get(id);
            eosio::print("content of stored pair<uint16_t, pr_unit16>: first=", psn.pp.first, "\nsecond=");
            eosio::print(psn.pp.second.first, " ", psn.pp.second.second);
        }


        //Try an example of 3-layer nested container

        /*Example:
         *  cleos --verbose push action nestcontn2kv setovv '[1, [[21,22],[230], [240,250,260,280]]]' -p alice@active
         */
        [[eosio::action]]
        void setovv(int id, const optional<vecvec_uint32>& ovv)
        {
            // try type-defined 3-layer nested container optional<vector<vector<T> > > here
            SETCONTAINERVAL(ovv);
            eosio::print("type-defined optional<vecvec_uint32> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2kv  prntovv '[1]' -p alice@active
         *      output: >> stored optional<vector<vector<T> > > vals:
         *              >> 21 22
         *              >> 230
         *              >> 240 250 260 280
         */
        [[eosio::action]]
        void prntovv(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored optional<vector<vector<T> > > vals:\n");
            if (psn.ovv)
            {
              auto v2 = psn.ovv.value();
              for (int i=0 ; i < v2.size(); i++)
              {
                for (int j=0; j < v2[i].size(); j++)
                    eosio::print(v2[i][j], " ");
                eosio::print("\n");
              }
            }
            else
                eosio::print("stored optional<vector<vector<T> > >  has null value!");
        }

        //Example: cleos --verbose push action nestcontn2kv erase '["bob", 2]' -p bob@active
        [[eosio::action]]
        void erase(name user, int id)
        {
            require_auth(user);
            eosio::print (user, " is requesting to erase the record with key=", id, "\n");
            auto iter = mymap.find(id);
            check(iter != mymap.end(), "cannot erase: Record with given id does not exist");
            mymap.erase(id);
            eosio::print("record with id=", id, " is successfully erased!");
        }

        //Example: cleos --verbose push action nestcontn2kv prntallkeys '["bob"]' -p bob@active
        [[eosio::action]]
        void prntallkeys(name user)
        {
            //print all keys stored in this block chain for this contract
            require_auth(user);
            eosio::print (user, " is requesting prntallkeys\n");
            if (mymap.empty())
                eosio::print ("mymap is empty, no key in mymap!");

            eosio::print("mymap keys include:\n");
            for ( const auto& item : mymap )
            {
                eosio::print(item.first(), "\n");
            }
        }

        //Example: cleos --verbose push action nestcontn2kv get2 '["str1"]' -p alice@active
        [[eosio::action]]
        person2kv get2(string id)
        {
            auto iter = mymap2.find(id);
            check(iter != mymap2.end(), "Record does not exist");
            const auto& item_found = iter->second();
            return item_found;
        }

        /*Examples:
        * cleos --verbose push action nestcontn2kv setv2 '["str1",[102,202,302,602]]' -p alice@active
        * cleos --verbose push action nestcontn2kv setv2 '["str2", []]' -p bob@active
        */
        [[eosio::action]]
        void setv2(string id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN(2, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
        *  cleos --verbose push action nestcontn2kv prntv2 '["str1"]' -p alice@active
        *      output: >> size of stored v:4 vals:102,202,302,602,
        *
        *  cleos --verbose push action nestcontn2kv prntv2 '["str2"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv2(string id)
        {
            const auto& psn = get2(id);
            eosio::print("size of stored v:", psn.v.size()," vals:");
            for (const auto & ele : psn.v)
                eosio::print(ele, ",");
        }


        //Example: cleos --verbose push action nestcontn2kv get3 '["name1"]' -p alice@active
        [[eosio::action]]
        person2kv get3(name id)
        {
            auto iter = mymap3.find(id);
            check(iter != mymap3.end(), "Record does not exist");
            const auto& item_found = iter->second();
            return item_found;
        }

        /*Examples:
        * cleos --verbose push action nestcontn2kv setv3 '["name1",[103,203,303,603]]' -p alice@active
        * cleos --verbose push action nestcontn2kv setv3 '["name2", []]' -p bob@active
        */
        [[eosio::action]]
        void setv3(name id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN(3, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
        *  cleos --verbose push action nestcontn2kv prntv3 '["name1"]' -p alice@active
        *      output: >> size of stored v:4 vals:103,203,303,603,
        *
        *  cleos --verbose push action nestcontn2kv prntv3 '["name2"]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv3(name id)
        {
            const auto& psn = get3(id);
            eosio::print("size of stored v:", psn.v.size()," vals:");
            for (const auto & ele : psn.v)
                eosio::print(ele, ",");
        }


        //Example: cleos --verbose push action nestcontn2kv get4 '[{"_count":18, "_strID":"dumstr"}]' -p alice@active
        [[eosio::action]]
        person2kv get4(mystructrefl id)
        {
            auto iter = mymap4.find(id);
            check(iter != mymap4.end(), "Record does not exist");
            const auto& item_found = iter->second();
            return item_found;
        }

        /*Examples:
        * cleos --verbose push action nestcontn2kv setv4 '[{"_count":18, "_strID":"dumstr"},[104,204,304,604]]' -p alice@active
        * cleos --verbose push action nestcontn2kv setv4 '[{"_count":11, "_strID":"dumstr1"}, []]' -p bob@active
        */
        [[eosio::action]]
        void setv4(mystructrefl id, const vector<uint16_t>& v)
        {
            SETCONTAINERVALN(4, v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
        *  cleos --verbose push action nestcontn2kv prntv4 '[{"_count":18, "_strID":"dumstr"}]' -p alice@active
        *      output: >> size of stored v:4 vals:104,204,304,604,
        *
        *  cleos --verbose push action nestcontn2kv prntv4 '[{"_count":11, "_strID":"dumstr1"}]' -p bob@active
        *      output: >> size of stored v:0 vals:
        */
        [[eosio::action]]
        void prntv4(mystructrefl id)
        {
            const auto& psn = get4(id);
            eosio::print("size of stored v:", psn.v.size()," vals:");
            for (const auto & ele : psn.v)
                eosio::print(ele, ",");
        }

        //The following are needed for eosio-cpp compiler to identify the actions in genearted .abi
        using get_action = eosio::action_wrapper<"get"_n, &nestcontn2kv::get>;
        using erase_action = eosio::action_wrapper<"erase"_n, &nestcontn2kv::erase>;
        using prntallkeys_action = eosio::action_wrapper<"prntallkeys"_n, &nestcontn2kv::prntallkeys>;

        using setv_action = eosio::action_wrapper<"setv"_n, &nestcontn2kv::setv>;
        using prntv_action = eosio::action_wrapper<"prntv"_n, &nestcontn2kv::prntv>;
        using setst_action = eosio::action_wrapper<"setst"_n, &nestcontn2kv::setst>;
        using prntst_action = eosio::action_wrapper<"prntst"_n, &nestcontn2kv::prntst>;
        using setm_action = eosio::action_wrapper<"setm"_n, &nestcontn2kv::setm>;
        using prntm_action = eosio::action_wrapper<"prntm"_n, &nestcontn2kv::prntm>;
        using sets_action = eosio::action_wrapper<"sets"_n, &nestcontn2kv::sets>;
        using prnts_action = eosio::action_wrapper<"prnts"_n, &nestcontn2kv::prnts>;
        using sets2_action = eosio::action_wrapper<"sets2"_n, &nestcontn2kv::sets2>;
        using prnts2_action = eosio::action_wrapper<"prnts2"_n, &nestcontn2kv::prnts2>;
        using setvs_action = eosio::action_wrapper<"setvs"_n, &nestcontn2kv::setvs>;
        using prntvs_action = eosio::action_wrapper<"prntvs"_n, &nestcontn2kv::prntvs>;
        using seto_action = eosio::action_wrapper<"seto"_n, &nestcontn2kv::seto>;
        using prnto_action = eosio::action_wrapper<"prnto"_n, &nestcontn2kv::prnto>;
        using setp_action = eosio::action_wrapper<"setp"_n, &nestcontn2kv::setp>;
        using prntp_action = eosio::action_wrapper<"prntp"_n, &nestcontn2kv::prntp>;

        using setstst_action = eosio::action_wrapper<"setstst"_n, &nestcontn2kv::setstst>;
        using prntstst_action = eosio::action_wrapper<"prntstst"_n, &nestcontn2kv::prntstst>;
        using setstv_action = eosio::action_wrapper<"setstv"_n, &nestcontn2kv::setstv>;
        using prntstv_action = eosio::action_wrapper<"prntstv"_n, &nestcontn2kv::prntstv>;
        using setsto_action = eosio::action_wrapper<"setsto"_n, &nestcontn2kv::setsto>;
        using prntsto_action = eosio::action_wrapper<"prntsto"_n, &nestcontn2kv::prntsto>;
        using setstm_action = eosio::action_wrapper<"setstm"_n, &nestcontn2kv::setstm>;
        using prntstm_action = eosio::action_wrapper<"prntstm"_n, &nestcontn2kv::prntstm>;
        using setstp_action = eosio::action_wrapper<"setstp"_n, &nestcontn2kv::setstp>;
        using prntstp_action = eosio::action_wrapper<"prntstp"_n, &nestcontn2kv::prntstp>;

        using setvst_action = eosio::action_wrapper<"setvst"_n, &nestcontn2kv::setvst>;
        using prntvst_action = eosio::action_wrapper<"prntvst"_n, &nestcontn2kv::prntvst>;
        using setvv_action = eosio::action_wrapper<"setvv"_n, &nestcontn2kv::setvv>;
        using prntvv_action = eosio::action_wrapper<"prntvv"_n, &nestcontn2kv::prntvv>;
        using setvo_action = eosio::action_wrapper<"setvo"_n, &nestcontn2kv::setvo>;
        using prntvo_action = eosio::action_wrapper<"prntvo"_n, &nestcontn2kv::prntvo>;
        using setvm_action = eosio::action_wrapper<"setvm"_n, &nestcontn2kv::setvm>;
        using prntvm_action = eosio::action_wrapper<"prntvm"_n, &nestcontn2kv::prntvm>;
        using setvp_action = eosio::action_wrapper<"setvp"_n, &nestcontn2kv::setvp>;
        using prntvp_action = eosio::action_wrapper<"prntvp"_n, &nestcontn2kv::prntvp>;

        using setost_action = eosio::action_wrapper<"setost"_n, &nestcontn2kv::setost>;
        using prntost_action = eosio::action_wrapper<"prntost"_n, &nestcontn2kv::prntost>;
        using setov_action = eosio::action_wrapper<"setov"_n, &nestcontn2kv::setov>;
        using prntov_action = eosio::action_wrapper<"prntov"_n, &nestcontn2kv::prntov>;
        using setoo_action = eosio::action_wrapper<"setoo"_n, &nestcontn2kv::setoo>;
        using prntoo_action = eosio::action_wrapper<"prntoo"_n, &nestcontn2kv::prntoo>;
        using setom_action = eosio::action_wrapper<"setom"_n, &nestcontn2kv::setom>;
        using prntom_action = eosio::action_wrapper<"prntom"_n, &nestcontn2kv::prntom>;
        using setop_action = eosio::action_wrapper<"setop"_n, &nestcontn2kv::setop>;
        using prntop_action = eosio::action_wrapper<"prntop"_n, &nestcontn2kv::prntop>;

        using setmst_action = eosio::action_wrapper<"setmst"_n, &nestcontn2kv::setmst>;
        using prntmst_action = eosio::action_wrapper<"prntmst"_n, &nestcontn2kv::prntmst>;
        using setmv_action = eosio::action_wrapper<"setmv"_n, &nestcontn2kv::setmv>;
        using prntmv_action = eosio::action_wrapper<"prntmv"_n, &nestcontn2kv::prntmv>;
        using setmo_action = eosio::action_wrapper<"setmo"_n, &nestcontn2kv::setmo>;
        using prntmo_action = eosio::action_wrapper<"prntmo"_n, &nestcontn2kv::prntmo>;
        using setmm_action = eosio::action_wrapper<"setmm"_n, &nestcontn2kv::setmm>;
        using prntmm_action = eosio::action_wrapper<"prntmm"_n, &nestcontn2kv::prntmm>;
        using setmp_action = eosio::action_wrapper<"setmp"_n, &nestcontn2kv::setmp>;
        using prntmp_action = eosio::action_wrapper<"prntmp"_n, &nestcontn2kv::prntmp>;

        using setpst_action = eosio::action_wrapper<"setpst"_n, &nestcontn2kv::setpst>;
        using prntpst_action = eosio::action_wrapper<"prntpst"_n, &nestcontn2kv::prntpst>;
        using setpv_action = eosio::action_wrapper<"setpv"_n, &nestcontn2kv::setpv>;
        using prntpv_action = eosio::action_wrapper<"prntpv"_n, &nestcontn2kv::prntpv>;
        using setpo_action = eosio::action_wrapper<"setpo"_n, &nestcontn2kv::setpo>;
        using prntpo_action = eosio::action_wrapper<"prntpo"_n, &nestcontn2kv::prntpo>;
        using setpm_action = eosio::action_wrapper<"setpm"_n, &nestcontn2kv::setpm>;
        using prntpm_action = eosio::action_wrapper<"prntpm"_n, &nestcontn2kv::prntpm>;
        using setpp_action = eosio::action_wrapper<"setpp"_n, &nestcontn2kv::setpp>;
        using prntpp_action = eosio::action_wrapper<"prntpp"_n, &nestcontn2kv::prntpp>;

        using setovv_action = eosio::action_wrapper<"setovv"_n, &nestcontn2kv::setovv>;
        using prntovv_action = eosio::action_wrapper<"prntovv"_n, &nestcontn2kv::prntovv>;

        using get2_action = eosio::action_wrapper<"get2"_n, &nestcontn2kv::get2>;
        using setv2_action = eosio::action_wrapper<"setv2"_n, &nestcontn2kv::setv2>;
        using prntv2_action = eosio::action_wrapper<"prntv2"_n, &nestcontn2kv::prntv2>;

        using get3_action = eosio::action_wrapper<"get3"_n, &nestcontn2kv::get3>;
        using setv3_action = eosio::action_wrapper<"setv3"_n, &nestcontn2kv::setv3>;
        using prntv3_action = eosio::action_wrapper<"prntv3"_n, &nestcontn2kv::prntv3>;

        using get4_action = eosio::action_wrapper<"get4"_n, &nestcontn2kv::get4>;
        using setv4_action = eosio::action_wrapper<"setv4"_n, &nestcontn2kv::setv4>;
        using prntv4_action = eosio::action_wrapper<"prntv4"_n, &nestcontn2kv::prntv4>;
};

