### About the scripts

The following 3 Bash scripts are used to collectively test all the cleos commands listed in the comments of the 5 related .cpp files, which are in the *tests/toolchain/build-pass/* subdirectory of eosio.cdt repository:

* The *[doTestKVKeys.sh](./scripts/doTestKVKeys.sh)* file which is the script to test all cleos commands related to *testkvkeys.cpp*
* The *[doNestContainer.sh](./sripts/doNestContainer.sh)* file script which tests all cleos commands related to *nestcontn2a.cpp* and *tupletest.cpp*
* The *[doNestContainerKV.sh]()./scripts/doNestContainerKV.sh)* file script which tests all cleos commands related to *nestcontn2kv.cpp* and *tupletestkv.cpp*
