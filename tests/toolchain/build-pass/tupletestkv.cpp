/* Verify the support of nested containers involving std::tuple<Ts...> for the new Key-value table eosio::kv::map
 * This tupletestkv.cpp can be  regarded as a continuation of ../nestcontn2kv.cpp
 * For each action, an example regarding how to use the action with the cleos command line is given.
 *
 * Important Remarks:
 *      1) The input of a std::tuple<T0,T1,T2> via cleos uses [ele0, ele1, ele2], it is the same as the input of a vector/set
 *      2) However the input formats of settm,settp are different from those of corresponding setvm, setvp in nestcontn2a.cpp
 *      3) More importantly, vector<optional<T> > and set<optional<T> >  are NOT yet supported as shown in nestcontn2a.cpp,
 *         BUT tuple of optional<T> is supported as shown here in this tupletest.cpp
 *
 * Expected printout:
 *      For each setx action, the printed result on the cleos console is given in its corresponding prntx action.
 */


//--- You might need to verify some eosio-defined types such as eosio::asset can be the values of kv::map

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

// use typedefs to make multi-layer nested containers work!
typedef vector<uint16_t> vec_uint16;
typedef set<uint16_t> set_uint16;
typedef optional<uint16_t> op_uint16;
typedef map<uint16_t, uint16_t> mp_uint16;
typedef pair<uint16_t, uint16_t> pr_uint16;

typedef tuple<uint16_t, uint16_t> tup_uint16;


struct person2kv {
    //  Each container/object is represented by one letter: v-vector, m-map, s-mystruct,o-optional, p-pair, t-tuple
    //  with exceptions:     s2 - mystruct2,    st - set

    tuple<uint16_t, string> t; //---new code on verifying std::tuple<Ts...>

    //Two-layer nested-container involving tuple
    vector<tup_uint16> vt;
    set<tup_uint16> stt;
    optional<tup_uint16> ot;
    map<uint16_t, tup_uint16> mt;
    pair<uint32_t, tup_uint16> pt;
    tuple<tup_uint16, tup_uint16,  tup_uint16> tt;

    tuple<uint16_t, vec_uint16, vec_uint16> tv;     //tuple of vectors
    tuple<uint16_t, set_uint16, set_uint16> tst;
    tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> to;
    tuple<uint16_t, mp_uint16, mp_uint16> tm;
    tuple<uint16_t, pr_uint16, pr_uint16> tp;       //tuple of pairs

    tuple<string, vec_uint16, pr_uint16> tmisc;     //tuple of misc. types
};


class [[eosio::contract("tupletestkv")]] tupletestkv : public eosio::contract {
    using my_map_t = eosio::kv::map<"people2kv"_n, int, person2kv>;
    private:
        my_map_t mymap{};

    public:
        using contract::contract;

        tupletestkv(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}


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
          * cleos --verbose push action tupletestkv sett '[1,[100,"str1"]]' -p alice@active
          */
        [[eosio::action]]
        void sett(int id, const tuple<uint16_t, string>& t)
        {
            SETCONTAINERVAL(t);
            eosio::print("tuple<uint16_t, string> stored successfully");
        }


         /*Examples:
         *  cleos --verbose push action tupletestkv prntt '[1]' -p alice@active
         *      output: >> elements of stored tuple t:100, str1
         */
        [[eosio::action]]
        void prntt(int id)
        {
            const auto& psn = get(id);
            eosio::print("elements of stored tuple t:", std::get<0>(psn.t), ", ", std::get<1>(psn.t));
        }

        //=== 1. Try other containers (vector,set,optional,map,pair,tuple)  of tuples

        //Example: cleos --verbose push action tupletestkv setvt '[1, [[10,20],[30,40], [50,60]]]' -p alice@active
        [[eosio::action]]
        void setvt(int id, const vector<tup_uint16>& vt)
        {
            SETCONTAINERVAL(vt);
            eosio::print("type defined vector<tup_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prntvt '[1]' -p alice@active
         *      output: >> stored vector<tuple<Ts...>> vals:
         *              >> 10 20
         *              >> 30 40
         *              >> 50 60
         */
        [[eosio::action]]
        void prntvt(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored vector<tuple<Ts...>> vals:\n");
            for (int i=0 ; i < psn.vt.size(); i++)
            {
                eosio::print(std::get<0>(psn.vt[i]), " ", std::get<1>(psn.vt[i]));
                eosio::print("\n");
            }
        }

        //Example: cleos --verbose push action tupletestkv setstt '[1, [[1,2],[36,46], [56,66]]]' -p alice@active
        [[eosio::action]]
        void setstt(int id, const set<tup_uint16>& stt)
        {
            SETCONTAINERVAL(stt);
            eosio::print("type defined set<tup_uint16> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prntstt '[1]' -p alice@active
         *      output: >> stored set<tuple<Ts...>> vals:
         *              >> 1 2
         *              >> 36 46
         *              >> 56 66
         */
        [[eosio::action]]
        void prntstt(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored set<tuple<Ts...>> vals:\n");
            for (auto it1=psn.stt.begin(); it1!= psn.stt.end(); it1++)
            {
                eosio::print(std::get<0>(*it1), " ", std::get<1>(*it1));
                eosio::print("\n");
            }
        }

         /*Examples:
         *  cleos --verbose push action tupletestkv setot '[1, null]' -p bob@active
         *
         *  cleos --verbose push action tupletestkv setot '[2, [1001,2001]]' -p alice@active
         */
        [[eosio::action]]
        void setot(int id, const optional<tup_uint16>& ot)
        {
            SETCONTAINERVAL(ot);
            eosio::print("type defined optional<tup_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action tupletestkv  prntot '[1]' -p bob@active
         *      output: >> stored optional<tup_uint16>  vals:
         *              >> NULL or no value
         *
         *  cleos --verbose push action tupletestkv  prntot '[2]' -p alice@active
         *      output: >> stored optional<tup_uint16>  vals:
         *              >> 1001 2001
         */
        [[eosio::action]]
        void prntot(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored optional<tup_uint16>  vals:\n");
            if (psn.ot)
            {
                eosio::print(std::get<0>(psn.ot.value()), " ", std::get<1>(psn.ot.value()));
            }
            else
                eosio::print("NULL or no value");
        }

        /*Example:
         * cleos --verbose push action tupletestkv setmt '[1, [{"key":1,"value":[10,11]},  {"key":2,"value":[200,300]} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmt(int id, const map<uint16_t, tup_uint16>& mt)
        {
            SETCONTAINERVAL(mt);
            eosio::print("type defined map<uint16_t, tup_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prntmt '[1]' -p alice@active
         *      output: >> stored map<uint16_t, vec_uint16>: size=2 content:
         *              >> 1:vals 10 11
         *              >> 2:vals 200 300
         */
        [[eosio::action]]
        void prntmt(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored map<uint16_t, tup_uint16>: size=", psn.mt.size()," content:\n");
            for (auto it2 = psn.mt.begin(); it2 != psn.mt.end(); ++it2)
            {
                    eosio::print(it2->first, ":", "vals ");
                    eosio::print(std::get<0>(it2->second), " ", std::get<1>(it2->second));
                    eosio::print("\n");
            }
        }



        /*Example:
         * cleos --verbose push action tupletestkv setpt '[1, {"first":10, "second":[100,101]}]' -p alice@active
         */
        [[eosio::action]]
        void setpt(int id, const pair<uint32_t, tup_uint16>& pt)
        {
            SETCONTAINERVAL(pt);
            eosio::print("type defined pair<uint32_t, tup_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prntpt '[1]' -p alice@active
         *      output: >> content of stored pair<uint32_t, tup_uint16>: first=10
         *              >> second=100 101
         */
        [[eosio::action]]
        void prntpt(int id)
        {
            const auto& psn = get(id);

            eosio::print("content of stored pair<uint32_t, tup_uint16>: first=", psn.pt.first, "\nsecond=");
            eosio::print(std::get<0>(psn.pt.second), " ", std::get<1>(psn.pt.second));
        }

        //Example: cleos --verbose push action tupletestkv settt '[1, [[1,2],[30,40], [50,60]]]' -p alice@active
        [[eosio::action]]
        void settt(int id, const tuple<tup_uint16, tup_uint16,  tup_uint16>& tt)
        {
            SETCONTAINERVAL(tt);
            eosio::print("type defined tuple<tuple<Ts...> > stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prnttt '[1]' -p alice@active
         *      output: >> stored tuple<tup_uint16, tup_uint16,  tup_uint16> vals:
         *              >> 1 2
         *              >> 30 40
         *              >> 50 60
         */
        [[eosio::action]]
        void prnttt(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<tup_uint16, tup_uint16,  tup_uint16> vals:\n");
            const auto& ele0 = std::get<0>(psn.tt);
            const auto& ele1 = std::get<1>(psn.tt);
            const auto& ele2 = std::get<2>(psn.tt);

            eosio::print(std::get<0>(ele0), " ", std::get<1>(ele0), "\n");
            eosio::print(std::get<0>(ele1), " ", std::get<1>(ele1), "\n");
            eosio::print(std::get<0>(ele2), " ", std::get<1>(ele2), "\n");
        }

         //=== 2. Try tuple of other containers (vector,set,optional,map,pair)

         //Example: cleos --verbose push action tupletestkv settv '[1, [16,[26,36], [46,506,606]]]' -p alice@active
        [[eosio::action]]
        void settv(int id, const tuple<uint16_t, vec_uint16, vec_uint16>& tv)
        {
            SETCONTAINERVAL(tv);
            eosio::print("type defined tuple<uint16_t, vec_uint16, vec_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prnttv '[1]' -p alice@active
         *      output: >> stored tuple<uint16_t, vec_uint16, vec_uint16>  vals:
         *              >> ele0: 16
         *              >> ele1: 26 36
         *              >> ele2: 46 506 606
         */
        [[eosio::action]]
        void prnttv(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<uint16_t, vec_uint16, vec_uint16>  vals:\n");

            auto printVec = [](vec_uint16 v) {//eosio-cpp compiler allows c++11 usage such as lambda
                for (int i=0; i < v.size();i++)
                    eosio::print(v[i]," ");
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(psn.tv),"\n");
            eosio::print("ele1: "); printVec(std::get<1>(psn.tv));
            eosio::print("ele2: "); printVec(std::get<2>(psn.tv));
        }

         //Example: cleos --verbose push action tupletestkv settst '[1, [10,[21,31], [41,51,61]]]' -p alice@active
        [[eosio::action]]
        void settst(int id, const tuple<uint16_t, set_uint16, set_uint16>& tst)
        {
            SETCONTAINERVAL(tst);
            eosio::print("type defined tuple<uint16_t, set_uint16, set_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prnttst '[1]' -p alice@active
         *      output: >> stored tuple<uint16_t, set_uint16, set_uint16>  vals:
         *              >> ele0: 10
         *              >> ele1: 21 31
         *              >> ele2: 41 51 61
         */
        [[eosio::action]]
        void prnttst(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<uint16_t, set_uint16, set_uint16> vals:\n");

            auto printSet = [](set_uint16 s) {
                for (auto it = s.begin(); it !=s.end(); it++ )
                    eosio::print(*it, " ");
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(psn.tst),"\n");
            eosio::print("ele1: "); printSet(std::get<1>(psn.tst));
            eosio::print("ele2: "); printSet(std::get<2>(psn.tst));
        }

         /*Examples:
         *  cleos --verbose push action tupletestkv  setto '[1, [100, null, 200, null, 300]]' -p alice@active
         *
         *  cleos --verbose push action tupletestkv  setto '[2, [null, null, 10, null, 20]]' -p bob@active
         *      Remark: Yes, tuple of optionals is supported here !
         */
        [[eosio::action]]
        void setto(int id, const tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> & to)
        {
            SETCONTAINERVAL(to);
            eosio::print("type defined tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action tupletest  prntto '[1]' -p alice@active
         *      output: >> stored tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> vals:
         *              >> 100 NULL 200 NULL 300
         *
         *  cleos --verbose push action tupletest  prntto '[2]' -p bob@active
         *      output: >> stored tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> vals:
         *              >> NULL NULL 10 NULL 20
         */
        [[eosio::action]]
        void prntto(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> vals:\n");
            auto printOptional = [](op_uint16 op) {
                if (op)
                    eosio::print(op.value(),  " ");
                else
                    eosio::print("NULL", " ");
            };

            printOptional(std::get<0>(psn.to));
            printOptional(std::get<1>(psn.to));
            printOptional(std::get<2>(psn.to));
            printOptional(std::get<3>(psn.to));
            printOptional(std::get<4>(psn.to));
            eosio::print("\n");
        }


        //Example: cleos --verbose push action tupletestkv settm '[1, [126, [{"key":10,"value":100},{"key":11,"value":101}], [{"key":80,"value":800},{"key":81,"value":9009}] ]]' -p alice@active
        //         ******Note: The input format of settm is different from that of setvm in nestcontn2a.cpp!
        [[eosio::action]]
        void settm(int id, const tuple<uint16_t, mp_uint16, mp_uint16>& tm)
        {
            SETCONTAINERVAL(tm);
            eosio::print("type defined tuple<uint16_t, mp_uint16, mp_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prnttm '[1]' -p alice@active
         *      output: >> stored tuple<uint16_t, mp_uint16, mp_uint16> vals:
         *              >> ele0: 126
         *              >> ele1: 10:100 11:101
         *              >> ele2: 80:800 81:9009
         */
        [[eosio::action]]
        void prnttm(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<uint16_t, mp_uint16, mp_uint16> vals:\n");

            auto printMap = [](mp_uint16 m) {
                for (auto it = m.begin(); it !=m.end(); it++ )
                    eosio::print(it->first, ":", it->second, " ");
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(psn.tm),"\n");
            eosio::print("ele1: "); printMap(std::get<1>(psn.tm));
            eosio::print("ele2: "); printMap(std::get<2>(psn.tm));
        }

        //Example:  cleos --verbose push action tupletestkv settp '[1, [127, {"key":18, "value":28}, {"key":19, "value":29}]]' -p alice@active
        //         ******Note: The input format of settp is different from that of setvp in nestcontn2a.cpp!
        [[eosio::action]]
        void settp(int id, const tuple<uint16_t, pr_uint16, pr_uint16>& tp)
        {
            SETCONTAINERVAL(tp);
            eosio::print("type defined tuple<uint16_t, pr_uint16, pr_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prnttp '[1]' -p alice@active
         *      output: >> stored tuple<uint16_t, pr_unit16, pr_unit16> vals:
         *              >> ele0: 127
         *              >> ele1: 18:28
         *              >> ele2: 19:29
         */
        [[eosio::action]]
        void prnttp(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<uint16_t, pr_unit16, pr_unit16> vals:\n");

            auto printPair = [](pr_uint16 p) {
                eosio::print(p.first, ":", p.second);
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(psn.tp),"\n");
            eosio::print("ele1: "); printPair(std::get<1>(psn.tp));
            eosio::print("ele2: "); printPair(std::get<2>(psn.tp));
        }

        //Example: cleos --verbose push action tupletestkv settmisc '[1, ["strHere", [10,11,12,16], {"key":86,"value":96}] ]' -p alice@active
        [[eosio::action]]
        void settmisc(int id, const tuple<string, vec_uint16, pr_uint16>  & tmisc)
        {
            SETCONTAINERVAL(tmisc);
            eosio::print("type defined tuple<string, vec_uint16, pr_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletestkv  prnttmisc '[1]' -p alice@active
         *      output: >> stored tuple<string, vec_uint16, pr_uint16> vals:
         *              >> ele0: strHere
         *              >> ele1: 10 11 12 16
         *              >> ele2: 86:96
         */
        [[eosio::action]]
        void prnttmisc(int id)
        {
            const auto& psn = get(id);
            eosio::print("stored tuple<string, vec_uint16, pr_uint16> vals:\n");

            auto printVec = [](vec_uint16 v) {
                for (int i=0; i < v.size();i++)
                    eosio::print(v[i]," ");
                eosio::print("\n");
            };

            auto printPair = [](pr_uint16 p) {
                eosio::print(p.first, ":", p.second);
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(psn.tmisc),"\n");
            eosio::print("ele1: "); printVec(std::get<1>(psn.tmisc));
            eosio::print("ele2: "); printPair(std::get<2>(psn.tmisc));
        }

        //The following are needed for eosio-cpp compiler to identify the actions in genearted .abi
        using get_action = eosio::action_wrapper<"get"_n, &tupletestkv::get>;

        using sett_action = eosio::action_wrapper<"sett"_n, &tupletestkv::sett>;
        using prntt_action = eosio::action_wrapper<"prntt"_n, &tupletestkv::prntt>;
        using setvt_action = eosio::action_wrapper<"setvt"_n, &tupletestkv::setvt>;
        using prntvt_action = eosio::action_wrapper<"prntvt"_n, &tupletestkv::prntvt>;
        using setstt_action = eosio::action_wrapper<"setstt"_n, &tupletestkv::setstt>;
        using prntstt_action = eosio::action_wrapper<"prntstt"_n, &tupletestkv::prntstt>;
        using setot_action = eosio::action_wrapper<"setot"_n, &tupletestkv::setot>;
        using prntot_action = eosio::action_wrapper<"prntot"_n, &tupletestkv::prntot>;
        using setmt_action = eosio::action_wrapper<"setmt"_n, &tupletestkv::setmt>;
        using prntmt_action = eosio::action_wrapper<"prntmt"_n, &tupletestkv::prntmt>;
        using setpt_action = eosio::action_wrapper<"setpt"_n, &tupletestkv::setpt>;
        using prntpt_action = eosio::action_wrapper<"prntpt"_n, &tupletestkv::prntpt>;
        using settt_action = eosio::action_wrapper<"settt"_n, &tupletestkv::settt>;
        using prnttt_action = eosio::action_wrapper<"prnttt"_n, &tupletestkv::prnttt>;

        using settv_action = eosio::action_wrapper<"settv"_n, &tupletestkv::settv>;
        using prnttv_action = eosio::action_wrapper<"prnttv"_n, &tupletestkv::prnttv>;
        using settst_action = eosio::action_wrapper<"settst"_n, &tupletestkv::settst>;
        using prnttst_action = eosio::action_wrapper<"prnttst"_n, &tupletestkv::prnttst>;
        using setto_action = eosio::action_wrapper<"setto"_n, &tupletestkv::setto>;
        using prntto_action = eosio::action_wrapper<"prntto"_n, &tupletestkv::prntto>;
        using settm_action = eosio::action_wrapper<"settm"_n, &tupletestkv::settm>;
        using prnttm_action = eosio::action_wrapper<"prnttm"_n, &tupletestkv::prnttm>;
        using settp_action = eosio::action_wrapper<"settp"_n, &tupletestkv::settp>;
        using prnttp_action = eosio::action_wrapper<"prnttp"_n, &tupletestkv::prnttp>;
        using settmisc_action = eosio::action_wrapper<"settmisc"_n, &tupletestkv::settmisc>;
        using prnttmisc_action = eosio::action_wrapper<"prnttmisc"_n, &tupletestkv::prnttmisc>;
};


