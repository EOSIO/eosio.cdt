/* Verify the support of nested containers involving std::tuple<Ts...> in eosio multi-index table
 * This tupletest.cpp can be  regarded as a continuation of ../nestcontn2a.cpp
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


// use typedefs to make multi-layer nested containers work!
typedef vector<uint16_t> vec_uint16;
typedef set<uint16_t> set_uint16;
typedef optional<uint16_t> op_uint16;
typedef map<uint16_t, uint16_t> mp_uint16;
typedef pair<uint16_t, uint16_t> pr_uint16;

typedef tuple<uint16_t, uint16_t> tup_uint16;

class [[eosio::contract("tupletest")]] tupletest : public eosio::contract {
    private:
        struct [[eosio::table]] person2 {
            name key;

            //  Each container/object is represented by one letter: v-vector, m-map, s-mystruct,o-optional, p-pair, t - tuple
            //  with exceptions:     s2 - mystruct2,    st - set

            tuple<uint16_t, string> t;

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



            uint64_t primary_key() const { return key.value; }
        };
        using psninfoindex2 = eosio::multi_index<"people2"_n, person2>;


    public:
        using contract::contract;

        tupletest(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

        /*Examples:
         * cleos --verbose push action tupletest sett '["alice", [100,"strA"]]' -p alice@active
         * cleos --verbose push action tupletest sett '["bob", [200, "strB"]]' -p bob@active
          */
        [[eosio::action]]
        void sett(name user, const tuple<uint16_t, string>& t)
        {
            SETCONTAINERVAL(t);
            eosio::print("tuple<uint16_t, string> stored successfully");
        }

        /*Examples:
         *  cleos --verbose push action tupletest prntt '["alice"]' -p alice@active
         *      output: >> elements of stored tuple t:100, strA
         *
         *  cleos --verbose push action tupletest prntt '["bob"]' -p bob@active
         *      output: >> elements of stored tuple t:200, strB
         */
        [[eosio::action]]
        void prntt(name user)
        {
            PRNTCHECK();
            eosio::print("elements of stored tuple t:", std::get<0>(iter->t), ", ", std::get<1>(iter->t));
        }

        //=== 1. Try other containers (vector,set,optional,map,pair,tuple)  of tuples

        //Example: cleos --verbose push action tupletest setvt '["alice", [[10,20],[30,60], [80,90]]]' -p alice@active
        [[eosio::action]]
        void setvt(name user, const vector<tup_uint16>& vt)
        {
            SETCONTAINERVAL(vt);
            eosio::print("type defined vector<tup_uint16>  or vector<tuple<Ts...> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action tupletest  prntvt '["alice"]' -p alice@active
         *      output: >> stored vector<tuple<Ts...> vals:
         *              >> 10 20
         *              >> 30 60
         *              >> 80 90
         */
        [[eosio::action]]
        void prntvt(name user)
        {

            PRNTCHECK();
            eosio::print("stored vector<tuple<Ts...> vals:\n");
            for (int i=0 ; i < iter->vt.size(); i++)
            {
                eosio::print(std::get<0>(iter->vt[i]), " ", std::get<1>(iter->vt[i]));
                eosio::print("\n");
            }
        }

        //Example: cleos --verbose push action tupletest  setstt '["alice", [[1,2],[3,6], [8,9]]]' -p alice@active
        [[eosio::action]]
        void setstt(name user, const set<tup_uint16> & stt)
        {
            SETCONTAINERVAL(stt);
            eosio::print("type defined set<tup_uint16> or set<tuple<Ts...> stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action tupletest   prntstt '["alice"]' -p alice@active
         *      output: >> stored set<tuple<Ts...> vals:
         *              >> 1 2
         *              >> 3 6
         *              >> 8 9
         */
        [[eosio::action]]
        void prntstt(name user)
        {

            PRNTCHECK();
            eosio::print("stored set<tuple<Ts...> vals:\n");
            for (auto it1=iter->stt.begin(); it1!= iter->stt.end(); it1++)
            {
                eosio::print(std::get<0>(*it1), " ", std::get<1>(*it1));
                eosio::print("\n");
            }
        }


        /*Examples:
         *  cleos --verbose push action tupletest setot '["bob", null]' -p bob@active
         *
         *  cleos --verbose push action tupletest setot '["alice", [1001,2001]]' -p alice@active
         */
        [[eosio::action]]
        void setot(name user, const optional<tup_uint16> & ot)
        {
            SETCONTAINERVAL(ot);
            eosio::print("type defined optional<tuple<Ts...> > stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action tupletest  prntot '["bob"]' -p bob@active
         *      output: >> stored optional<tup_uint16>  vals:
         *              >> NULL or no value
         *
         *  cleos --verbose push action tupletest  prntot '["alice"]' -p alice@active
         *      output: >> stored optional<tup_uint16>  vals:
         *              >> 1001 2001
         */
        [[eosio::action]]
        void prntot(name user)
        {
            PRNTCHECK();
            eosio::print("stored optional<tup_uint16>  vals:\n");
            if (iter->ot)
            {
                eosio::print(std::get<0>(iter->ot.value()), " ", std::get<1>(iter->ot.value()));
            }
            else
                eosio::print("NULL or no value");
        }



        /*Example:
         * cleos --verbose push action tupletest setmt '["alice", [{"key":1,"value":[10,11]},  {"key":2,"value":[200,300]} ]]' -p alice@active
         */
        [[eosio::action]]
        void setmt(name user, const map<uint16_t, tup_uint16> & mt)
        {
            SETCONTAINERVAL(mt);
            eosio::print("type defined map<K, tuple<Ts...> > stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prntmt '["alice"]' -p alice@active
         *      output: >> stored map<uint16_t, vec_uint16>: size=2 content:
         *              >> 1:vals 10 11
         *              >> 2:vals 200 300
         */
        [[eosio::action]]
        void prntmt(name user)
        {
            PRNTCHECK();
            eosio::print("stored map<uint16_t, tup_uint16>: size=", iter->mt.size()," content:\n");
            for (auto it2 = iter->mt.begin(); it2 != iter->mt.end(); ++it2)
            {
                    eosio::print(it2->first, ":", "vals ");
                    eosio::print(std::get<0>(it2->second), " ", std::get<1>(it2->second));
                    eosio::print("\n");
            }
        }

        /*Example:
         *  cleos --verbose push action tupletest setpt '["alice", {"first":10, "second":[100,101]}]' -p alice@active
         */
        [[eosio::action]]
        void setpt(name user, const pair<uint32_t, tup_uint16>& pt)
        {
            SETCONTAINERVAL(pt);
            eosio::print("type-defined pair-tuple pair<uint32_t, tup_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prntpt '["alice"]' -p alice@active
         *      output: >> content of stored pair<uint32_t, tup_uint16>: first=10
         *              >> second=100 101
         */
        [[eosio::action]]
        void prntpt(name user)
        {
            PRNTCHECK();
            eosio::print("content of stored pair<uint32_t, tup_uint16>: first=", iter->pt.first, "\nsecond=");
            eosio::print(std::get<0>(iter->pt.second), " ", std::get<1>(iter->pt.second));
        }

        //Example: cleos --verbose push action tupletest settt '["alice", [[1,2],[30,40], [50,60]]]' -p alice@active
        [[eosio::action]]
        void settt(name user, const tuple<tup_uint16, tup_uint16,  tup_uint16>& tt)
        {
            SETCONTAINERVAL(tt);
            eosio::print("type defined tuple<tuple<Ts...> > stored successfully!");

        }

        /*Example:
         *  cleos --verbose push action tupletest  prnttt '["alice"]' -p alice@active
         *      output: >> stored tuple<tup_uint16, tup_uint16,  tup_uint16> vals:
         *              >> 1 2
         *              >> 30 40
         *              >> 50 60
         */
        [[eosio::action]]
        void prnttt(name user)
        {

            PRNTCHECK();
            eosio::print("stored tuple<tup_uint16, tup_uint16,  tup_uint16> vals:\n");
            const auto& ele0 = std::get<0>(iter->tt);
            const auto& ele1 = std::get<1>(iter->tt);
            const auto& ele2 = std::get<2>(iter->tt);

            eosio::print(std::get<0>(ele0), " ", std::get<1>(ele0), "\n");
            eosio::print(std::get<0>(ele1), " ", std::get<1>(ele1), "\n");
            eosio::print(std::get<0>(ele2), " ", std::get<1>(ele2), "\n");
        }

        //=== 2. Try tuple of other containers (vector,set,optional,map,pair)

         //Example: cleos --verbose push action tupletest settv '["alice", [16,[26,36], [46,506,606]]]' -p alice@active
        [[eosio::action]]
        void settv(name user, const tuple<uint16_t, vec_uint16, vec_uint16>& tv)
        {
            SETCONTAINERVAL(tv);
            eosio::print("type defined tuple<uint16_t, vec_uint16, vec_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prnttv '["alice"]' -p alice@active
         *      output: >> stored tuple<uint16_t, vec_uint16, vec_uint16>  vals:
         *              >> ele0: 16
         *              >> ele1: 26 36
         *              >> ele2: 46 506 606
         */
        [[eosio::action]]
        void prnttv(name user)
        {

            PRNTCHECK();

            eosio::print("stored tuple<uint16_t, vec_uint16, vec_uint16>  vals:\n");

            auto printVec = [](vec_uint16 v) {//eosio-cpp compiler allows c++11 usage such as lambda
                for (int i=0; i < v.size();i++)
                    eosio::print(v[i]," ");
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(iter->tv),"\n");
            eosio::print("ele1: "); printVec(std::get<1>(iter->tv));
            eosio::print("ele2: "); printVec(std::get<2>(iter->tv));
        }

         //Example: cleos --verbose push action tupletest settst '["alice", [10,[21,31], [41,51,61]]]' -p alice@active
        [[eosio::action]]
        void settst(name user, const tuple<uint16_t, set_uint16, set_uint16>& tst)
        {
            SETCONTAINERVAL(tst);
            eosio::print("type defined tuple<uint16_t, set_uint16, set_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prnttst '["alice"]' -p alice@active
         *      output: >> stored tuple<uint16_t, set_uint16, set_uint16>  vals:
         *              >> ele0: 10
         *              >> ele1: 21 31
         *              >> ele2: 41 51 61
         */
        [[eosio::action]]
        void prnttst(name user)
        {

            PRNTCHECK();

            eosio::print("stored tuple<uint16_t, set_uint16, set_uint16> vals:\n");

            auto printSet = [](set_uint16 s) {
                for (auto it = s.begin(); it !=s.end(); it++ )
                    eosio::print(*it, " ");
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(iter->tst),"\n");
            eosio::print("ele1: "); printSet(std::get<1>(iter->tst));
            eosio::print("ele2: "); printSet(std::get<2>(iter->tst));
        }

        /*Examples:
         *  cleos --verbose push action tupletest  setto '["alice", [100, null, 200, null, 300]]' -p alice@active
         *
         *  cleos --verbose push action tupletest  setto '["bob", [null, null, 10, null, 20]]' -p bob@active
         *      Remark: Yes, tuple of optionals is supported here !
         */
        [[eosio::action]]
        void setto(name user, const tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> & to)
        {
            SETCONTAINERVAL(to);
            eosio::print("type defined tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> stored successfully!");
        }

        /*Examples:
         *  cleos --verbose push action tupletest  prntto '["alice"]' -p alice@active
         *      output: >> stored tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> vals:
         *              >> 100 NULL 200 NULL 300
         *
         *  cleos --verbose push action tupletest  prntto '["bob"]' -p bob@active
         *      output: >> stored tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> vals:
         *              >> NULL NULL 10 NULL 20
         */
        [[eosio::action]]
        void prntto(name user)
        {
            PRNTCHECK();
            eosio::print("stored tuple<op_uint16, op_uint16, op_uint16,op_uint16,op_uint16> vals:\n");
            auto printOptional = [](op_uint16 op) {
                if (op)
                    eosio::print(op.value(),  " ");
                else
                    eosio::print("NULL", " ");
            };

            printOptional(std::get<0>(iter->to));
            printOptional(std::get<1>(iter->to));
            printOptional(std::get<2>(iter->to));
            printOptional(std::get<3>(iter->to));
            printOptional(std::get<4>(iter->to));
            eosio::print("\n");
        }


        //Example: cleos --verbose push action tupletest settm '["alice", [126, [{"key":10,"value":100},{"key":11,"value":101}], [{"key":80,"value":800},{"key":81,"value":9009}] ]]' -p alice@active
        //         ******Note: The input format of settm is different from that of setvm in nestcontn2a.cpp!
        [[eosio::action]]
        void settm(name user, const tuple<uint16_t, mp_uint16, mp_uint16>& tm)
        {
            SETCONTAINERVAL(tm);
            eosio::print("type defined tuple<uint16_t, mp_uint16, mp_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prnttm '["alice"]' -p alice@active
         *      output: >> stored tuple<uint16_t, mp_uint16, mp_uint16> vals:
         *              >> ele0: 126
         *              >> ele1: 10:100 11:101
         *              >> ele2: 80:800 81:9009
         */
        [[eosio::action]]
        void prnttm(name user)
        {

            PRNTCHECK();

            eosio::print("stored tuple<uint16_t, mp_uint16, mp_uint16> vals:\n");

            auto printMap = [](mp_uint16 m) {
                for (auto it = m.begin(); it !=m.end(); it++ )
                    eosio::print(it->first, ":", it->second, " ");
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(iter->tm),"\n");
            eosio::print("ele1: "); printMap(std::get<1>(iter->tm));
            eosio::print("ele2: "); printMap(std::get<2>(iter->tm));
        }

        //Example:  cleos --verbose push action tupletest settp '["alice", [127, {"key":18, "value":28}, {"key":19, "value":29}]]' -p alice@active
        //         ******Note: The input format of settp is different from that of setvp in nestcontn2a.cpp!
        [[eosio::action]]
        void settp(name user, const tuple<uint16_t, pr_uint16, pr_uint16>& tp)
        {
            SETCONTAINERVAL(tp);
            eosio::print("type defined tuple<uint16_t, pr_uint16, pr_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prnttp '["alice"]' -p alice@active
         *      output: >> stored tuple<uint16_t, pr_unit16, pr_unit16> vals:
         *              >> ele0: 127
         *              >> ele1: 18:28
         *              >> ele2: 19:29
         */
        [[eosio::action]]
        void prnttp(name user)
        {

            PRNTCHECK();

            eosio::print("stored tuple<uint16_t, pr_unit16, pr_unit16> vals:\n");

            auto printPair = [](pr_uint16 p) {
                eosio::print(p.first, ":", p.second);
                eosio::print("\n");
            };
            eosio::print("ele0: ", std::get<0>(iter->tp),"\n");
            eosio::print("ele1: "); printPair(std::get<1>(iter->tp));
            eosio::print("ele2: "); printPair(std::get<2>(iter->tp));
        }

        //Example: cleos --verbose push action tupletest settmisc '["alice", ["strHere", [10,11,12,16], {"key":86,"value":96}] ]' -p alice@active
        [[eosio::action]]
        void settmisc(name user, const tuple<string, vec_uint16, pr_uint16>  & tmisc)
        {
            SETCONTAINERVAL(tmisc);
            eosio::print("type defined tuple<string, vec_uint16, pr_uint16> stored successfully!");
        }

        /*Example:
         *  cleos --verbose push action tupletest  prnttmisc '["alice"]' -p alice@active
         *      output: >> stored tuple<string, vec_uint16, pr_uint16> vals:
         *              >> ele0: strHere
         *              >> ele1: 10 11 12 16
         *              >> ele2: 86:96
         */
        [[eosio::action]]
        void prnttmisc(name user)
        {

            PRNTCHECK();

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
            eosio::print("ele0: ", std::get<0>(iter->tmisc),"\n");
            eosio::print("ele1: "); printVec(std::get<1>(iter->tmisc));
            eosio::print("ele2: "); printPair(std::get<2>(iter->tmisc));
        }
};