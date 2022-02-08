#!/bin/bash
#
# This script is used to test all the cleos commands of the EOSIO actions coded in testkvkeys.cpp,
# which is in the subdirectory tests/toolchain/build-pass/ of eosio.cdt
#
# Before running this script, make sure your default wallet is unlocked, and replace EOS6...5CV with your own development key
# In a development environment, you may need to restart nodeos
#
# Here it is assumed that your eos with the KV_DATABASE feature is installed in ~/Work/eos/
# See https://github.com/EOSIO/eos/tree/develop/contracts/enable-kv to know how to enable Key value map/table feature of nodeos
#
# It is also assumed that the current working directory is ~/WorkNestedContainer/nested-container/nestcontn2kv/testkvkeys/,
# which has testkvkeys.cpp
#
# Remark:
#   If you restart to run nodeos first in this script, followed by running enable-kv.sh,
#   make sure that there is a pause after running nodeos by adding a statement like 'sleep 2',
#   to avoid the error message curl: (7) Failed to connect to 127.0.0.1 port 8888: Connection refused
#

#========================================
# configuration section: Modify the following to suit your own environment
# Here myContractPath is the local path that has testkvkeys.cpp and this doTestKVKeys.sh
myPubKey=EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
myContractPath=/path1/to/testkvkeys/
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
# eacho -e 'After nodeos runs properly, then start enable-kv.sh ...\n'
# cd ~/Work/eos/contracts/enable-kv/
# ./enable-kv.sh -c ~/Work/eos/build/contracts/contracts/
# echo -e 'enable-kv.sh is executed, now the nodeos has the new protocol feature: KV_DATABASE ******\n\n'

cleos create account eosio alice $myPubKey
cleos create account eosio bob $myPubKey
cleos create account eosio jane $myPubKey
read -p 'test accounts alice,bob,jane created' notcare


echo -e 'Get to the directory that has this doTestKVKeys.sh script\n'
cd $myContractPath


echo -e '\n\n eosio-cpp is compiling contract testkvkeys, please wait...\n'
$eosio_cppPath testkvkeys.cpp  #compile testkvkeys.cpp to get updated .wasm and .abi
sleep 2

# always use --verbose mode to launch cleos to display multi-line output and reveal some internals
cleos --verbose create account eosio testkvkeys $myPubKey -p eosio@active
cleos --verbose set contract testkvkeys ./ -p testkvkeys@active
read -p 'testkvkeys.cpp is compiled and the contract testkvkeys is set and loaded into block chain' notcare


echo -e "\n\n=========================pass vector<T> using cleos"
cleos --verbose push action testkvkeys setv '[1,[100,200,300,600]]' -p alice@active
cleos --verbose push action testkvkeys setv '[2, []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv '[1]' -p alice@active
cleos --verbose push action testkvkeys prntv '[2]' -p bob@active
read -p "verified action setv for vector<uint16_t>" notcare



echo -e "\n\n******Use get kv_table to verify kvtb1 where the key type is int ******"
echo -e "The output is in a JSON format for 2 integer keys used in this script:"
cleos get kv_table testkvkeys kvtb1 map.index
    # get kv_table requires the primary index of a given kv::table,
    # but here kv::map rather than kv::table is used, so what's its primary key ?
    # the bottom of generated ABI file  testkvkeys.abi shows "primary_index" of kvtb55  has "name": "map.index"
read -p "there are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n****** Testing kv::map with different types of keys"

echo -e "\n\n=========================Testing when kv::map key type is std::string"
cleos --verbose push action testkvkeys setv2 '["str1",[102,202,302,602]]' -p alice@active
cleos --verbose push action testkvkeys setv2 '["str2", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv2 '["str1"]' -p alice@active
cleos --verbose push action testkvkeys prntv2 '["str2"]' -p bob@active
read -p 'verified kv::map<"kvtb2"_n, string, person2kv> when its key type is std::string'  notcare

echo -e "\n\nUse get kv_table to verify kvtb2 where the key type is std::string  ******"
echo -e "The output is in a JSON format for 2 strings:"
cleos get kv_table testkvkeys kvtb2 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::name"
cleos --verbose push action testkvkeys setv3 '["name1",[103,203,303,603]]' -p alice@active
cleos --verbose push action testkvkeys setv3 '["name2", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv3 '["name1"]' -p alice@active
cleos --verbose push action testkvkeys prntv3 '["name2"]' -p bob@active
read -p 'verified kv::map<"kvtb3"_n, name, person2kv> when its key type is eosio::name'  notcare

echo -e "\n\nUse get kv_table to verify kvtb3 where the key type is eosio::name  ******"
echo -e "The output is in a JSON format for 2 names:"
cleos get kv_table testkvkeys kvtb3 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is self-defined strut with a CDT_REFLECT wrapper"
cleos --verbose push action testkvkeys setv4 '[{"_count":18, "_strID":"dumstr"},[104,204,304,604]]' -p alice@active
cleos --verbose push action testkvkeys setv4 '[{"_count":11, "_strID":"dumstr1"}, []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv4 '[{"_count":18, "_strID":"dumstr"}]' -p alice@active
cleos --verbose push action testkvkeys prntv4 '[{"_count":11, "_strID":"dumstr1"}]' -p bob@active
echo  -e "\tC++ struct mystructrefl with no base class when serving as the key of kv::map, has following shortcut JSON input format:\n"
echo  -e "\tThe key mystructrefl has a value a little different from previous setv4, to avoid nodeos Duplicate transaction error***"
cleos --verbose push action testkvkeys setv4 '[[19, "dumstrx"],[104,204,304,604]]' -p alice@active
cleos --verbose push action testkvkeys setv4 '[[12, "dumstr1x"], []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv4 '[[19, "dumstrx"]]' -p alice@active
cleos --verbose push action testkvkeys prntv4 '[[12, "dumstr1x"]]' -p bob@active
read -p 'verified kv::map<"kvtb4"_n, mystructrefl, person2kv> when its key type is mystructrefl'  notcare


echo -e "\n\nUse get kv_table to verify kvtb4 where the key type is self-defined mystruct with a CDT_REFLECT  wrapper******"
echo -e "The output is in a JSON format for 4 instances of mystructrefl:"
cleos get kv_table testkvkeys kvtb4 map.index
read -p "There are 4 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::asset"
cleos --verbose push action testkvkeys setv5 '["1236 DOLLAR",[105,205,305,605]]' -p alice@active
cleos --verbose push action testkvkeys setv5 '["200.0000 SYS", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv5 '["1236 DOLLAR"]' -p alice@active
cleos --verbose push action testkvkeys prntv5 '["200.0000 SYS"]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb5"_n, eosio::asset, tbl2> when its key type is eosio::asset'  notcare


echo -e "\n\nUse get kv_table to verify kvtb5 where the key type is eosio::asset******"
cleos get kv_table testkvkeys kvtb5 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::symbol"
cleos --verbose push action testkvkeys setv11 '["18,ABCDEFG",[106,206,306,606]]' -p alice@active
cleos --verbose push action testkvkeys setv11 '["3,MSFT", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv11 '["18,ABCDEFG"]' -p alice@active
cleos --verbose push action testkvkeys prntv11 '["3,MSFT"]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb11"_n, eosio::symbol, tbl2> when its key type is eosio::symbol'  notcare


echo -e "\n\nUse get kv_table to verify kvtb11 where the key type is eosio::symbol******"
cleos get kv_table testkvkeys kvtb11 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::symbol_code"
cleos --verbose push action testkvkeys setv12 '["ABCDEFG",[107,207,307,607]]' -p alice@active
cleos --verbose push action testkvkeys setv12 '["MSFT", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv12 '["ABCDEFG"]' -p alice@active
cleos --verbose push action testkvkeys prntv12 '["MSFT"]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb12"_n, eosio::symbol_code, tbl2> when its key type is eosio::symbol_code'  notcare


echo -e "\n\nUse get kv_table to verify kvtb12 where the key type is eosio::symbol_code******"
cleos get kv_table testkvkeys kvtb12 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::time_point"
cleos --verbose push action testkvkeys setv13 '["2021-07-16T19:20:30.987",[108,208,308,608]]' -p alice@active
cleos --verbose push action testkvkeys setv13 '["2021-09-16T00:18:36.306", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv13 '["2021-07-16T19:20:30.987"]' -p alice@active
cleos --verbose push action testkvkeys prntv13 '["2021-09-16T00:18:36.306"]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb13"_n, eosio::time_point, tbl2> when its key type is eosio::time_point'  notcare


echo -e "\n\nUse get kv_table to verify kvtb13 where the key type is eosio::time_point******"
cleos get kv_table testkvkeys kvtb13 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::time_point_sec"
cleos --verbose push action testkvkeys setv14 '["2021-07-16T19:20:30",[109,209,309,609]]' -p alice@active
cleos --verbose push action testkvkeys setv14 '["2021-09-16T00:18:36", []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv14 '["2021-07-16T19:20:30"]' -p alice@active
cleos --verbose push action testkvkeys prntv14 '["2021-09-16T00:18:36"]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb14"_n, eosio::time_point_sec, tbl2> when its key type is eosio::time_point_sec'  notcare


echo -e "\n\nUse get kv_table to verify kvtb14 where the key type is eosio::time_point_sec******"
cleos get kv_table testkvkeys kvtb14 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare


echo -e "\n\n=========================Testing when kv::map key type is eosio::extended_symbol"
echo -e "*** eosio::extended_symbol is NOT a built-in-type of eos/ or eosio.cdt, its JSON input format is like that of self-defined mystructrefl ***\n"
  #  eosio::extended_symbol  is NOT a built-in type of eos/ or eosio.cdt/, as seen in is_builtin_type() of eosio.cdt/tools/include/eosio/gen.hpp
  #  and abi_serializer::configure_built_in_types() of eos/libraries/chain/abi_serializer.cpp,
cleos --verbose push action testkvkeys setv15 '[{"sym":"18,GOOG", "contract":"ctname1"},[110,210,310,610]]' -p alice@active
cleos --verbose push action testkvkeys setv15 '[{"sym":"3,MSFT", "contract":"ctname2"},[]]' -p alice@active
sleep 1
cleos --verbose push action testkvkeys prntv15 '[{"sym":"18,GOOG", "contract":"ctname1"}]' -p alice@active
cleos --verbose push action testkvkeys prntv15 '[{"sym":"3,MSFT", "contract":"ctname2"}]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb15"_n, eosio::extended_symbol, tbl2> when its key type is eosio::extended_symbol'  notcare


echo -e "\n\nUse get kv_table to verify kvtb15 where the key type is eosio::extended_symbol******"
cleos get kv_table testkvkeys kvtb15 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::microseconds"
echo -e "*** eosio::microseconds is NOT a built-in-type of eos/ or eosio.cdt, its JSON input format is like that of self-defined mystructrefl ***\n"
  #  eosio::microseconds is like eosio::extended_symbol, NOT a built-in type in eos/ or eosio.cdt/
cleos --verbose push action testkvkeys setv21 '[{"_count":20},[111,211,311,611]]' -p alice@active
cleos --verbose push action testkvkeys setv21 '[{"_count":21},[]]' -p alice@active
sleep 1
cleos --verbose push action testkvkeys prntv21 '[{"_count":20}]' -p alice@active
cleos --verbose push action testkvkeys prntv21 '[{"_count":21}]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb21"_n, eosio::microseconds, tbl2> when its key type is eosio::microseconds'  notcare


echo -e "\n\nUse get kv_table to verify kvtb21 where the key type is eosio::microseconds******"
cleos get kv_table testkvkeys kvtb21 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is eosio::extended_asset"
cleos --verbose push action testkvkeys setv22 '[{"quantity":"236.0000 SYS", "contract":"ctname1"},[112,212,312,612]]' -p alice@active
cleos --verbose push action testkvkeys setv22 '[{"quantity":"321 DOALLAR", "contract":"ctname2"},[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv22 '[{"quantity":"236.0000 SYS", "contract":"ctname1"}]' -p alice@active
cleos --verbose push action testkvkeys prntv22 '[{"quantity":"321 DOALLAR", "contract":"ctname2"}]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb22"_n, eosio::extended_asset, tbl2>  when its key type is eosio::extended_asset'  notcare


echo -e "\n\nUse get kv_table to verify kvtb22 where the key type is eosio::extended_asset******"
cleos get kv_table testkvkeys kvtb22 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare


echo -e "\n\n=========================Testing when kv::map key type is eosio::block_timestamp "
cleos --verbose push action testkvkeys setv23 '["2000-01-01T00:00:00.500",[116,216,316,616]]' -p alice@active
cleos --verbose push action testkvkeys setv23 '["2021-01-21T18:31:28.000",[]]' -p alice@active
sleep 1
cleos --verbose push action testkvkeys prntv23 '["2000-01-01T00:00:00.500"]' -p alice@active
cleos --verbose push action testkvkeys prntv23 '["2021-01-21T18:31:28.000"]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb23"_n, eosio::block_timestamp , tbl2>  when its key type is eosio::block_timestamp '  notcare


echo -e "\n\nUse get kv_table to verify kvtb23 where the key type is eosio::block_timestamp ******"
cleos get kv_table testkvkeys kvtb23 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n********* Test the cases where the key of kv::map is a STL container or nested container****************** "

echo -e "\n\n=========================Testing when kv::map key type is std::optional<T> "
cleos --verbose push action testkvkeys setv24 '[null,[117,217,317,617]]' -p alice@active
cleos --verbose push action testkvkeys setv24 '[12,[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv24 '[null]' -p alice@active
cleos --verbose push action testkvkeys prntv24 '[12]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb24"_n,  std::optional<uint16_t>, tbl2>  when its key type is std::optional<uint16_t>'  notcare


echo -e "\n\nUse get kv_table to verify kvtb24 where the key type is std::optional<T>  ******"
cleos get kv_table testkvkeys kvtb24 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is std::vector<T> "
cleos --verbose push action testkvkeys setv25 '[[11,21],[118,218,318,618]]' -p alice@active
cleos --verbose push action testkvkeys setv25 '[[12,22,32],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv25 '[[11,21]]' -p alice@active
cleos --verbose push action testkvkeys prntv25 '[[12,22,32]]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb25"_n,  std::vector<uint16_t>, tbl2>  when its key type is std::vector<uint16_t>'  notcare

echo -e "\n\nUse get kv_table to verify kvtb25 where the key type is std::vector<T>  ******"
cleos get kv_table testkvkeys kvtb25 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is std::set<T> "
cleos --verbose push action testkvkeys setv25a '[[1,2],[318,328,338,638]]' -p alice@active
cleos --verbose push action testkvkeys setv25a '[[10,20,30],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv25a '[[1,2]]' -p alice@active
cleos --verbose push action testkvkeys prntv25a '[[10,20,30]]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb25a"_n,  std::set<uint16_t>, tbl2>  when its key type is std::set<uint16_t>'  notcare

echo -e "\n\nUse get kv_table to verify kvtb25a where the key type is std::set<T>  ******"
cleos get kv_table testkvkeys kvtb25a map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is std::list<T> "
cleos --verbose push action testkvkeys setv35 '[[1,2],[158,258,358,658]]' -p alice@active
cleos --verbose push action testkvkeys setv35 '[[10,20,30],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv35 '[[1,2]]' -p alice@active
cleos --verbose push action testkvkeys prntv35 '[[10,20,30]]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb35"_n, std::list<uint16_t>, tbl2> when its key type is std::list<uint16_t>'  notcare

echo -e "\n\nUse get kv_table to verify kvtb35 where the key type is std::list<T>  ******"
cleos get kv_table testkvkeys kvtb35 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is std::deque<T> "
cleos --verbose push action testkvkeys setv32 '[[10,11],[120,220,320,620]]' -p alice@active
cleos --verbose push action testkvkeys setv32 '[[20,21,22],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv32 '[[10,11]]' -p alice@active
cleos --verbose push action testkvkeys prntv32 '[[20,21,22]]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb32"_n,  std::deque<uint16_t>, tbl2>  when its key type is std::deque<uint16_t>'  notcare

echo -e "\n\nUse get kv_table to verify kvtb32 where the key type is std::deque<T>  ******"
cleos get kv_table testkvkeys kvtb32 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare


echo -e "\n\n=========================Testing when kv::map key type is std::map<K,V> "
cleos --verbose push action testkvkeys setv31 '[[{"key":11,"value":101}, {"key":12,"value":102}],[119,219,319,619]]' -p alice@active
cleos --verbose push action testkvkeys setv31 '[[{"key":21,"value":201}, {"key":22,"value":202}],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv31 '[[{"key":11,"value":101}, {"key":12,"value":102}]]' -p alice@active
cleos --verbose push action testkvkeys prntv31 '[[{"key":21,"value":201}, {"key":22,"value":202}]]' -p bob@active
echo  -e "\tstd::map<K,V> when serving as the key of kv::map, has following shortcut JSON input format:\n"
cleos --verbose push action testkvkeys setv31 '[[[16,106], [17,107]],[119,219,319,619]]' -p alice@active
cleos --verbose push action testkvkeys setv31 '[[[26,206], [23,203]],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv31 '[[ [16,106], [17,107] ]]' -p alice@active
cleos --verbose push action testkvkeys prntv31 '[[[26,206], [23,203]]]' -p bob@active
read -p 'verified  eosio::kv::map<"kvtb31"_n, std::map<uint16_t, uint16_t>, tbl2> when its key type is std::map<uint16_t, uint16_t>'  notcare

echo -e "\n\nUse get kv_table to verify kvtb31 where the key type is std::map<K,V>  ******"
cleos get kv_table testkvkeys kvtb31 map.index
read -p "There are 4 rows in the output, each row starts with data member v" notcare


echo -e "\n********* Test the cases where the key of kv::map is a TWO-layer nested container****************** "

echo -e "\n\n=========================Testing when kv::map key type is std::vector<vector<T> > "
cleos --verbose push action testkvkeys setv33 '[[[1,2],[30], [40,50,60]],[126,226,326,626]]' -p alice@active
cleos --verbose push action testkvkeys setv33 '[[[11,21],[31], [41,51,61]],[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv33 '[[[1,2],[30], [40,50,60]]]' -p alice@active
cleos --verbose push action testkvkeys prntv33 '[[[11,21],[31], [41,51,61]]]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb33"_n, std::vector<vec_uint16>, tbl2>  when its key type is std::vector<vector<T> >'  notcare

echo -e "\n\nUse get kv_table to verify kvtb33 where the key type is std::vector<vector<T> >  ******"
cleos get kv_table testkvkeys kvtb33 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare

echo -e "\n\n=========================Testing when kv::map key type is std::optional<vector<T> > "
cleos --verbose push action testkvkeys setv34 '[[11, 21],[127,227,327,627]]' -p alice@active
cleos --verbose push action testkvkeys setv34 '[null,[]]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv34 '[[11, 21]]' -p alice@active
cleos --verbose push action testkvkeys prntv34 '[null]' -p bob@active
read -p 'verified eosio::kv::map<"kvtb34"_n, std::optional<vec_uint16>, tbl2>  when its key type is std::optional<vector<T> >'  notcare

echo -e "\n\nUse get kv_table to verify kvtb34 where the key type is std::optional<vector<T> >  ******"
cleos get kv_table testkvkeys kvtb34 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare


echo -e "\n********* Test the cases where the key of kv::map is complex struct ****************** "

echo -e "\n\n=========================Testing when kv::map key type is a struct of struct mystructrefl2"
cleos --verbose push action testkvkeys setv41 '[{"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"},[118,218,318,618]]' -p alice@active
cleos --verbose push action testkvkeys setv41 '[{"_structfld":{"_count":19, "_strID":"dumstrONE"}, "_strID2":"dumstrTWO2"}, []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv41 '[{"_structfld":{"_count":18, "_strID":"dumstr"}, "_strID2":"dumstr2"}]' -p alice@active
cleos --verbose push action testkvkeys prntv41 '[{"_structfld":{"_count":19, "_strID":"dumstrONE"}, "_strID2":"dumstrTWO2"}]' -p bob@active
echo  -e "\tC++ complex struct mystructrefl2 with no base class when serving as the key of kv::map, has following shortcut JSON input format:\n"
cleos --verbose push action testkvkeys setv41 '[[ [28, "dumstrx"], "dumstr2x"],[118,218,318,618]]' -p alice@active
cleos --verbose push action testkvkeys setv41 '[[ [29, "dumstrONEx"], "dumstrTWO2x"], []]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntv41 '[[ [28, "dumstrx"], "dumstr2x"]]' -p alice@active
cleos --verbose push action testkvkeys prntv41 '[[ [29, "dumstrONEx"], "dumstrTWO2x"]]' -p bob@active
read -p 'verified  eosio::kv::map<"kvtb41"_n, mystructrefl2, tbl2> when its key type is mystructrefl2'  notcare


echo -e "\n\nUse get kv_table to verify kvtb41 where the key type is mystructrefl2  ******"
cleos get kv_table testkvkeys kvtb41 map.index
read -p "There are 4 rows in the output, each row starts with data member v" notcare

echo -e "\n********* Test the case where CDT_REFLECT(...) is used in the VALUE part (not the key  part)of kv::map****************** "
cleos --verbose push action testkvkeys setvals55 '[11,"name1", "1236 DOLLAR", {"_count":26, "_strID":"dumstr1"}]' -p alice@active
cleos --verbose push action testkvkeys setvals55 '[12,"name2", "120.000 SYS", {"_count":27, "_strID":"dumstr2"}]' -p bob@active
sleep 1
cleos --verbose push action testkvkeys prntvals55 '[11]' -p alice@active
cleos --verbose push action testkvkeys prntvals55 '[12]' -p bob@active
read -p 'verified  eosio::kv::map<"kvtb55"_n, int, tbl2> when reading/writing the VALUE part which has fields wrapping CDT_REFLECT'  notcare


echo -e "\n\nUse get kv_table to verify kvtb55 when CDT_REFLECT(...) is used in the VALUE part (not the key  part)of kv::map ******"
cleos get kv_table testkvkeys kvtb55 map.index
read -p "There are 2 rows in the output, each row starts with data member v" notcare
