#!/bin/bash
#
# This script is used to test all the cleos commands of the EOSIO actions coded in nestcontn2kv.cpp and tupletestkv.cpp,
# which are in the subdirectory tests/toolchain/build-pass/ of eosio.cdt
#
# Before running this script, make sure your default wallet is unlocked, and replace EOS6...5CV with your own development key
# In a development environment, you may need to restart nodeos
#
# Here it is assumed that your eos with the KV_DATABASE feature is installed in ~/Work/eos/
# See https://github.com/EOSIO/eos/tree/develop/contracts/enable-kv to know how to enable Key value map/table feature of nodeos
#
# It is also assumed that the current working directory is ~/WorkNestedContainer/nested-container/nestcontn2kv/,
# which has nestcontn2kv.cpp and a subdirectory tupletestkv/, the subdirectory tupletestkv/ has tupletestkv.cpp
#
# Naming Convention of the containers:
#   Each container/object is represented by one letter: v-vector, m-map, s-mystruct,o-optional, p-pair, t-tuple
#                                      with exceptions: s2 - mystruct2,  st - set
#   You can use above naming convention to search for corresponding cleos command of nested containers, e.g
#       -  'setvm'  handles vector of maps
#       -  'setost' handles optional of set
#
# Remark:
#   If you restart to run nodeos first in this script, followed by running enable-kv.sh,
#   make sure that there is a pause after running nodeos by adding a statement like 'sleep 2',
#   to avoid the error message curl: (7) Failed to connect to 127.0.0.1 port 8888: Connection refused
#

#========================================
# configuration section: Modify the following to suit your own environment
# Here myContractPath is the local path that has nestcontn2kv.cpp and this doNestContainerKV.sh
#   and also $myContractPath/tupletestkv/ has tupletestkv.cpp
myPubKey=EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
myContractPath=/path1/to/nestcontn2kv/
nodeosPath=/path2/to/nodeos
eosio_cppPath=/path3/to/eosio-cpp
#It is ok to use default cleos path
#========================================

# ===Modify the following options to run nodeos if necesssary
$nodeosPath -e -p eosio \
--plugin eosio::producer_plugin \
--plugin eosio::producer_api_plugin \
--plugin eosio::chain_api_plugin \
--plugin eosio::http_plugin \
--plugin eosio::state_history_plugin \
--disable-replay-opts \
--access-control-allow-origin='*' \
--contracts-console \
--http-validate-host=false \
--verbose-http-errors >> nodeos.log 2>&1 &

read -p 'nodeos restarted!' notcare
  # notcare means we do not care an user input, notcare is used in this script for you to pause and examine the intermediate result
echo -e '===Make sure nodeos is running properly, check nodeos.log if necessary\n'
sleep 1

# === Modify the following to run enable-kv.sh
# echo -e 'After nodeos runs properly, please start enable-kv.sh ...\n'
# cd ~/Work/eos/contracts/enable-kv/
# ./enable-kv.sh -c ~/Work/eos/build/contracts/contracts/
# echo -e 'enable-kv.sh is executed, now the nodeos has the new protocol feature: KV_DATABASE ******\n\n'

cleos create account eosio alice $myPubKey
cleos create account eosio bob $myPubKey
cleos create account eosio jane $myPubKey
read -p 'test accounts alice,bob,jane created' notcare

echo -e 'Get to the directory that has this doNestContainerKV.sh script\n'
cd $myContractPath


echo -e '\n\n eosio-cpp is compiling contract nestcontn2kv, please wait...\n'
$eosio_cppPath nestcontn2kv.cpp  #compile nestcontn2kv.cpp to get updated .wasm and .abi
sleep 2

# always use --verbose mode to launch cleos to display multi-line output and reveal some internals
cleos --verbose create account eosio nestcontn2kv $myPubKey -p eosio@active
cleos --verbose set contract nestcontn2kv ./ -p nestcontn2kv@active
read -p 'nestcontn2kv.cpp is compiled and the contract nestcontn2kv is set and loaded into block chain' notcare


echo -e "\n\n=========================pass vector<T> using cleos"
cleos --verbose push action nestcontn2kv setv '[1,[100,200,300,600]]' -p alice@active
cleos --verbose push action nestcontn2kv setv '[2, []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv prntv '[1]' -p alice@active
cleos --verbose push action nestcontn2kv prntv '[2]' -p bob@active
read -p "verified action setv for vector<uint16_t>" notcare

echo -e "\n\n=========================pass set<T> using cleos"
cleos --verbose push action nestcontn2kv setst '[1, [101,201,301]]' -p alice@active
cleos --verbose push action nestcontn2kv setst '[2, []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv prntst '[1]' -p alice@active
cleos --verbose push action nestcontn2kv prntst '[2]' -p bob@active
read -p "verified action setst for set<uint16_t>" notcare

echo -e "\n\n=========================pass simple struct using cleos"
cleos --verbose push action nestcontn2kv sets '[1, {"_count":18, "_strID":"dumstr"}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prnts '[1]' -p alice@active
echo  -e "\t*******************************************************************************************"
echo  -e "\tThe C++ struct mystruct with no base class  has following shortcut JSON input formats:"
echo  -e "\tpair is a struct with fields names first-second"
echo  -e "\tmap can be regarded as an array of structs with fields names key-value"
echo  -e "\tso the shortcut input formats are applicable to struct, map, pair related types!"
echo  -e "\tso the shortcut input formats are also applicable to the key of kv::map"
echo  -e "\t\twhen the key of kv::map is type involving struct, map, pair"
echo  -e "\t*******************************************************************************************\n"

echo  -e  "Here in this .sh script, I list normal JSON input of mystruct followed by its shortcut JSON input."
echo  -e  "In nestcontn2kv.cpp, the action data for both input formats are the same,"
echo  -e  "however in order to avoid being considered by nodeos as a Duplicate transaction error,"
read  -p  "the action data for the shortcut JSON input format is slightly different from that of normal input format..." notcare
  ## Another way to avoid Duplicate transaction error of nodoes is to put all shortcut JSON input formats after regular inputs setpp()

cleos --verbose push action nestcontn2kv sets '[1, [19,"dumstr2"]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prnts '[1]' -p alice@active
read -p "verified action sets for simple mystruct "  notcare

echo -e "\n\n=========================pass pair<T1,T2> using cleos"
cleos --verbose push action nestcontn2kv setp '[1, {"first":183, "second":269}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntp '[1]' -p alice@active
echo  -e "\tpair<T1,T2> has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setp '[1, [184, 279]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntp '[1]' -p alice@active
read -p "verified action setp for pair<uint16_t, uint16_t> "  notcare

echo -e "\n\n=========================pass map<K,V> using cleos"
cleos --verbose push action nestcontn2kv setm '[1, [{"key":"str1","value":"str1val"}, {"key":"str3","value":"str3val"}]]' -p alice@active
cleos --verbose push action nestcontn2kv setm '{"id":2, "m":[{"key":"str4", "value":"str4val"}, {"key":"str6", "value":"str6val"}]}' -p jane@active
sleep 1
cleos --verbose push action nestcontn2kv prntm '[1]' -p alice@active
cleos --verbose push action nestcontn2kv prntm '[2]' -p jane@active
echo  -e "\tmap<K,V> has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setm '[1, [ ["str1x", "str1valx"], ["str3", "str3val"] ]]' -p alice@active
cleos --verbose push action nestcontn2kv setm '[2,  [ ["str4x", "str4valx"], ["str6", "str6val"] ]]' -p jane@active
echo -e "\tThe JSON input format of an empty map is [] as below:\n"
cleos --verbose push action nestcontn2kv setm '[3, []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv prntm '[1]' -p alice@active
cleos --verbose push action nestcontn2kv prntm '[2]' -p jane@active
cleos --verbose push action nestcontn2kv prntm '[3]' -p bob@active
read -p "verified action setm for map<string,string>:alice and jane both verified"  notcare

echo -e "\n\n=========================pass struct of structs using cleos"
cleos --verbose push action nestcontn2kv sets2 '[1, {"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prnts2 '[1]' -p alice@active
echo  -e "\tstruct of structs has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv sets2 '[1, [ [19, "dumstrx"], "dumstr2"] ]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prnts2 '[1]' -p alice@active
read -p "verified action sets2 for complex mystruct2 "  notcare

echo -e "\n\n=========================pass vector<mystruct> using cleos"
cleos --verbose push action nestcontn2kv setvs '[1, [{"_count":18, "_strID":"dumstr"},{"_count":19, "_strID":"dumstr2"}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvs '[1]' -p alice@active
echo  -e "\tvector<mystruct> has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setvs '[1, [ [28, "dumstrx"],[29, "dumstr2"] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvs '[1]' -p alice@active
read -p "verified action setvs for vector<mystruct> "  notcare


echo -e "\n\n=========================pass optional<T> using cleos"
cleos --verbose push action nestcontn2kv seto '[1, null]' -p bob@active   #can pass null for optional<string>
cleos --verbose push action nestcontn2kv seto '[2,"hello strval22"]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prnto '[1]' -p bob@active
cleos --verbose push action nestcontn2kv  prnto '[2]' -p alice@active
read -p "verified action seto for null case and non-null case of optional<string> "  notcare


echo -e "\n**************************Starting to testing 2-layer nested containers...";

echo -e "\n****** 1. Testing set - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined set<set<T> > using cleos"
cleos --verbose push action nestcontn2kv setstst '[1, [[10,20],[3], [400,500,600]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntstst '[1]' -p alice@active
read -p "verified type defined setstst for set<set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined set<vector<T> > using cleos"
cleos --verbose push action nestcontn2kv setstv '[1, [[16,26],[36], [46,506,606]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntstv '[1]' -p alice@active
read -p "verified type defined setstv for set<vec_uint16>"  notcare

echo -e "\n\n*** nested container set<optional<T> > is supported after https://github.com/EOSIO/eos/pull/10635 is merged into eosio"
echo -e "\n\n=========================pass type-defined set<optional<T> > using cleos"
cleos --verbose push action nestcontn2kv setsto '[1, [101, null, 201, 301]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntsto '[1]' -p alice@active
read -p "verified type defined setvo for set<op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined set<map<K,V> > using cleos"
cleos --verbose push action nestcontn2kv setstm '[1, [ [{"key":30,"value":300},{"key":31,"value":301}], [{"key":60,"value":600},{"key":61,"value":601}] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntstm '[1]' -p alice@active
echo  -e "\tset<map<K,V> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setstm '[1, [ [ [36,306],[31,301] ], [ [68,608],[61,601] ] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntstm '[1]' -p alice@active
read -p "verified type defined setstm for set<mp_uint16>"  notcare

echo -e "\n\n=========================pass set<pair<T1,T2> > using cleos"
cleos --verbose push action nestcontn2kv setstp '[1, [{"first":68, "second":128}, {"first":69, "second":129}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntstp '[1]' -p alice@active
echo  -e "\tset<pair<T1,T2> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setstp '[1, [ [681,1281],[69,129] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntstp '[1]' -p alice@active
read -p "verified setstp for set<pair<uint32_t, uint32_t> >"  notcare

echo -e "\n****** 2. Testing vector - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined vector<set<T> > using cleos"
cleos --verbose push action nestcontn2kv setvst '[1, [[10,20],[3], [400,500,600]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvst '[1]' -p alice@active
read -p "verified type defined setvst for vector<set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined vector<vector<T> > using cleos"
cleos --verbose push action nestcontn2kv setvv '[1, [[1,2],[30], [40,50,60]]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvv '[1]' -p alice@active
read -p "verified type defined setvv for vector<vec_uint16>"  notcare

echo -e "\n\n*** nested container vector<optional<T> > is supported after https://github.com/EOSIO/eos/pull/10635 is merged into eosio"
echo -e "\n\n=========================pass type-defined vector<optional<T> > using cleos"
cleos --verbose push action nestcontn2kv setvo '[1, [100, null, 200, null, 300]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvo '[1]' -p alice@active
read -p "verified type defined setvo for vector<op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined vector<map<K,V> > using cleos"
cleos --verbose push action nestcontn2kv setvm '[1, [ [{"key":10,"value":100},{"key":11,"value":101}], [{"key":80,"value":800},{"key":81,"value":9009}] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvm '[1]' -p alice@active
echo  -e "\tvector<map<K,V> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setvm '[1, [ [ [19,1009],[11,101] ], [ [809,800],[81,9009] ] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvm '[1]' -p alice@active
read -p "verified type defined setvm for vector<mp_uint16>"  notcare

echo -e "\n\n=========================pass vector<pair<T1,T2> >using cleos"
cleos --verbose push action nestcontn2kv setvp '[1, [{"first":18, "second":28}, {"first":19, "second":29}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvp '[1]' -p alice@active
echo  -e "\tvector<pair<T1,T2> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setvp '[1, [ [118,28],[119,29] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntvp '[1]' -p alice@active
read -p "verified action setvp for vector<pair<uint32_t, uint32_t> >"  notcare


echo -e "\n****** 3. Testing optional - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined optional<set<T> > using cleos"
cleos --verbose push action nestcontn2kv setost '[1, null]' -p bob@active
cleos --verbose push action nestcontn2kv setost '[2, [1006,2006, 3006]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntost '[1]' -p bob@active
cleos --verbose push action nestcontn2kv  prntost '[2]' -p alice@active
read -p "verified type defined setost for optional<set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<vector<T> > using cleos"
cleos --verbose push action nestcontn2kv setov '[1, null]' -p bob@active
cleos --verbose push action nestcontn2kv setov '[2, [1001,2001, 3001]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntov '[1]' -p bob@active
cleos --verbose push action nestcontn2kv  prntov '[2]' -p alice@active
read -p "verified type defined setov for optional<vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<optional<T> > using cleos"
cleos --verbose push action nestcontn2kv setoo '[1, null]' -p bob@active
cleos --verbose push action nestcontn2kv setoo '[2,123]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntoo '[1]' -p bob@active
cleos --verbose push action nestcontn2kv  prntoo '[2]' -p alice@active
read -p "verified type defined setoo for optional<op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<map<K,V> > using cleos"
cleos --verbose push action nestcontn2kv  setom '[1,[{"key":10,"value":1000},{"key":11,"value":1001}] ]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntom '[1]' -p alice@active
echo  -e "\toptional<map<K,V> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv  setom '[1,[ [16,1006], [11,1001] ] ]' -p alice@active
cleos --verbose push action nestcontn2kv  setom '[2, null ]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv  prntom '[1]' -p alice@active
cleos --verbose push action nestcontn2kv  prntom '[2]' -p bob@active
read -p "verified type defined setom for optional<mp_uint16>"  notcare


echo -e "\n\n=========================pass type-defined optional<pair<T1,T2> > using cleos"
cleos --verbose push action nestcontn2kv setop '[1, {"first":60, "second":61}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntop '[1]' -p alice@active
echo  -e "\toptional<pair<T1,T2> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setop '[1, [68,69] ]' -p alice@active
cleos --verbose push action nestcontn2kv setop '[2, null]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv  prntop '[1]' -p alice@active
cleos --verbose push action nestcontn2kv  prntop '[2]' -p bob@active
read -p "verified type-defined setop for optional<pr_unit16>"  notcare

echo -e "\n****** 4. Testing map - set,vector,optional,map,pair"

echo -e "\n\n=========================pass type-defined map<K,set<T> > using cleos"
cleos --verbose push action nestcontn2kv setmst '[1, [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmst '[1]' -p alice@active
echo  -e "\tmap<K,set<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setmst '[1, [ [26,[10,11,12,16]],  [27, [200,300]] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmst '[1]' -p alice@active
read -p "verified type defined setmst for map<uint16_t, set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K,vector<T> > using cleos"
cleos --verbose push action nestcontn2kv setmv '[1, [{"key":1,"value":[10,11,12,16]},  {"key":2,"value":[200,300]} ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmv '[1]' -p alice@active
echo  -e "\tmap<K,vector<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setmv '[1, [  [28,[10,11,12,16]],  [29, [200,300]] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmv '[1]' -p alice@active
read -p "verified type defined setmv for map<uint16_t, vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K,optional<T> > using cleos"
cleos --verbose push action nestcontn2kv setmo '[1, [{"key":10,"value":1000},{"key":11,"value":null}]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmo '[1]' -p alice@active
echo  -e "\tmap<K,optional<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setmo '[1, [ [18,1000],[19,null] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmo '[1]' -p alice@active
read -p "verified type defined setmo for map<uint16_t, op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K1,map<K2,V> > using cleos"
cleos push action nestcontn2kv setmm '[1, [{"key":10,"value":[{"key":200,"value":2000}, {"key":201,"value":2001}] }, {"key":11,"value":[{"key":300,"value":3000}, {"key":301,"value":3001}] } ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmm '[1]' -p alice@active
echo  -e "\tmap<K1,map<K2,V> > has following shortcut JSON input format:\n"
cleos push action nestcontn2kv setmm '[1, [  [18, [ [200,2000], [20,2001] ] ], [19, [ [300,3000], [301,3001] ] ]  ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmm '[1]' -p alice@active
read -p "verified type defined setmm for map<uint16_t, mp_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K,pair<T> > using cleos"
cleos --verbose push action nestcontn2kv setmp '[1, [{"key":36,"value":{"first":300, "second":301}}, {"key":37,"value":{"first":600, "second":601}} ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmp '[1]' -p alice@active
echo  -e "\tmap<K,pair<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setmp '[1, [ [66, [300,301] ], [67, [600,601]] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntmp '[1]' -p alice@active
read -p "verified type-defined setmp for map<uint16_t, pr_unit16> "  notcare


echo -e "\n****** 5. Testing pair - set,vector,optional,map,pair"

echo -e "\n\n=========================pass pair<T1,set<T> > using cleos"
cleos --verbose push action nestcontn2kv setpst '[1, {"first":20, "second":[200,201,202]}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpst '[1]' -p alice@active
echo  -e "\tpair<T1,set<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setpst '[1, [26, [200,201,206] ] ]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpst '[1]' -p alice@active
read -p "verified type-defined setpst for pair<uint32_t, set_uint16>"  notcare

echo -e "\n\n=========================pass pair<T1,vector<T> > using cleos"
cleos --verbose push action nestcontn2kv setpv '[1, {"first":10, "second":[100,101,102]}]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpv '[1]' -p alice@active
echo  -e "\tpair<T1,vector<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setpv '[1, [18, [100,101,108] ] ]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpv '[1]' -p alice@active
read -p "verified type-defined setpv for pair<uint32_t, vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined pair<T1,optional<T> > using cleos"
cleos --verbose push action nestcontn2kv setpo '[1, {"first":70, "second":71}]' -p alice@active
cleos --verbose push action nestcontn2kv setpo '[2, {"first":70, "second":null}]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpo '[1]' -p alice@active
cleos --verbose push action nestcontn2kv  prntpo '[2]' -p bob@active
echo  -e "\tpair<T1,optional<T> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setpo '[1, [80,81] ]' -p alice@active
cleos --verbose push action nestcontn2kv setpo '[2, [80, null] ]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpo '[1]' -p alice@active
cleos --verbose push action nestcontn2kv  prntpo '[2]' -p bob@active
read -p "verified type-defined setpo for pair<uint32_t, op_uint16>"  notcare

echo -e "\n\n=========================pass type-defined pair<T1,map<K,V> > using cleos"
cleos --verbose push action nestcontn2kv setpm '[1, {"first":6, "second":[{"key":20,"value":300}, {"key":21,"value":301}] }]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpm '[1]' -p alice@active
echo  -e "\tpair<T1,map<K,V> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setpm '[1, [7, [[20,300], [28,308]] ] ]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpm '[1]' -p alice@active
read -p "verified type-defined setpm for pair<uint16_t, mp_uint16> "  notcare

echo -e "\n\n=========================pass pair<T1,pair<T2,T3> > using cleos"
cleos --verbose push action nestcontn2kv setpp '[1, {"first":30, "second":{"first":301, "second":302} }]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpp '[1]' -p alice@active
echo  -e "\tpair<T1,pair<T2,T3> > has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setpp '[1, [36, [307, 308] ]]' -p alice@active
sleep 1
cleos --verbose push action nestcontn2kv  prntpp '[1]' -p alice@active
read -p "verified type-defined setpp for pair<uint16_t, pr_unit16>"  notcare

echo -e "\n\n=========================pass type-defined optional<vector<vector<T> > > using cleos, which is a 3-layer nested container!"
cleos --verbose push action nestcontn2kv setovv '[1, [[21,22],[230], [240,250,260,280]]]' -p alice@active
cleos --verbose push action nestcontn2kv setovv '[2, null]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv  prntovv '[1]' -p alice@active
cleos --verbose push action nestcontn2kv  prntovv '[2]' -p bob@active
read -p "verified type-defined setovv for optional<vector<vec_uint32>>>"  notcare


echo -e "\n\n******Use get kv_table to retrieve ALL the fields of all the keys stored in this defined eosio::kv::map ******"
echo -e "The output is in a JSON format for 3 integer keys 1,2,3 used in this script:"
read -p "there are 3 rows in the output, each row starts with data member v, ends with data member ovv" notcare
cleos get kv_table nestcontn2kv people2kv map.index
    # get kv_table requires the primary index of a given kv::table,
    # but here kv::map rather than kv::table is used, so what's its primary key ?
    # the bottom of generated ABI file  nestcontn2kv.abi shows "primary_index" of people2kv  has "name": "map.index"

echo -e "\n****** Testing kv::map with different types of keys"

echo -e "\n\n=========================Testing when kv::map key type is std::string"
cleos --verbose push action nestcontn2kv setv2 '["str1",[102,202,302,602]]' -p alice@active
cleos --verbose push action nestcontn2kv setv2 '["str2", []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv prntv2 '["str1"]' -p alice@active
cleos --verbose push action nestcontn2kv prntv2 '["str2"]' -p bob@active
read -p 'verified kv::map<"people2kv2"_n, string, person2kv> when its key type is std::string'  notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::name"
cleos --verbose push action nestcontn2kv setv3 '["name1",[103,203,303,603]]' -p alice@active
cleos --verbose push action nestcontn2kv setv3 '["name2", []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv prntv3 '["name1"]' -p alice@active
cleos --verbose push action nestcontn2kv prntv3 '["name2"]' -p bob@active
read -p 'verified kv::map<"people2kv3"_n, name, person2kv> when its key type is eosio::name'  notcare

echo -e "\n\n=========================Testing when kv::map key type is self-defined strut with a CDT_REFLECT wrapper"
read -p "****You can use shortcut JSON input formats when the key of kv::map is a type involving struct,map,pair**********" notcare
cleos --verbose push action nestcontn2kv setv4 '[{"_count":18, "_strID":"dumstr"},[104,204,304,604]]' -p alice@active
cleos --verbose push action nestcontn2kv setv4 '[{"_count":11, "_strID":"dumstr1"}, []]' -p bob@active
sleep 1
cleos --verbose push action nestcontn2kv prntv4 '[{"_count":18, "_strID":"dumstr"}]' -p alice@active
cleos --verbose push action nestcontn2kv prntv4 '[{"_count":11, "_strID":"dumstr1"}]' -p bob@active
echo  -e "\tThe key is a C++ struct with no base class, it has following shortcut JSON input format:\n"
cleos --verbose push action nestcontn2kv setv4 '[[19, "dumstrx"],[104,204,304,604]]' -p alice@active
cleos --verbose push action nestcontn2kv setv4 '[[12, "dumstr1x"], []]' -p bob@active
sleep 1
    ## Note: Here both setv4 and prntv4 are modified, since their params are associated with the key of kv::map
cleos --verbose push action nestcontn2kv prntv4 '[[19, "dumstrx"]]' -p alice@active
cleos --verbose push action nestcontn2kv prntv4 '[[12, "dumstr1x"]]' -p bob@active
read -p 'verified kv::map<"people2kv4"_n, mystructrefl, person2kv> when its key type is mystructrefl'  notcare

echo -e "\n\nUse get kv_table to verify people2kv2 where the key type is std::string  ******"
echo -e "The output is in a JSON format for 2 strings:"
read -p "there are 2 rows in the output, each row starts with data member v, ends with data member ovv" notcare
cleos get kv_table nestcontn2kv people2kv2 map.index

echo -e "\n\nUse get kv_table to verify people2kv2 where the key type is eosio::name  ******"
echo -e "The output is in a JSON format for 2 names:"
read -p "there are 2 rows in the output, each row starts with data member v, ends with data member ovv" notcare
cleos get kv_table nestcontn2kv people2kv3 map.index

echo -e "\n\nUse get kv_table to verify people2kv2 where the key type is self-defined mystruct with a CDT_REFLECT  wrapper******"
echo -e "The output is in a JSON format for 4 instances of mystructrefl:"
read -p "there are 4 rows in the output, each row starts with data member v, ends with data member ovv" notcare
cleos get kv_table nestcontn2kv people2kv4 map.index

echo -e "\n\n******Continue to verify std::tuple<Ts...> is supported in the eosio kv::map table ******"

cd tupletestkv
echo -e 'Get to the directory that has tupletestkv.cpp\n'
# cd ~/WorkNestedContainer/nested-container/nestcontn2kv/tupletestkv

pwd
echo -e '\n******Entered tupletestkv/ subdirectory to compile and then publish tupletestkv contract...\n'
echo -e '\n\n eosio-cpp is compiling contract tupletestkv, please wait...\n'
$eosio_cppPath tupletestkv.cpp
    #  eosio-cpp compiler has to have the fix of EPE-972, otherwise above command will print out errors

cleos --verbose create account eosio tupletestkv $myPubKey -p eosio@active
cleos --verbose set contract tupletestkv ./ -p tupletestkv@active
read -p 'tupletestkv.cpp is compiled and the contract tupletestkv is set and loaded into block chain' notcare


##############--------- The following is related to tuple<Ts...>
echo -e "\n\n=========================pass tuple<Ts...> using cleos"
cleos --verbose push action tupletestkv sett '[1,[100,"str1"]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv prntt '[1]' -p alice@active
read -p "verified action sett for tuple<uint16_t, string>" notcare

echo -e "\n****** A1. Testing other containers (vector,set,optional,map,pair,tuple)  of tuples"

echo -e "\n\n=========================pass type-defined vector<tuple<Ts...> > using cleos"
cleos --verbose push action tupletestkv setvt '[1, [[10,20],[30,40], [50,60]]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntvt '[1]' -p alice@active
read -p "verified type defined setvt for vector<tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined set<tuple<Ts...> > using cleos"
cleos --verbose push action tupletestkv setstt '[1, [[1,2],[36,46], [56,66]]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntstt '[1]' -p alice@active
read -p "verified type defined setstt for set<tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined optional<tuple<Ts...> > using cleos"
cleos --verbose push action tupletestkv setot '[1, null]' -p bob@active
cleos --verbose push action tupletestkv setot '[2, [1001,2001]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntot '[1]' -p bob@active
cleos --verbose push action tupletestkv  prntot '[2]' -p alice@active
read -p "verified type defined setot for optional<tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined map<K, tuple<Ts...> > using cleos"
cleos --verbose push action tupletestkv setmt '[1, [{"key":1,"value":[10,11]},  {"key":2,"value":[200,300]} ]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntmt '[1]' -p alice@active
echo  -e "\tmap of tuple<Ts...> has following shortcut JSON input format:\n"
cleos --verbose push action tupletestkv setmt '[1, [[16,[10,11]],  [26,[200,300]] ]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntmt '[1]' -p alice@active
read -p "verified type defined setmt for map<uint16_t, tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined pair<T1, tuple<Ts...> > using cleos"
cleos --verbose push action tupletestkv setpt '[1, {"first":10, "second":[100,101]}]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntpt '[1]' -p alice@active
echo  -e "\tpair of tuple<Ts...> has following shortcut JSON input format:\n"
cleos --verbose push action tupletestkv setpt '[1, [16, [108,109]]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prntpt '[1]' -p alice@active
read -p "verified type defined setpt for pair<uint32_t, tup_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of tuple<Ts...>  using cleos"
cleos --verbose push action tupletestkv settt '[1, [[1,2],[30,40], [50,60]]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttt '[1]' -p alice@active
read -p "verified type defined settt for tuple<tup_uint16, tup_uint16,  tup_uint16>"  notcare

echo -e "\n\n\n****** A2. Testing tuple of other containers (vector,set,optional,map,pair)"

echo -e "\n\n=========================pass type-defined tuple of vector<T>  using cleos"
cleos --verbose push action tupletestkv settv '[1, [16,[26,36], [46,506,606]]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttv '[1]' -p alice@active
read -p "verified type defined settv for tuple<uint16_t, vec_uint16, vec_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of set<T>  using cleos"
cleos --verbose push action tupletestkv settst '[1, [10,[21,31], [41,51,61]]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttst '[1]' -p alice@active
read -p "verified type defined settst for tuple<uint16_t, set_uint16, set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of optional<T>  using cleos"
cleos --verbose push action tupletestkv  setto '[1, [100, null, 200, null, 300]]' -p alice@active
cleos --verbose push action tupletestkv  setto '[2, [null, null, 10, null, 20]]' -p bob@active
sleep 1
cleos --verbose push action tupletestkv  prntto '[1]' -p alice@active
cleos --verbose push action tupletestkv  prntto '[2]' -p bob@active
read -p "verified type defined settst for tuple<uint16_t, set_uint16, set_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of map<K,V>  using cleos"
cleos --verbose push action tupletestkv settm '[1, [126, [{"key":10,"value":100},{"key":11,"value":101}], [{"key":80,"value":800},{"key":81,"value":9009}] ]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttm '[1]' -p alice@active
echo  -e "\ttuple of map<K,V> has following shortcut JSON input format:\n"
cleos --verbose push action tupletestkv settm '[1, [226, [[10, 106],[11, 1018]], [[80,800],[81, 9009]] ]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttm '[1]' -p alice@active
read -p "verified type defined settm for tuple<uint16_t, mp_uint16, mp_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of pair<T1,T2>  using cleos"
cleos --verbose push action tupletestkv settp '[1, [127, {"first":18, "second":28}, {"first":19, "second":29}]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttp '[1]' -p alice@active
echo  -e "\ttuple of  pair<T1,T2> has following shortcut JSON input format:\n"
cleos --verbose push action tupletestkv settp '[1, [218, [16, 26], [19, 29] ]]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttp '[1]' -p alice@active
read -p "verified type defined settp for tuple<uint16_t, pr_uint16, pr_uint16>"  notcare

echo -e "\n\n=========================pass type-defined tuple of mixed types tuple<string, vec_uint16, pr_uint16>  using cleos"
cleos --verbose push action tupletestkv settmisc '[1, ["strHere", [10,11,12,16], {"first":86,"second":96}] ]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttmisc '[1]' -p alice@active
echo  -e "\ttuple<string, vec_uint16, pr_uint16>  has following shortcut JSON input format as pr_uint16 is of type pair<T1,T2>:\n"
cleos --verbose push action tupletestkv settmisc '[1, ["strHerex", [10,11,12,182], [86,961]] ]' -p alice@active
sleep 1
cleos --verbose push action tupletestkv  prnttmisc '[1]' -p alice@active
read -p "verified type defined settmisc for tuple<string, vec_uint16, pr_uint16>"  notcare

echo -e "\n\n******Use get table to retrieve ALL the fields of all the keys stored in this multi-index table ******"
echo -e "The output is in a JSON format for 3 name keys alice, bob,jane used in this script:"
read -p "there are multiple rows in the output, each row starts with data member t, ends with data member tmisc" notcare
echo -e "\n\nAttention:\n"
read -p "******please check the result of the following 'cleos get kv_table tupletestkv people2kv map.index' to make sure tuple of optional<T> is indeed supported!" notcare
cleos get kv_table tupletestkv people2kv map.index
read -p "******please check above result to make sure the table output is not corrupted and fields 'to' for alice and bob are correct!" notcare

cd ..
pwd
echo -e "\n...Finished tupletestkv,  back to its parent directory nestcontn2kv/ ... \n"
