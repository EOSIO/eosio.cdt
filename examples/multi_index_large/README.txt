--- multi_index_large Project ---

 -- How to Build with CMake and Make --
   - cd into the 'build' directory
   - run the command 'cmake ..'
   - run the command 'make'

 - After build -
   - The built smart contract is under the 'multi_index_large' directory in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point to the './build/multi_index_large' directory

 - Additions to CMake should be done to the CMakeLists.txt in the './src' directory and not in the top level CMakeLists.txt

 -- How to build with eosio-cpp --
   - cd into the 'build' directory
   - run the command 'eosio-cpp -abigen ../src/multi_index_large.cpp -o multi_index_large.wasm -I ../include/'

 - After build -
   - The built smart contract is in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point to the 'build' directory

cleos create account eosio testtaba your_public_key
cleos set contract testtaba multi_index_large_wasm_directory multi_index_large.wasm multi_index_large.abi -p testtaba

- example cleos actions
cleos push action testtaba set '{"id":1, "u64": 1, "u128":"0x00000000000000000000000000000000E", "f64":1.1, "f128":"0x00000000000000000000000000000001", "i128":"0x000000000000000000000000000000001", "chk256":"0987654321ABCDEF0987654321FFFF1234567890ABCDEF001234567890ABCDE0"}' -p testtaba
cleos push action testtaba set '{"id":2, "u64": 2, "u128":"0x00000000000000000000000000000001E", "f64":2.1, "f128":"0x00000000000000000000000000000002", "i128":"0x000000000000000000000000000000002", "chk256":"0987654321ABCDEF0987654321FFFF1234567890ABCDEF001234567890ABCDE1"}' -p testtaba
testtaba
cleos push action testtaba print '{"id":1}' -p testtaba
cleos push action testtaba print '{"id":2}' -p testtaba
cleos push action testtaba mod '{"id":2, "u64": 22, "u128":"0x00000000000000000000000000000021E", "f64":20.1, "f128":"0x00000000000000000000000000000021", "i128":"0x000000000000000000000000000000022", "chk256":"0987654321ABCDEF0987654321FFFF1234567890ABCDEF001234567890ABCDE1"}' -p testtaba
cleos push action testtaba print '{"id":2}' -p testtaba
cleos push action testtaba del '{"id":2}' -p testtaba
cleos push action testtaba print '{"id":2}' -p testtaba
cleos push action testtaba byf '{"f64":5.1}' -p testtaba
cleos push action testtaba byff '{"f128":"0x00000000000000000000000000000005"}' -p testtaba
cleos push action testtaba byuuuu '{"u128":"0xF0000000000000000000000000000004E"}' -p testtaba
