--- kv_example Project ---

 -- How to Build with CMake and Make --
   - cd into the 'build' directory
   - run the command 'cmake ..'
   - run the command 'make'

 - After build -
   - The built smart contract is under the 'kv_example' directory in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point to the './build/kv_example' directory

 - Additions to CMake should be done to the CMakeLists.txt in the './src' directory and not in the top level CMakeLists.txt

 -- How to build with eosio-cpp --
   - cd into the 'build' directory
   - run the command 'eosio-cpp -abigen ../src/kv_example.cpp -o kv_example.wasm -I ../include/'

 - After build -
   - The built smart contract is in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point to the 'build' directory
