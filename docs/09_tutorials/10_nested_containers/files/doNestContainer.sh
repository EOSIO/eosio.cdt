#!/bin/bash

#
# This script is used to test all the cleos commands of the EOSIO actions coded in nestcontn2a.cpp and tupletest.cpp,
# which are in the subdirectory tests/toolchain/build-pass/ of eosio.cdt
#
# Before running this script, make sure your default wallet is unlocked, and replace EOS6...5CV with your own development key
# In a development environment, you may need to restart nodeos
#
# Here it is assumed that the current working directory is ~/WorkNestedContainer/nested-container/nestcontn2a/,
# which has nestcontn2a.cpp and a subdirectory tupletest/, the subdirectory tupletest/ has tupletest.cpp
#
# Naming Convention of the containers:
#   Each container/object is represented by one letter: v-vector, m-map, s-mystruct,o-optional, p-pair, t-tuple
#                                      with exceptions: s2 - mystruct2,   st - set
#   You can use above naming convention to search for corresponding cleos command of nested containers, e.g
#       -  'setvm'  handles vector of maps
#       -  'setost' handles optional of set

# eacho -e 'Make sure nodeos running properly\n'
cleos create account eosio alice EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio jane EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
read -p 'test accounts alice,bob,jane created' notcare
    # notcare means we do not care an user input, notcare is used in this script for you to pause and examine the intermediate result

# echo -e 'Get to the directory that has this doNestContainer.sh script\n'
# cd ~/WorkNestedContainer/nested-container/nestcontn2a/

echo -e '\n\n eosio-cpp is compiling contract nestcontn2a, please wait...\n'
eosio-cpp nestcontn2a.cpp  #compile nestcontn2a.cpp to get updated .wasm and .abi
sleep 2

# always use --verbose mode to launch cleos to display multi-line output and reveal some internals
cleos --verbose create account eosio nestcontn2a EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
cleos --verbose set contract nestcontn2a ./ -p nestcontn2a@active
read -p 'nestcontn2a.cpp is compiled and the contract nestcontn2a is set and loaded into block chain' notcare

echo -e "\n\n=========================pass vector<T> using cleos"
cleos --verbose push action nestcontn2a setv '["alice", [100,200,300,600]]' -p alice@active
cleos --verbose push action nestcontn2a setv '["bob", []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a prntv '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a prntv '["bob"]' -p bob@active
read -p "verified action setv for vector<uint16_t>" notcare

echo -e "\n\n=========================pass set<T> using cleos"
cleos --verbose push action nestcontn2a setst '["alice", [101,201,301]]' -p alice@active
cleos --verbose push action nestcontn2a setst '["bob", []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a prntst '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a prntst '["bob"]' -p bob@active
read -p "verified action setst for set<uint16_t>" notcare

echo -e "\n\n=========================pass map<K,V> using cleos"
cleos --verbose push action nestcontn2a setm '["alice", [{"key":"str1","value":"str1val"}, {"key":"str3","value":"str3val"}]]' -p alice@active
cleos --verbose push action nestcontn2a setm '{"user":"jane", "m":[{"key":"str4", "value":"str4val"}, {"key":"str6", "value":"str6val"}]}' -p jane@active
cleos --verbose push action nestcontn2a setm '["bob", []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a prntm '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a prntm '["jane"]' -p jane@active
cleos --verbose push action nestcontn2a prntm '["bob"]' -p bob@active
read -p "verified action setm for map<string,string>:alice and jane both verified"  notcare

echo -e "\n\n=========================pass simple struct using cleos"
cleos --verbose push action nestcontn2a sets '["alice", {"_count":18, "_strID":"dumstr"}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prnts '["alice"]' -p alice@active
read -p "verified action sets for simple mystruct "  notcare

echo -e "\n\n=========================pass struct of structs using cleos"
cleos --verbose push action nestcontn2a sets2 '["alice", {"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prnts2 '["alice"]' -p alice@active
read -p "verified action sets2 for complex mystruct2 "  notcare

echo -e "\n\n=========================pass vector<mystruct> using cleos"
cleos --verbose push action nestcontn2a setvs '["alice", [{"_count":18, "_strID":"dumstr"},{"_count":19, "_strID":"dumstr2"}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntvs '["alice"]' -p alice@active
read -p "verified action setvs for vector<mystruct> "  notcare


echo -e "\n\n=========================pass optional<T> using cleos"
cleos --verbose push action nestcontn2a seto '["bob", null]' -p bob@active   #can pass null for optional<string>
cleos --verbose push action nestcontn2a seto '["alice","hello strval22"]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prnto '["bob"]' -p bob@active
cleos --verbose push action nestcontn2a  prnto '["alice"]' -p alice@active
read -p "verified action seto for null case and non-null case of optional<string> "  notcare


echo -e "\n\n=========================pass pair<T1,T2> using cleos"
cleos --verbose push action nestcontn2a setp '["alice", {"first":183, "second":269}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntp '["alice"]' -p alice@active
read -p "verified action setp for pair<uint16_t, uint16_t> "  notcare

echo -e "\n**************************Starting to testing 2-layer nested containers...";

echo -e "\n****** 1. Testing set - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined set<set<T> > using cleos"
cleos --verbose push action nestcontn2a setstst '["alice", [[10,20],[3], [400,500,600]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntstst '["alice"]' -p alice@active
read -p "verified type defined setstst for set<set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined set<vector<T> > using cleos"
cleos --verbose push action nestcontn2a setstv '["alice", [[16,26],[36], [46,506,606]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntstv '["alice"]' -p alice@active
read -p "verified type defined setstv for set<vec_uint16>"  notcare

echo -e "\n\n*** nested container set<optional<T> > is supported after https://github.com/EOSIO/eos/pull/10635 is merged into eosio"
echo -e "\n\n=========================pass type-defined set<optional<T> > using cleos"
cleos --verbose push action nestcontn2a setsto '["alice", [101, null, 201, 301]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntsto '["alice"]' -p alice@active
read -p "verified type defined setvo for set<op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined set<map<K,V> > using cleos"
cleos --verbose push action nestcontn2a setstm '["alice", [ [{"first":30,"second":300},{"first":31,"second":301}], [{"first":60,"second":600},{"first":61,"second":601}] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntstm '["alice"]' -p alice@active
read -p "verified type defined setstm for set<mp_uint16>"  notcare

echo -e "\n\n=========================pass set<pair<T1,T2> > using cleos"
cleos --verbose push action nestcontn2a setstp '["alice", [{"first":68, "second":128}, {"first":69, "second":129}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntstp '["alice"]' -p alice@active
read -p "verified setstp for set<pair<uint32_t, uint32_t> >"  notcare

echo -e "\n****** 2. Testing vector - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined vector<set<T> > using cleos"
cleos --verbose push action nestcontn2a setvst '["alice", [[10,20],[3], [400,500,600]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntvst '["alice"]' -p alice@active
read -p "verified type defined setvst for vector<set_uint16>"  notcare


echo -e "\n\n=========================pass type-defined vector<vector<T> > using cleos"
cleos --verbose push action nestcontn2a setvv '["alice", [[1,2],[30], [40,50,60]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntvv '["alice"]' -p alice@active
read -p "verified type defined setvv for vector<vec_uint16>"  notcare

echo -e "\n\n*** nested container vector<optional<T> > is supported after https://github.com/EOSIO/eos/pull/10635 is merged into eosio"
echo -e "\n\n=========================pass type-defined vector<optional<T> > using cleos"
cleos --verbose push action nestcontn2a setvo '["alice", [100, null, 200, null, 300]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntvo '["alice"]' -p alice@active
read -p "verified type defined setvo for vector<op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined vector<map<K,V> > using cleos"
cleos --verbose push action nestcontn2a setvm '["alice", [ [{"first":10,"second":100},{"first":11,"second":101}], [{"first":80,"second":800},{"first":81,"second":9009}] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntvm '["alice"]' -p alice@active
read -p "verified type defined setvm for vector<mp_uint16>"  notcare

echo -e "\n\n=========================pass vector<pair<T1,T2> >using cleos"
cleos --verbose push action nestcontn2a setvp '["alice", [{"first":18, "second":28}, {"first":19, "second":29}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntvp '["alice"]' -p alice@active
read -p "verified action setvp for vector<pair<uint32_t, uint32_t> >"  notcare


echo -e "\n****** 3. Testing optional - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined optional<set<T> > using cleos"
cleos --verbose push action nestcontn2a setost '["bob", null]' -p bob@active
cleos --verbose push action nestcontn2a setost '["alice", [1006,2006, 3006]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntost '["bob"]' -p bob@active
cleos --verbose push action nestcontn2a  prntost '["alice"]' -p alice@active
read -p "verified type defined setost for optional<set_uint16>"  notcare



echo -e "\n\n=========================pass type-defined optional<vector<T> > using cleos"
cleos --verbose push action nestcontn2a setov '["bob", null]' -p bob@active
cleos --verbose push action nestcontn2a setov '["alice", [1001,2001, 3001]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntov '["bob"]' -p bob@active
cleos --verbose push action nestcontn2a  prntov '["alice"]' -p alice@active
read -p "verified type defined setov for optional<vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<optional<T> > using cleos"
cleos --verbose push action nestcontn2a setoo '["bob", null]' -p bob@active
cleos --verbose push action nestcontn2a setoo '["alice",123]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntoo '["bob"]' -p bob@active
cleos --verbose push action nestcontn2a  prntoo '["alice"]' -p alice@active
read -p "verified type defined setoo for optional<op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<map<K,V> > using cleos"
cleos --verbose push action nestcontn2a  setom '["alice",[{"first":10,"second":1000},{"first":11,"second":1001}] ]' -p alice@active
cleos --verbose push action nestcontn2a  setom '["bob", null ]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a  prntom '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a  prntom '["bob"]' -p bob@active
read -p "verified type defined setom for optional<mp_uint16>"  notcare


echo -e "\n\n=========================pass type-defined optional<pair<T1,T2> > using cleos"
cleos --verbose push action nestcontn2a setop '["alice", {"first":60, "second":61}]' -p alice@active
cleos --verbose push action nestcontn2a setop '["bob", null]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a  prntop '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a  prntop '["bob"]' -p bob@active
read -p "verified type-defined setop for optional<pr_unit16>"  notcare

echo -e "\n****** 4. Testing map - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined map<K,set<T> > using cleos"
cleos --verbose push action nestcontn2a setmst '["alice", [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntmst '["alice"]' -p alice@active
read -p "verified type defined setmst for map<uint16_t, set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K,vector<T> > using cleos"
cleos --verbose push action nestcontn2a setmv '["alice", [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntmv '["alice"]' -p alice@active
read -p "verified type defined setmv for map<uint16_t, vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K,optional<T> > using cleos"
cleos --verbose push action nestcontn2a setmo '["alice", [{"key":10,"value":1000},{"key":11,"value":null}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntmo '["alice"]' -p alice@active
read -p "verified type defined setmo for map<uint16_t, op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K1,map<K2,V> > using cleos"
cleos push action nestcontn2a setmm '["alice", [{"key":10,"value":[{"first":200,"second":2000}, {"first":201,"second":2001}] }, {"key":11,"value":[{"first":300,"second":3000}, {"first":301,"second":3001}] } ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntmm '["alice"]' -p alice@active
read -p "verified type defined setmm for map<uint16_t, mp_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K,pair<T> > using cleos"
cleos --verbose push action nestcontn2a setmp '["alice", [{"key":36,"value":{"first":300, "second":301}}, {"key":37,"value":{"first":600, "second":601}} ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntmp '["alice"]' -p alice@active
read -p "verified type-defined setmp for map<uint16_t, pr_unit16> "  notcare


echo -e "\n****** 5. Testing pair - set,vector,optional,map,pair"

echo -e "\n\n=========================pass pair<T1,set<T> > using cleos"
cleos --verbose push action nestcontn2a setpst '["alice", {"first":20, "second":[200,201,202]}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntpst '["alice"]' -p alice@active
read -p "verified type-defined setpst for pair<uint32_t, set_uint16>"  notcare

echo -e "\n\n=========================pass pair<T1,vector<T> > using cleos"
cleos --verbose push action nestcontn2a setpv '["alice", {"first":10, "second":[100,101,102]}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntpv '["alice"]' -p alice@active
read -p "verified type-defined setpv for pair<uint32_t, vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined pair<T1,optional<T> > using cleos"
cleos --verbose push action nestcontn2a setpo '["alice", {"first":70, "second":71}]' -p alice@active
cleos --verbose push action nestcontn2a setpo '["bob", {"first":70, "second":null}]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a  prntpo '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a  prntpo '["bob"]' -p bob@active
read -p "verified type-defined setpo for pair<uint32_t, op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined pair<T1,map<K,V> > using cleos"
cleos --verbose push action nestcontn2a setpm '["alice", {"key":6, "value":[{"first":20,"second":300}, {"first":21,"second":301}] }]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntpm '["alice"]' -p alice@active
read -p "verified type-defined setpm for pair<uint16_t, mp_uint16> "  notcare

echo -e "\n\n=========================pass pair<T1,pair<T2,T3> > using cleos"
cleos --verbose push action nestcontn2a setpp '["alice", {"key":30, "value":{"first":301, "second":302} }]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2a  prntpp '["alice"]' -p alice@active
read -p "verified type-defined setpp for pair<uint16_t, pr_unit16>"  notcare


echo -e "\n\n=========================pass type-defined optional<vector<vector<T> > > using cleos, which is a 3-layer nested container!"
cleos --verbose push  action nestcontn2a setovv '["alice", [[21,22],[230], [240,250,260,280]]]' -p alice@active
cleos --verbose push action nestcontn2a setovv '["bob", null]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2a  prntovv '["alice"]' -p alice@active
cleos --verbose push action nestcontn2a  prntovv '["bob"]' -p bob@active
read -p "verified type-defined setovv for optional<vector<vec_uint32>>>"  notcare

echo -e "\n\n******Use get table to retrieve ALL the fields of all the keys stored in this multi-index table ******"
echo -e "The output is in a JSON format for 3 name keys alice, bob,jane used in this script:"
read -p "there are 3 rows in the output, each row starts with data member v, ends with data member ovv" notcare
cleos get table nestcontn2a nestcontn2a people2

echo -e "\n\n******Continue to verify std::tuple<Ts...> is supported in the eosio multi-index table ******"

cd tupletest
echo -e 'Get to the directory that has tupletest.cpp\n'
# cd ~/WorkNestedContainer/nested-container/nestcontn2a/tupletest

pwd
echo -e '\n******Entered tupletest/ subdirectory to compile and then publish tupletest contract...\n'
echo -e '\n\n eosio-cpp is compiling contract tupletest, please wait...\n'
eosio-cpp tupletest.cpp
    #  eosio-cpp compiler has to have the fix of EPE-972, otherwise above command will print out errors

cleos --verbose create account eosio tupletest EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
cleos --verbose set contract tupletest ./ -p tupletest@active
read -p 'tupletest.cpp is compiled and the contract tupletest is set and loaded into block chain' notcare


##############--------- The following is related to tuple<Ts...>
echo -e "\n\n=========================pass tuple<Ts...> using cleos"
cleos --verbose push action tupletest sett '["alice", [100,"strA"]]' -p alice@active
cleos --verbose push action tupletest sett '["bob", [200, "strB"]]' -p bob@active
sleep 1
cleos --verbose push action tupletest prntt '["alice"]' -p alice@active
cleos --verbose push action tupletest prntt '["bob"]' -p bob@active
read -p "verified action sett for tuple<uint16_t, string>" notcare

echo -e "\n****** A1. Testing other containers (vector,set,optional,map,pair, tuple)  of tuples"

echo -e "\n\n=========================pass type-defined vector<tuple<Ts...> > using cleos"
cleos --verbose push action tupletest setvt '["alice", [[10,20],[30,60], [80,90]]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prntvt '["alice"]' -p alice@active
read -p "verified type defined setvt for vector<tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined set<tuple<Ts...> > using cleos"
cleos --verbose push action tupletest  setstt '["alice", [[1,2],[3,6], [8,9]]]' -p alice@active
sleep 1
cleos --verbose push action tupletest   prntstt '["alice"]' -p alice@active
read -p "verified type defined setstt for set<tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<tuple<Ts...> > using cleos"
cleos --verbose push action tupletest setot '["bob", null]' -p bob@active
cleos --verbose push action tupletest setot '["alice", [1001,2001]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prntot '["bob"]' -p bob@active
cleos --verbose push action tupletest  prntot '["alice"]' -p alice@active
read -p "verified type defined setot for optional<tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map of tuple<Ts...>  using cleos"
cleos --verbose push action tupletest setmt '["alice", [{"key":1,"value":[10,11]},  {"key":2,"value":[200,300]} ]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prntmt '["alice"]' -p alice@active
read -p "verified type defined setmt for map<uint16_t, tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined pair of tuple<Ts...>  using cleos"
cleos --verbose push action tupletest setpt '["alice", {"first":10, "second":[100,101]}]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prntpt '["alice"]' -p alice@active
read -p "verified type defined setpt for pair<uint32_t, tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of tuple<Ts...>  using cleos"
cleos --verbose push action tupletest settt '["alice", [[1,2],[30,40], [50,60]]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prnttt '["alice"]' -p alice@active
read -p "verified type defined settt for tuple<tup_uint16, tup_uint16,  tup_uint16>"  notcare

echo -e "\n\n\n****** A2. Testing tuple of other containers (vector,set,optional,map,pair)"

echo -e "\n\n=========================pass type-defined tuple of vector<T>  using cleos"
cleos --verbose push action tupletest settv '["alice", [16,[26,36], [46,506,606]]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prnttv '["alice"]' -p alice@active
read -p "verified type defined settv for tuple<uint16_t, vec_uint16, vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of set<T>  using cleos"
cleos --verbose push action tupletest settst '["alice", [10,[21,31], [41,51,61]]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prnttst '["alice"]' -p alice@active
read -p "verified type defined settst for tuple<uint16_t, set_uint16, set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of optional<T>  using cleos"
echo -e "\n*****Attention: tuple of optional<T> is supported, though vector<optional<T> >, set<optional<T> > are NOT yet supported in nestcontn2a.cpp!\n"
cleos --verbose push action tupletest  setto '["alice", [100, null, 200, null, 300]]' -p alice@active
cleos --verbose push action tupletest  setto '["bob", [null, null, 10, null, 20]]' -p bob@active
sleep 1
cleos --verbose push action tupletest  prntto '["alice"]' -p alice@active
cleos --verbose push action tupletest  prntto '["bob"]' -p bob@active
read -p "verified type defined settst for tuple<uint16_t, set_uint16, set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of map<K,V>  using cleos"
echo -e "\n*****Note: The input format of settm is different from that of setvm in nestcontn2a.cpp!\n"
cleos --verbose push action tupletest settm '["alice", [126, [{"key":10,"value":100},{"key":11,"value":101}], [{"key":80,"value":800},{"key":81,"value":9009}] ]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prnttm '["alice"]' -p alice@active
read -p "verified type defined settm for tuple<uint16_t, mp_uint16, mp_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of pair<T1,T2>  using cleos"
echo -e "\n******Note: The input format of settp is different from that of setvp in nestcontn2a.cpp!\n"
cleos --verbose push action tupletest settp '["alice", [127, {"key":18, "value":28}, {"key":19, "value":29}]]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prnttp '["alice"]' -p alice@active
read -p "verified type defined settp for tuple<uint16_t, pr_uint16, pr_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of mixed types tuple<string, vec_uint16, pr_uint16>  using cleos"
cleos --verbose push action tupletest settmisc '["alice", ["strHere", [10,11,12,16], {"key":86,"value":96}] ]' -p alice@active
sleep 1
cleos --verbose push action tupletest  prnttmisc '["alice"]' -p alice@active
read -p "verified type defined settmisc for tuple<string, vec_uint16, pr_uint16>"  notcare

echo -e "\n\n******Use get table to retrieve ALL the fields of all the keys stored in this multi-index table ******"
echo -e "The output is in a JSON format for 3 name keys alice, bob,jane used in this script:"
read -p "there are multiple rows in the output, each row starts with data member t, ends with data member tmisc" notcare
echo -e "\n\nAttention:\n"
read -p "******please check the result of the following 'cleos get table tupletest tupletest people2' to make sure tuple of optional<T> is indeed supported!" notcare
cleos get table tupletest tupletest people2
read -p "******please check above result to make sure the table output is not corrupted and fields 'to' for alice and bob are correct!" notcare

cd ..
pwd
echo -e "\n...Finished tupletest,  back to its parent directory nestcontn2a/ ... \n"
