/* Verify the support of nested containers in eosio multi-index table
 * For each action, an example regarding how to use the action with the cleos command line is given.
 *
 * std:pair<T1,T2> is a struct with 2 fields first and second,
 * std::map<K,V> is handled as an array/vector of pairs/structs by EOSIO with implicit fields key, value,
 * the cases of combined use of key/value and first/second involving map,pair in the cleos are documented here.
 * so handling of std::pair is NOT the same as the handling of a general struct such as struct mystruct!
 *
 * When assigning data input with cleos:
 *      [] represents an empty vector<T>/set<T> or empty map<T1,T2> where T, T1, T2 can be any composite types
 *      null represents an uninitialized std::optional<T> where T can be any composite type
 *      BUT [] or null can NOT be used to represent an empty struct or empty std::pair
 *
 * Expected printout:
 *      For each setx action, the printed result on the cleos console is given in its corresponding prntx action.
 */

#include <eosio/eosio.hpp>

#include <vector>
#include <set>
#include <optional>
#include <map>
#include <tuple>

using namespace eosio;
using namespace std;

#define  SETCONTAINERVAL(x) do { \
        require_auth(user); \
        psninfoindex2 tblIndex(get_self(), get_first_receiver().value); \
        auto iter = tblIndex.find(user.value); \
        if (iter == tblIndex.end()) \
        { \
            tblIndex.emplace(user, [&](auto &row) { \
                        row.key = user; \
                        row.x = x; \
                    }); \
        } \
        else \
        { \
            tblIndex.modify(iter, user, [&]( auto& row ) { \
                        row.x = x; \
                    }); \
        } \
    }while(0)

#define PRNTCHECK() require_auth(user); \
        psninfoindex2 tblIndex(get_self(), get_first_receiver().value); \
        auto iter = tblIndex.find(user.value); \
        check(iter != tblIndex.end(), "Record does not exist");

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

// use typedefs to make multi-layer nested containers work!
typedef vector<uint16_t> vec_uint16;
typedef set<uint16_t> set_uint16;
typedef optional<uint16_t> op_uint16;
typedef map<uint16_t, uint16_t> mp_uint16;
typedef pair<uint16_t, uint16_t> pr_unit16;

typedef vector<uint32_t> vec_uint32;
typedef vector<vec_uint32> vecvec_uint32;


class [[eosio::contract("nestcontn2a")]] nestcontn2a : public eosio::contract {
    private:
        struct [[eosio::table]] person2 {
            name key;

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

            uint64_t primary_key() const { return key.value; }
        };
        using psninfoindex2 = eosio::multi_index<"people2"_n, person2>;


    public:
        using contract::contract;

        nestcontn2a(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}


        //[[eosio::action]] void settuple(name user, const tuple<uint16_t>& tp) {}
        //  eosio-cpp compile error for std::tuple: Tried to get a nested template type of a template not containing one

         /*Examples:
          * cleos --verbose push action nestcontn2a setv '["alice", [100,200,300,600]]' -p alice@active
          * cleos --verbose push action nestcontn2a setv '["bob", []]' -p bob@active
          */
        [[eosio::action]]
        void setv(name user, const vector<uint16_t>& v)
        {
            SETCONTAINERVAL(v);
            eosio::print("vector<uint16_t> stored successfully");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a prntv '["alice"]' -p alice@active
         *      output: >> size of stored v:4 vals of v:100,200,300,600,
         *
         *  cleos --verbose push action nestcontn2a prntv '["bob"]' -p bob@active
         *      output: >> size of stored v:0 vals of v:
         */
        [[eosio::action]]
        void prntv(name user)
        {
            PRNTCHECK();
            eosio::print("size of stored v:", iter->v.size()," vals of v:");
            for (const auto & ele : iter->v)
                eosio::print(ele, ",");

        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a setst '["alice", [101,201,301]]' -p alice@active
         *  cleos --verbose push action nestcontn2a setst '["bob", []]' -p bob@active
         */
        [[eosio::action]]
        void setst(name user, const set<uint16_t> & st)
        {
            SETCONTAINERVAL(st);
            eosio::print("set<uint16_t> stored successfully");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a prntst '["alice"]' -p alice@active
         *      output: >> size of stored st:3 vals:101,201,301,
         *
         *  cleos --verbose push action nestcontn2a prntst '["bob"]' -p bob@active
         *      output: >> size of stored st:0 vals:
         */
        [[eosio::action]]
        void prntst(name user)
        {
            PRNTCHECK();
            eosio::print("size of stored st:", iter->st.size()," vals:");
            for (const auto & ele : iter->st)
                eosio::print(ele, ",");

        }

        /*Examples:
         * To use shortcut notation:
         *   cleos --verbose push action nestcontn2a setm '["alice", [{"key":"str1","value":"str1val"}, {"key":"str3","value":"str3val"}]]' -p alice@active
         *
         * To use full JSON notation:
         *   cleos --verbose push action nestcontn2a setm '{"user":"jane", "m":[{"key":"str4", "value":"str4val"}, {"key":"str6", "value":"str6val"}]}' -p jane@active
         *
         * To pass an empty map:
         *   cleos --verbose push action nestcontn2a setm '["bob", []]' -p bob@active
         */
        [[eosio::action]]
        void setm(name user, const map<string,string>  & m)
        {
            SETCONTAINERVAL(m);
            eosio::print("map<string,string> stored successfully");
        }

        /* Examples:
         *  cleos --verbose push action nestcontn2a prntm '["alice"]' -p alice@active
         *      output: >> size of stored m:2 vals of m:str1:str1val  str3:str3val
         *
         *  cleos --verbose push action nestcontn2a prntm '["jane"]' -p jane@active
         *      output: >> size of stored m:2 vals of m:str4:str4val  str6:str6val
         *
         *  cleos --verbose push action nestcontn2a prntm '["bob"]' -p bob@active
         *      output: >> size of stored m:0 vals of m:
        */
        [[eosio::action]]
        void prntm(name user)
        {
            PRNTCHECK();
            eosio::print("size of stored m:", iter->m.size()," vals of m:");
            for (auto it2 = iter->m.begin(); it2 != iter->m.end(); ++it2)
                eosio::print(it2->first, ":", it2->second, "  ");

        }

        //Example: cleos --verbose push action nestcontn2a sets '["alice", {"_count":18, "_strID":"dumstr"}]' -p alice@active
        [[eosio::action]]
        void sets(name user, const mystruct& s)
        {
            SETCONTAINERVAL(s);
            eosio::print("mystruct stored successfully");
        }

        /*Example:
         * cleos --verbose push action nestcontn2a  prnts '["alice"]' -p alice@active
         *      output: >> stored mystruct val:18,dumstr
         */
        [[eosio::action]]
        void prnts(name user)
        {
            PRNTCHECK();
            eosio::print("stored mystruct val:", iter->s._count,",", iter->s._strID);
        }

        //Example: cleos --verbose push action nestcontn2a sets2 '["alice", {"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
        [[eosio::action]]
        void sets2(name user, const mystruct2& s2)
        {
            SETCONTAINERVAL(s2);
            eosio::print("complex mystruct2 stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prnts2 '["alice"]' -p alice@active
         *      output: >> stored mystruct2 val:18,dumstr,dumstr2
         */
        [[eosio::action]]
        void prnts2(name user)
        {
            PRNTCHECK();
            eosio::print("stored mystruct2 val:", iter->s2._structfld._count,",", iter->s2._structfld._strID, ",", iter->s2._strID2);
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a setvs '["alice", [{"_count":18, "_strID":"dumstr"},{"_count":19, "_strID":"dumstr2"}]]' -p alice@active
         */
        [[eosio::action]]
        void setvs(name user, const vector<mystruct>& vs)
        {
            SETCONTAINERVAL(vs);
            eosio::print("vector<mystruct> stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntvs '["alice"]' -p alice@active
         *      output: >> stored vector<mystruct>  size=2:
         *              >> 18,dumstr
         *              >> 19,dumstr2
         */
        [[eosio::action]]
        void prntvs(name user)
        {
            PRNTCHECK();
            eosio::print("stored vector<mystruct>  size=",iter->vs.size(), ":\n");
            for (const auto& ele: iter->vs)
            {
                eosio::print(ele._count,",", ele._strID,"\n");
            }
        }

        /*Examples
         *  To pass a null value:
         *      cleos --verbose push action nestcontn2a seto '["bob", null]' -p bob@active
         *
         *  To pass a non-null value:
         *      cleos --verbose push action nestcontn2a seto '["alice","hello strval22"]' -p alice@active
         *
         */
        [[eosio::action]]
        void seto(name user, const optional<string>& o)
        {
            SETCONTAINERVAL(o);
            eosio::print("optional<string> stored successfully");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prnto '["bob"]' -p bob@active
         *      output: >> stored optional<string>  has null value!
         *
         *  cleos --verbose push action nestcontn2a  prnto '["alice"]' -p alice@active
         *      output: >> stored optional<string> =hello strval22
         */
        [[eosio::action]]
        void prnto(name user)
        {
            PRNTCHECK();
            if (iter->o)
                eosio::print("stored optional<string> =", iter->o.value());
            else
                eosio::print("stored optional<string>  has null value!");
        }

        //Example: cleos --verbose push action nestcontn2a setp '["alice", {"first":183, "second":269}]' -p alice@active
        [[eosio::action]]
        void setp(name user, const pair<uint16_t, uint16_t>& p)
        {
            SETCONTAINERVAL(p);
            eosio::print("pair<uint16_t, uint16_t> stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntp '["alice"]' -p alice@active
         *      output: tored pair<uint16_t,uint16_t> val:183,269
         */
        [[eosio::action]]
        void prntp(name user)
        {
            PRNTCHECK();
            eosio::print("stored pair<uint16_t,uint16_t> val:", iter->p.first,",", iter->p.second);
        }

        //============Two-layer nest containers start here===========
        //=== 1. Try vector - set,vector,optional,map,pair

        //Example: cleos --verbose push action nestcontn2a setvst '["alice", [[10,20],[3], [400,500,600]]]' -p alice@active
        [[eosio::action]]
        void setvst(name user, const vector<set_uint16>& vst)
        {
            SETCONTAINERVAL(vst);
            eosio::print("type defined vector<set_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntvst '["alice"]' -p alice@active
         *      output: >> stored vector<set<T>>:size=3 vals:
         *              >> 10 20
         *              >> 3
         *              >> 400 500 600
         */
        [[eosio::action]]
        void prntvst(name user)
        {

            PRNTCHECK();
            eosio::print("stored vector<set<T>>:size=", iter->vst.size(), " vals:\n");
            for (auto it1=iter->vst.begin(); it1!= iter->vst.end(); it1++)
            {
                for (auto it2=it1->begin(); it2!= it1->end(); it2++)
                    eosio::print(*it2, " ");
                eosio::print("\n");
            }
        }


        //Example: cleos --verbose push action nestcontn2a setvv '["alice", [[1,2],[30], [40,50,60]]]' -p alice@active
        [[eosio::action]]
        void setvv(name user, const vector<vec_uint16>& vv)
        {
            SETCONTAINERVAL(vv);
            eosio::print("type defined vector<vec_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntvv '["alice"]' -p alice@active
         *      output: >> stored vector<vector<T>> vals:
         *              >> 1 2
         *              >> 30
         *              >> 40 50 60
         */
        [[eosio::action]]
        void prntvv(name user)
        {

            PRNTCHECK();
            eosio::print("stored vector<vector<T>> vals:\n");
            for (int i=0 ; i < iter->vv.size(); i++)
            {
                for (int j=0; j < iter->vv[i].size(); j++)
                    eosio::print(iter->vv[i][j], " ");
                eosio::print("\n"); //--- use cleos --verbose to show new lines !!!
            }
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a setvo '["alice", [100, null, 200, null, 300]]' -p alice@active
         *        ******user data can NOT be pushed into the chain, cleos get table will not work if using setvo
         *  vector<optional<T> > is  NOT supported currently!
         */
        [[eosio::action]]
        void setvo(name user, const vector<op_uint16>& vo)
        {
            SETCONTAINERVAL(vo);
            eosio::print("type defined vector<optional<T> > stored successfully!");
        }

        //Example: cleos --verbose push action nestcontn2a  prntvo '["alice"]' -p alice@active
        //          NOT supported
        [[eosio::action]]
        void prntvo(name user)
        {
            PRNTCHECK();
            eosio::print("stored vector<op_uint16> vals:\n");
            for (const auto& ele : iter->vo)
            {
                if (ele)
                    eosio::print(ele.value(), " ");
                else
                    eosio::print("NULL", " ");
            }
        }


        /*Example:
         *  cleos --verbose push action nestcontn2a setvm '["alice", [ [{"first":10,"second":100},{"first":11,"second":101}], [{"first":80,"second":800},{"first":81,"second":9009}] ]]' -p alice@active
         */
        [[eosio::action]]
        void setvm(name user, const vector<mp_uint16>& vm)
        {
            SETCONTAINERVAL(vm);
            eosio::print("type defined vector<map<K, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntvm '["alice"]' -p alice@active
         *      output: >> stored vector<mp_uint16>: size=2 content:
         *              >>
         *              >> Element 0--->
         *              >> 	10:100  11:101
         *              >> Element 1--->
         *              >> 	80:800  81:9009
         */
        [[eosio::action]]
        void prntvm(name user)
        {
            PRNTCHECK();
            eosio::print("stored vector<mp_uint16>: size=", iter->vm.size()," content:\n");
            int count=0;
            for (const auto& mpval : iter->vm)
            {
                eosio::print("\nElement ", count++, "--->\n\t");
                for (auto it2 = mpval.begin(); it2 != mpval.end(); ++it2)
                    eosio::print(it2->first, ":", it2->second, "  ");
            }

        }


        /*Example:
         *  cleos --verbose push action nestcontn2a setvp '["alice", [{"first":18, "second":28}, {"first":19, "second":29}]]' -p alice@active
         */
        [[eosio::action]]
        void setvp(name user, const vector<pair<uint32_t, uint32_t> >& vp)
        {
            SETCONTAINERVAL(vp);
            eosio::print("vector<pair<uint32_t, uint32_t> > stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntvp '["alice"]' -p alice@active
         *      output: >> stored vector<pair<uint32_t, uint32_t> >  size=2:
         *              >> 18,28
         *              >> 19,29
         */
        [[eosio::action]]
        void prntvp(name user)
        {
            PRNTCHECK();
            eosio::print("stored vector<pair<uint32_t, uint32_t> >  size=",iter->vp.size(), ":\n");
            for (const auto& ele: iter->vp)
            {
                eosio::print(ele.first,",", ele.second,"\n");
            }
        }

        //=== 2. Try set - set,vector,optional,map,pair

        //Example: cleos --verbose push action nestcontn2a setstst '["alice", [[10,20],[3], [400,500,600]]]' -p alice@active
        [[eosio::action]]
        void setstst(name user, const set<set_uint16>& stst)
        {
            SETCONTAINERVAL(stst);
            eosio::print("type defined set<set_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntstst '["alice"]' -p alice@active
         *      output: >> stored set<set<T>>:size=3 vals:
         *              >> 3
         *              >> 10 20
         *              >> 400 500 600
         */
        [[eosio::action]]
        void prntstst(name user)
        {

            PRNTCHECK();
            eosio::print("stored set<set<T>>:size=", iter->stst.size(), " vals:\n");
            for (auto it1=iter->stst.begin(); it1!= iter->stst.end(); it1++)
            {
                for (auto it2=it1->begin(); it2!= it1->end(); it2++)
                    eosio::print(*it2, " ");
                eosio::print("\n");
            }
        }

         //Example: cleos --verbose push action nestcontn2a setstv '["alice", [[16,26],[36], [46,506,606]]]' -p alice@active
        [[eosio::action]]
        void setstv(name user, const set<vec_uint16>& stv)
        {
            SETCONTAINERVAL(stv);
            eosio::print("type defined set<vec_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntstv '["alice"]' -p alice@active
         *      output: >> stored set<vector<T>>:size=3 vals:
         *              >> 16 26
         *              >> 36
         *              >> 46 506 606
         */
        [[eosio::action]]
        void prntstv(name user)
        {

            PRNTCHECK();
            eosio::print("stored set<vector<T>>:size=", iter->stv.size(), " vals:\n");
            for (auto it1=iter->stv.begin(); it1!= iter->stv.end(); it1++)
            {
                for (auto it2=it1->begin(); it2!= it1->end(); it2++)
                    eosio::print(*it2, " ");
                eosio::print("\n");
            }
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a setsto '["alice", [101, null, 201, 301]]' -p alice@active
         *        ***user data can NOT be pushed into the chain, cleos get table will not work if using setsto
         *  set<optional<T> > is  NOT supported currently!
         */
        [[eosio::action]]
        void setsto(name user, const set<op_uint16>& sto)
        {
            SETCONTAINERVAL(sto);
            eosio::print("type defined set<optional<T> > stored successfully!");
        }

        //Example: cleos --verbose push action nestcontn2a  prntsto '["alice"]' -p alice@active
        //          NOT supported
        [[eosio::action]]
        void prntsto(name user)
        {
            PRNTCHECK();
            eosio::print("stored vector<op_uint16> vals:\n");
            for (const auto& ele : iter->sto)
            {
                if (ele)
                    eosio::print(ele.value(), " ");
                else
                    eosio::print("NULL", " ");
            }
        }

        /*Example:
         * cleos --verbose push action nestcontn2a setstm '["alice", [ [{"first":30,"second":300},{"first":31,"second":301}], [{"first":60,"second":600},{"first":61,"second":601}] ]]' -p alice@active
         */
        [[eosio::action]]
        void setstm(name user, const set<mp_uint16>& stm)
        {
            SETCONTAINERVAL(stm);
            eosio::print("type defined set<map<K, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntstm '["alice"]' -p alice@active
         *      output: >> stored set<mp_uint16>: size=2 content:
         *              >>
         *              >> Element 0--->
         *              >> 	30:300  31:301
         *              >> Element 1--->
         *              >> 	60:600  61:601
         */
        [[eosio::action]]
        void prntstm(name user)
        {
            PRNTCHECK();
            eosio::print("stored set<mp_uint16>: size=", iter->stm.size()," content:\n");
            int count=0;
            for (const auto& mpval : iter->stm)
            {
                eosio::print("\nElement ", count++, "--->\n\t");
                for (auto it2 = mpval.begin(); it2 != mpval.end(); ++it2)
                    eosio::print(it2->first, ":", it2->second, "  ");
            }
        }


        /*Example:
         *  cleos --verbose push action nestcontn2a setstp '["alice", [{"first":68, "second":128}, {"first":69, "second":129}]]' -p alice@active
         */
        [[eosio::action]]
        void setstp(name user, const set<pair<uint32_t, uint32_t> >& stp)
        {
            SETCONTAINERVAL(stp);
            eosio::print("set<pair<T1,T2 >> stored successfully");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntstp '["alice"]' -p alice@active
         *      output: >> stored set<pair<uint32_t, uint32_t> >  size=2:
         *              >> 68,128
         *              >> 69,129
         */
        [[eosio::action]]
        void prntstp(name user)
        {
            PRNTCHECK();
            eosio::print("stored set<pair<uint32_t, uint32_t> >  size=",iter->stp.size(), ":\n");
            for (const auto& ele: iter->stp)
            {
                eosio::print(ele.first,",", ele.second,"\n");
            }
        }

        //=== 3. Try optional - set,vector,optional,map,pair


        /*Examples:
         *  cleos --verbose push action nestcontn2a setost '["bob", null]' -p bob@active
         *  cleos --verbose push action nestcontn2a setost '["alice", [1006,2006, 3006]]' -p alice@active
         */
        [[eosio::action]]
        void setost(name user, const optional<set_uint16>& ost)
        {
            SETCONTAINERVAL(ost);
            eosio::print("type defined optional<set<T> > stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prntost '["bob"]' -p bob@active
         *      output: >> stored optional<set_uint16>  vals:
         *              >> NULL or no value
         *
         *  cleos --verbose push action nestcontn2a  prntost '["alice"]' -p alice@active
         *      output: >> stored optional<set_uint16>  vals:
         *              >> 1006 2006 3006
         */
        [[eosio::action]]
        void prntost(name user)
        {
            PRNTCHECK();
            eosio::print("stored optional<set_uint16>  vals:\n");
            if (iter->ost)
            {
                for (const auto& ele : iter->ost.value())
                    eosio::print(ele, " ");
            }
            else
                eosio::print("NULL or no value");
        }



        /*Examples:
         *  cleos --verbose push action nestcontn2a setov '["bob", null]' -p bob@active
         *
         *  cleos --verbose push action nestcontn2a setov '["alice", [1001,2001, 3001]]' -p alice@active
         */
        [[eosio::action]]
        void setov(name user, const optional<vec_uint16>& ov)
        {
            SETCONTAINERVAL(ov);
            eosio::print("type defined optional<vector<T> > stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prntov '["bob"]' -p bob@active
         *      output: >> stored optional<vec_uint16>  vals:
         *              >> NULL or no value
         *
         *  cleos --verbose push action nestcontn2a  prntov '["alice"]' -p alice@active
         *      output: >> stored optional<vec_uint16>  vals:
         *              >> 1001 2001 3001
         */
        [[eosio::action]]
        void prntov(name user)
        {
            PRNTCHECK();
            eosio::print("stored optional<vec_uint16>  vals:\n");
            if (iter->ov)
            {
                for (const auto& ele : iter->ov.value())
                    eosio::print(ele, " ");
            }
            else
                eosio::print("NULL or no value");
        }



        /*Examples:
         *  cleos --verbose push action nestcontn2a setoo '["bob", null]' -p bob@active
         *
         *  cleos --verbose push action nestcontn2a setoo '["alice",123]' -p alice@active
         */
        [[eosio::action]]
        void setoo(name user, const optional<op_uint16>& oo)
        {
            SETCONTAINERVAL(oo);
            eosio::print("type defined optional<op_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prntoo '["bob"]' -p bob@active
         *      ouput: >> stored optional<optional<T>> val:null or no real value stored
         *
         *  cleos --verbose push action nestcontn2a  prntoo '["alice"]' -p alice@active
         *      output: >> stored optional<optional<T>> val:123
         */
        [[eosio::action]]
        void prntoo(name user)
        {
            PRNTCHECK();
            eosio::print("stored optional<optional<T>> val:");
            if (iter->oo && iter->oo.value())
                eosio::print(iter->oo.value().value(),"\n");
            else
                eosio::print("null or no real value stored");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  setom '["alice",[{"first":10,"second":1000},{"first":11,"second":1001}] ]' -p alice@active
         *
         *  cleos --verbose push action nestcontn2a  setom '["bob", null ]' -p bob@active
         */
        [[eosio::action]]
        void setom(name user, const optional<mp_uint16>& om)
        {
            SETCONTAINERVAL(om);
            eosio::print("type defined optional<map<K, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntom '["alice"]' -p alice@active
         *      output: >> size of stored om:2 vals:10:1000  11:1001
         *
         *  cleos --verbose push action nestcontn2a  prntom '["bob"]' -p bob@active
         *      output: >> optional<mp_uint16> has NULL value
         */
        [[eosio::action]]
        void prntom(name user)
        {
            PRNTCHECK();
            if (iter->om)
            {
                auto mpval = iter->om.value();
                eosio::print("size of stored om:", mpval.size()," vals:");
                for (auto it2 = mpval.begin(); it2 != mpval.end(); ++it2)
                    eosio::print(it2->first, ":", it2->second, "  ");

            }
            else
                eosio::print("optional<mp_uint16> has NULL value\n");
        }


        /*Examples:
         *  cleos --verbose push action nestcontn2a setop '["alice", {"first":60, "second":61}]' -p alice@active
         *
         *  cleos --verbose push action nestcontn2a setop '["bob", null]' -p bob@active
         */
        [[eosio::action]]
        void setop(name user, const optional<pr_unit16> & op)
        {
            SETCONTAINERVAL(op);
            eosio::print("type-defined optional-pair optional<pr_unit16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prntop '["alice"]' -p alice@active
         *      output: >> stored optional<pr_unit16> data:60 61
         *
         *  cleos --verbose push action nestcontn2a  prntop '["bob"]' -p bob@active
         *      output: >> optional<pr_unit16> has NULL value
         */
        [[eosio::action]]
        void prntop(name user)
        {
            PRNTCHECK();
            if (iter->op)
                eosio::print("stored optional<pr_unit16> data:", iter->op.value().first, " ", iter->op.value().second);
            else
                eosio::print("optional<pr_unit16> has NULL value\n");
        }


        //=== 4. Try map - set,vector,optional,map,pair

        /*Example:
         *  cleos --verbose push action nestcontn2a setmst '["alice", [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmst(name user, const map<uint16_t, set_uint16> & mst)
        {
            SETCONTAINERVAL(mst);
            eosio::print("type-defined map<K, set<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntmst '["alice"]' -p alice@active
         *      output: >> stored map<uint16_t, set_uint16>: size=2 content:
         *              >> 1:vals 10 11 12 16
         *              >> 2:vals 200 300
         */
        [[eosio::action]]
        void prntmst(name user)
        {
            PRNTCHECK();
            eosio::print("stored map<uint16_t, set_uint16>: size=", iter->mst.size()," content:\n");
            for (auto it2 = iter->mst.begin(); it2 != iter->mst.end(); ++it2)
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
         * cleos --verbose push action nestcontn2a setmv '["alice", [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmv(name user, const map<uint16_t, vec_uint16>& mv)
        {
            SETCONTAINERVAL(mv);
            eosio::print("type defined map<K, vector<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntmv '["alice"]' -p alice@active
         *      output: >> stored map<uint16_t, vec_uint16>: size=2 content:
         *              >> 1:vals 10 11 12 16
         *              >> 2:vals 200 300
         */
        [[eosio::action]]
        void prntmv(name user)
        {
            PRNTCHECK();
            eosio::print("stored map<uint16_t, vec_uint16>: size=", iter->mv.size()," content:\n");
            for (auto it2 = iter->mv.begin(); it2 != iter->mv.end(); ++it2)
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
         *  cleos --verbose push action nestcontn2a setmo '["alice", [{"key":10,"value":1000},{"key":11,"value":null}]]' -p alice@active
         */
        [[eosio::action]]
        void setmo(name user, const map<uint16_t, op_uint16>& mo)
        {
            SETCONTAINERVAL(mo);
            eosio::print("type defined map<K, optional<T> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntmo '["alice"]' -p alice@active
         *      output: >> size of stored mo:2 vals
         *              >> 10:1000 11:NULL
         */
        [[eosio::action]]
        void prntmo(name user)
        {
            PRNTCHECK();
            eosio::print("size of stored mo:", iter->mo.size()," vals\n");
            for (auto it2 = iter->mo.begin(); it2 != iter->mo.end(); ++it2)
            {
                eosio::print(it2->first, ":");
                if (it2->second)
                    eosio::print(it2->second.value()," ");
                else
                    eosio::print("NULL ");
            }
        }

        /*Example:
         *  cleos push action nestcontn2a setmm '["alice", [{"key":10,"value":[{"first":200,"second":2000}, {"first":201,"second":2001}] }, {"key":11,"value":[{"first":300,"second":3000}, {"first":301,"second":3001}] } ]]' -p alice@active
         *       Attention: please note the cleos input of mm or map<K1, map<K2, V> > is a combination of strings key/value and first/second !
         *       i.e the "value" part of mm is an array of <first,second> pairs!
         */
        [[eosio::action]]
        void setmm(name user, const map<uint16_t, mp_uint16>& mm)
        {
            SETCONTAINERVAL(mm);
            eosio::print("type defined map<K1,map<K2, V> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntmm '["alice"]' -p alice@active
         *      output: >> stored map<uint16_t, mp_uint16> val:size=2 vals are the following:
         *              >> 10--->
         *              >> 	200:2000
         *              >> 	201:2001
         *              >> 11--->
         *              >> 	300:3000
         *              >> 	301:3001
         */
        [[eosio::action]]
        void prntmm(name user)
        {
            PRNTCHECK();
            eosio::print("stored map<uint16_t, mp_uint16> val:");
            eosio::print("size=", iter->mm.size()," vals are the following:\n");
            for (auto it2 = iter->mm.begin(); it2 != iter->mm.end(); ++it2)
            {
                eosio::print(it2->first,"--->\n");
                const auto& temp = it2->second;
                for (auto it3 = temp.begin(); it3 != temp.end(); ++it3)
                    eosio::print("\t",it3->first, ":", it3->second, "\n");
            }
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a setmp '["alice", [{"key":36,"value":{"first":300, "second":301}}, {"key":37,"value":{"first":600, "second":601}} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmp(name user, const map<uint16_t, pr_unit16> & mp)
        {
            SETCONTAINERVAL(mp);
            eosio::print("type-defined map-pair map<uint16_t, pr_unit16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntmp '["alice"]' -p alice@active
         *      output: >> size of stored mp:2 vals:
         *              >> 36:300 301
         *              >> 37:600 601
         */
        [[eosio::action]]
        void prntmp(name user)
        {
            PRNTCHECK();
            eosio::print("size of stored mp:", iter->mp.size()," vals:\n");
            for (auto it2 = iter->mp.begin(); it2 != iter->mp.end(); ++it2)
                eosio::print(it2->first, ":", it2->second.first, " ", it2->second.second, "\n");
        }

        //=== 5. Try pair - set,vector,optional,map,pair

        /*Example:
         *  cleos --verbose push action nestcontn2a setpst '["alice", {"first":20, "second":[200,201,202]}]' -p alice@active
         */
        [[eosio::action]]
        void setpst(name user, const pair<uint32_t, set_uint16>& pst)
        {
            SETCONTAINERVAL(pst);
            eosio::print("type-defined pair-set pair<uint32_t, set_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntpst '["alice"]' -p alice@active
         *      output: >> content of stored pair<uint32_t, set_uint16>: first=20
         *              >> second=200 201 202
         */
        [[eosio::action]]
        void prntpst(name user)
        {
            PRNTCHECK();
            eosio::print("content of stored pair<uint32_t, set_uint16>: first=", iter->pst.first, "\nsecond=");
            for (const auto& ele: iter->pst.second)
                eosio::print(ele, " ");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a setpv '["alice", {"first":10, "second":[100,101,102]}]' -p alice@active
         */
        [[eosio::action]]
        void setpv(name user, const pair<uint32_t, vec_uint16>& pv)
        {
            SETCONTAINERVAL(pv);
            eosio::print("type-defined pair-vector pair<uint32_t, vec_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntpv '["alice"]' -p alice@active
         *      output: >> content of stored pair<uint32_t, vec_uint16>: first=10
         *              >> second=100 101 102
         */
        [[eosio::action]]
        void prntpv(name user)
        {
            PRNTCHECK();
            eosio::print("content of stored pair<uint32_t, vec_uint16>: first=", iter->pv.first, "\nsecond=");
            for (const auto& ele: iter->pv.second)
                eosio::print(ele, " ");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a setpo '["alice", {"first":70, "second":71}]' -p alice@active
         *
         *  cleos --verbose push action nestcontn2a setpo '["bob", {"first":70, "second":null}]' -p bob@active
         */
        [[eosio::action]]
        void setpo(name user, const pair<uint32_t, op_uint16> & po)
        {
            SETCONTAINERVAL(po);
            eosio::print("type-defined pair-optional pair<uint32_t, op_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prntpo '["alice"]' -p alice@active
         *      output: >> content of stored pair<uint16_t, op_unit16>: first=70
         *              >> second=71
         *
         *  cleos --verbose push action nestcontn2a  prntpo '["bob"]' -p bob@active
         *      output: >> content of stored pair<uint16_t, op_unit16>: first=70
         *              >> second=NULL
         */
        [[eosio::action]]
        void prntpo(name user)
        {
            PRNTCHECK();
            eosio::print("content of stored pair<uint16_t, op_unit16>: first=", iter->po.first, "\nsecond=");
            if (iter->po.second)
                eosio::print(iter->po.second.value(), " ");
            else
                eosio::print("NULL ");
        }

        /*Example:
         *  cleos --verbose push action nestcontn2a setpm '["alice", {"key":6, "value":[{"first":20,"second":300}, {"first":21,"second":301}] }]' -p alice@active
         *      Remark: the data input for pm uses a combination of key/vale and first/second
         */
        [[eosio::action]]
        void setpm(name user, const pair<uint16_t, mp_uint16> & pm)
        {
            SETCONTAINERVAL(pm);
            eosio::print("type-defined pair-map pair<uint16_t, mp_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action nestcontn2a  prntpm '["alice"]' -p alice@active
         *      output: >> content of stored pair<uint16_t, mp_uint16>: first=6
         *              >> second=20:300  21:301
         */
        [[eosio::action]]
        void prntpm(name user)
        {
            PRNTCHECK();
            eosio::print("content of stored pair<uint16_t, mp_uint16>: first=", iter->pm.first, "\nsecond=");
            for (auto it2 = iter->pm.second.begin(); it2 != iter->pm.second.end(); ++it2)
                eosio::print(it2->first, ":", it2->second, "  ");
        }


        /*Example:
         *  cleos --verbose push action nestcontn2a setpp '["alice", {"key":30, "value":{"first":301, "second":302} }]' -p alice@active
         *      Remark: input data for pp or pair-pair is a combination of key/value and first/second
         */
        [[eosio::action]]
        void setpp(name user, const pair<uint16_t, pr_unit16> & pp)
        {
            SETCONTAINERVAL(pp);
            eosio::print("type-defined pair-pair pair<uint16_t, pr_unit16> stored successfully!");
        }

        /*Example:
         * cleos --verbose push action nestcontn2a  prntpp '["alice"]' -p alice@active
         *      output: >> content of stored pair<uint16_t, pr_unit16>: first=30
         *              >> second=301 302
         */
        [[eosio::action]]
        void prntpp(name user)
        {
            PRNTCHECK();
            eosio::print("content of stored pair<uint16_t, pr_unit16>: first=", iter->pp.first, "\nsecond=");
            eosio::print(iter->pp.second.first, " ", iter->pp.second.second);
        }


        //Try an example of 3-layer nested container

        /*Example:
         *  cleos --verbose push action nestcontn2a setovv '["alice", [[21,22],[230], [240,250,260,280]]]' -p alice@active
         */
        [[eosio::action]]
        void setovv(name user, const optional<vecvec_uint32>& ovv)
        {
            // try type-defined 3-layer nested container optional<vector<vector<T> > > here
            SETCONTAINERVAL(ovv);
            eosio::print("type-defined optional<vecvec_uint32> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action nestcontn2a  prntovv '["alice"]' -p alice@active
         *      output: >> stored optional<vector<vector<T> > > vals:
         *              >> 21 22
         *              >> 230
         *              >> 240 250 260 280
         */
        [[eosio::action]]
        void prntovv(name user)
        {
            PRNTCHECK();
            eosio::print("stored optional<vector<vector<T> > > vals:\n");
            if (iter->ovv)
            {
              auto v2 = iter->ovv.value();
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

        //Example: cleos --verbose push action nestcontn2a erase '["alice"]' -p alice@active
        [[eosio::action]]
        void erase(name user)
        {
            require_auth(user);
            psninfoindex2 tblIndex( get_self(), get_first_receiver().value);

            auto iter = tblIndex.find(user.value);
            check(iter != tblIndex.end(), "Record does not exist");
            tblIndex.erase(iter);
            eosio::print("record erased successfully");
        }

};