---
search:
    keywords: ['db.h', 'db_store_i64', 'db_update_i64', 'db_remove_i64', 'db_get_i64', 'db_next_i64', 'db_previous_i64', 'db_find_i64', 'db_lowerbound_i64', 'db_upperbound_i64', 'db_end_i64', 'db_idx64_store', 'db_idx64_update', 'db_idx64_remove', 'db_idx64_next', 'db_idx64_previous', 'db_idx64_find_primary', 'db_idx64_find_secondary', 'db_idx64_lowerbound', 'db_idx64_upperbound', 'db_idx64_end', 'db_idx128_store', 'db_idx128_update', 'db_idx128_remove', 'db_idx128_next', 'db_idx128_previous', 'db_idx128_find_primary', 'db_idx128_find_secondary', 'db_idx128_lowerbound', 'db_idx128_upperbound', 'db_idx128_end', 'db_idx256_store', 'db_idx256_update', 'db_idx256_remove', 'db_idx256_next', 'db_idx256_previous', 'db_idx256_find_primary', 'db_idx256_find_secondary', 'db_idx256_lowerbound', 'db_idx256_upperbound', 'db_idx256_end', 'db_idx_double_store', 'db_idx_double_update', 'db_idx_double_remove', 'db_idx_double_next', 'db_idx_double_previous', 'db_idx_double_find_primary', 'db_idx_double_find_secondary', 'db_idx_double_lowerbound', 'db_idx_double_upperbound', 'db_idx_double_end', 'db_idx_long_double_store', 'db_idx_long_double_update', 'db_idx_long_double_remove', 'db_idx_long_double_next', 'db_idx_long_double_previous', 'db_idx_long_double_find_primary', 'db_idx_long_double_find_secondary', 'db_idx_long_double_lowerbound', 'db_idx_long_double_upperbound', 'db_idx_long_double_end']
---

# file db.h

**[Go to the source code of this file.](db_8h_source.md)**
Defines C API for interfacing with blockchain database. [More...](#detailed-description)
|Type|Name|
|-----|-----|
|int32\_t|[**db\_store\_i64**](group__database_ga3f259dd4be6adc25d95e897b7724bbc4.md#ga3f259dd4be6adc25d95e897b7724bbc4) (uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, uint64\_t id, const void \* data, uint32\_t len) <br>Store a record in a primary 64-bit integer index table. |
|void|[**db\_update\_i64**](group__database_gadfdbc05600fee5ea9d0741fa9ba7cc04.md#gadfdbc05600fee5ea9d0741fa9ba7cc04) (int32\_t iterator, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const void \* data, uint32\_t len) <br>Update a record in a primary 64-bit integer index table. |
|void|[**db\_remove\_i64**](group__database_ga3b3d2cb9baaffcca277aceafe15f885f.md#ga3b3d2cb9baaffcca277aceafe15f885f) (int32\_t iterator) <br>Remove a record from a primary 64-bit integer index table. |
|int32\_t|[**db\_get\_i64**](group__database_gae030353968fb545531c5286da8778808.md#gae030353968fb545531c5286da8778808) (int32\_t iterator, const void \* data, uint32\_t len) <br>Get a record in a primary 64-bit integer index table. |
|int32\_t|[**db\_next\_i64**](group__database_gab8a30f733c6cd6c9111f25adae685899.md#gab8a30f733c6cd6c9111f25adae685899) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row following the referenced table row in a primary 64-bit integer index table. |
|int32\_t|[**db\_previous\_i64**](group__database_ga2ffe62dd0ab43732ccebc441a2186139.md#ga2ffe62dd0ab43732ccebc441a2186139) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row preceding the referenced table row in a primary 64-bit integer index table. |
|int32\_t|[**db\_find\_i64**](group__database_gaa1f2c0990c948f52c5d9d2b7ace3e9c5.md#gaa1f2c0990c948f52c5d9d2b7ace3e9c5) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint64\_t id) <br>Find a table row in a primary 64-bit integer index table by primary key. |
|int32\_t|[**db\_lowerbound\_i64**](group__database_ga817277bc1a591237ac20e2c6a64242a4.md#ga817277bc1a591237ac20e2c6a64242a4) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint64\_t id) <br>Find the table row in a primary 64-bit integer index table that matches the lowerbound condition for a given primary key. |
|int32\_t|[**db\_upperbound\_i64**](group__database_gabca89e9c328d8ee50c35f93c6908cbf0.md#gabca89e9c328d8ee50c35f93c6908cbf0) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint64\_t id) <br>Find the table row in a primary 64-bit integer index table that matches the upperbound condition for a given primary key. |
|int32\_t|[**db\_end\_i64**](group__database_gab755d000538cddee84abbae2ed06f821.md#gab755d000538cddee84abbae2ed06f821) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table) <br>Get an iterator representing just-past-the-end of the last table row of a primary 64-bit integer index table. |
|int32\_t|[**db\_idx64\_store**](group__database_gaf00f39b0c943e762cd13ae1d73172261.md#gaf00f39b0c943e762cd13ae1d73172261) (uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, uint64\_t id, const uint64\_t \* secondary) <br>Store an association of a 64-bit integer secondary key to a primary key in a secondary 64-bit integer index table. |
|void|[**db\_idx64\_update**](group__database_ga118d635a2600ed798478ad829f36abac.md#ga118d635a2600ed798478ad829f36abac) (int32\_t iterator, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const uint64\_t \* secondary) <br>Update an association for a 64-bit integer secondary key to a primary key in a secondary 64-bit integer index table. |
|void|[**db\_idx64\_remove**](group__database_gaae117d50bba68c7a1f3b8f8828dc1699.md#gaae117d50bba68c7a1f3b8f8828dc1699) (int32\_t iterator) <br>Remove a table row from a secondary 64-bit integer index table. |
|int32\_t|[**db\_idx64\_next**](group__database_ga5fc70218dc661865823e57055801df24.md#ga5fc70218dc661865823e57055801df24) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row following the referenced table row in a secondary 64-bit integer index table. |
|int32\_t|[**db\_idx64\_previous**](group__database_ga243746a77546a54ce85345a537872cb7.md#ga243746a77546a54ce85345a537872cb7) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row preceding the referenced table row in a secondary 64-bit integer index table. |
|int32\_t|[**db\_idx64\_find\_primary**](group__database_ga4dcc0485335856945baebf6b5e246c81.md#ga4dcc0485335856945baebf6b5e246c81) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint64\_t \* secondary, uint64\_t primary) <br>Find a table row in a secondary 64-bit integer index table by primary key. |
|int32\_t|[**db\_idx64\_find\_secondary**](group__database_ga87ebf49c63616d8f4db298daea4fb1ec.md#ga87ebf49c63616d8f4db298daea4fb1ec) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, const uint64\_t \* secondary, uint64\_t \* primary) <br>Find a table row in a secondary 64-bit integer index table by secondary key. |
|int32\_t|[**db\_idx64\_lowerbound**](group__database_ga6a94c6bbb0cc74ca76c6f0fd56c03e2a.md#ga6a94c6bbb0cc74ca76c6f0fd56c03e2a) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint64\_t \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary 64-bit integer index table that matches the lowerbound condition for a given secondary key. |
|int32\_t|[**db\_idx64\_upperbound**](group__database_gacf9f8b130509755bb05cc28b974caca4.md#gacf9f8b130509755bb05cc28b974caca4) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint64\_t \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary 64-bit integer index table that matches the upperbound condition for a given secondary key. |
|int32\_t|[**db\_idx64\_end**](group__database_ga9596d2d4adea77798e08e7f3a5ac7586.md#ga9596d2d4adea77798e08e7f3a5ac7586) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table) <br>Get an end iterator representing just-past-the-end of the last table row of a secondary 64-bit integer index table. |
|int32\_t|[**db\_idx128\_store**](group__database_ga39cc50eac1593784a3c82a5a54d50d4b.md#ga39cc50eac1593784a3c82a5a54d50d4b) (uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, uint64\_t id, const uint128\_t \* secondary) <br>Store an association of a 128-bit integer secondary key to a primary key in a secondary 128-bit integer index table. |
|void|[**db\_idx128\_update**](group__database_ga97763f770a398b600a160ea69ed17cc3.md#ga97763f770a398b600a160ea69ed17cc3) (int32\_t iterator, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const uint128\_t \* secondary) <br>Update an association for a 128-bit integer secondary key to a primary key in a secondary 128-bit integer index table. |
|void|[**db\_idx128\_remove**](group__database_ga60e66048c40043424581fe8e507a353b.md#ga60e66048c40043424581fe8e507a353b) (int32\_t iterator) <br>Remove a table row from a secondary 128-bit integer index table. |
|int32\_t|[**db\_idx128\_next**](group__database_gaac074a8b2d42865e7ca1b7932e234655.md#gaac074a8b2d42865e7ca1b7932e234655) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row following the referenced table row in a secondary 128-bit integer index table. |
|int32\_t|[**db\_idx128\_previous**](group__database_gae56365d133e8c27ba709121ed1ad0db9.md#gae56365d133e8c27ba709121ed1ad0db9) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row preceding the referenced table row in a secondary 128-bit integer index table. |
|int32\_t|[**db\_idx128\_find\_primary**](group__database_ga0c371690e732d2f18e7037e038d4685d.md#ga0c371690e732d2f18e7037e038d4685d) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint128\_t \* secondary, uint64\_t primary) <br>Find a table row in a secondary 128-bit integer index table by primary key. |
|int32\_t|[**db\_idx128\_find\_secondary**](group__database_ga157160bb9692e0f1f6a98337e751f713.md#ga157160bb9692e0f1f6a98337e751f713) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, const uint128\_t \* secondary, uint64\_t \* primary) <br>Find a table row in a secondary 128-bit integer index table by secondary key. |
|int32\_t|[**db\_idx128\_lowerbound**](group__database_ga32e8a5092c3ad1b31b512e2b240fc8a8.md#ga32e8a5092c3ad1b31b512e2b240fc8a8) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint128\_t \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary 128-bit integer index table that matches the lowerbound condition for a given secondary key. |
|int32\_t|[**db\_idx128\_upperbound**](group__database_ga00bc640a4a60ac49a07d538af3cadfd8.md#ga00bc640a4a60ac49a07d538af3cadfd8) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint128\_t \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary 128-bit integer index table that matches the upperbound condition for a given secondary key. |
|int32\_t|[**db\_idx128\_end**](group__database_gad7250e200698d82294588a636356c555.md#gad7250e200698d82294588a636356c555) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table) <br>Get an end iterator representing just-past-the-end of the last table row of a secondary 128-bit integer index table. |
|int32\_t|[**db\_idx256\_store**](group__database_ga6e8c05d79f751ca250f454b5211fd149.md#ga6e8c05d79f751ca250f454b5211fd149) (uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, uint64\_t id, const uint128\_t \* data, uint32\_t data\_len) <br>Store an association of a 256-bit secondary key to a primary key in a secondary 256-bit index table. |
|void|[**db\_idx256\_update**](group__database_ga68a96c6e3358a8efa1a6c0b1ac57fbb2.md#ga68a96c6e3358a8efa1a6c0b1ac57fbb2) (int32\_t iterator, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const uint128\_t \* data, uint32\_t data\_len) <br>Update an association for a 256-bit secondary key to a primary key in a secondary 256-bit index table. |
|void|[**db\_idx256\_remove**](group__database_ga08729f61864b60499a55cc226bf9d51f.md#ga08729f61864b60499a55cc226bf9d51f) (int32\_t iterator) <br>Remove a table row from a secondary 256-bit index table. |
|int32\_t|[**db\_idx256\_next**](group__database_ga50b09ae563e297d986852b639fc569c4.md#ga50b09ae563e297d986852b639fc569c4) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row following the referenced table row in a secondary 256-bit index table. |
|int32\_t|[**db\_idx256\_previous**](group__database_ga658db41ae01fd5f9350dead6d90c301e.md#ga658db41ae01fd5f9350dead6d90c301e) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row preceding the referenced table row in a secondary 256-bit index table. |
|int32\_t|[**db\_idx256\_find\_primary**](group__database_gab3174c648f526d352dcbc013ac9c7df4.md#gab3174c648f526d352dcbc013ac9c7df4) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint128\_t \* data, uint32\_t data\_len, uint64\_t primary) <br>Find a table row in a secondary 128-bit integer index table by primary key. |
|int32\_t|[**db\_idx256\_find\_secondary**](group__database_ga32b944f43d73d2dd8f81d4a8cbf1881a.md#ga32b944f43d73d2dd8f81d4a8cbf1881a) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, const uint128\_t \* data, uint32\_t data\_len, uint64\_t \* primary) <br>Find a table row in a secondary 256-bit index table by secondary key. |
|int32\_t|[**db\_idx256\_lowerbound**](group__database_gacb2612415bf82cd5c8363ae8d82b19e7.md#gacb2612415bf82cd5c8363ae8d82b19e7) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint128\_t \* data, uint32\_t data\_len, uint64\_t \* primary) <br>Find the table row in a secondary 256-bit index table that matches the lowerbound condition for a given secondary key. |
|int32\_t|[**db\_idx256\_upperbound**](group__database_ga9be64d138ab8007da86302dc56b54f4b.md#ga9be64d138ab8007da86302dc56b54f4b) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, uint128\_t \* data, uint32\_t data\_len, uint64\_t \* primary) <br>Find the table row in a secondary 256-bit index table that matches the upperbound condition for a given secondary key. |
|int32\_t|[**db\_idx256\_end**](group__database_ga6890b5f6167a302e2b1dacdcdac24538.md#ga6890b5f6167a302e2b1dacdcdac24538) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table) <br>Get an end iterator representing just-past-the-end of the last table row of a secondary 256-bit index table. |
|int32\_t|[**db\_idx\_double\_store**](group__database_ga0a743bd3d42153ba725f8c7bff79bf2b.md#ga0a743bd3d42153ba725f8c7bff79bf2b) (uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, uint64\_t id, const double \* secondary) <br>Store an association of a double-precision floating-point secondary key to a primary key in a secondary double-precision floating-point index table. |
|void|[**db\_idx\_double\_update**](group__database_ga5652bf74b99632b57e371ba3aaa3e8db.md#ga5652bf74b99632b57e371ba3aaa3e8db) (int32\_t iterator, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const double \* secondary) <br>Update an association for a double-precision floating-point secondary key to a primary key in a secondary double-precision floating-point index table. |
|void|[**db\_idx\_double\_remove**](group__database_ga7879ef20b38df7a936152cd875307002.md#ga7879ef20b38df7a936152cd875307002) (int32\_t iterator) <br>Remove a table row from a secondary double-precision floating-point index table. |
|int32\_t|[**db\_idx\_double\_next**](group__database_ga47f20afce175cd339e73bd124b756c8f.md#ga47f20afce175cd339e73bd124b756c8f) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row following the referenced table row in a secondary double-precision floating-point index table. |
|int32\_t|[**db\_idx\_double\_previous**](group__database_ga19bc2a710628fd89639c7ffbb8c63607.md#ga19bc2a710628fd89639c7ffbb8c63607) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row preceding the referenced table row in a secondary double-precision floating-point index table. |
|int32\_t|[**db\_idx\_double\_find\_primary**](group__database_gaa53fb148f8152067cc849a3b3f660794.md#gaa53fb148f8152067cc849a3b3f660794) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, double \* secondary, uint64\_t primary) <br>Find a table row in a secondary double-precision floating-point index table by primary key. |
|int32\_t|[**db\_idx\_double\_find\_secondary**](group__database_gab2c538b679cfe4ca8e87cf076beb5f33.md#gab2c538b679cfe4ca8e87cf076beb5f33) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, const double \* secondary, uint64\_t \* primary) <br>Find a table row in a secondary double-precision floating-point index table by secondary key. |
|int32\_t|[**db\_idx\_double\_lowerbound**](group__database_gaedc57b6d44b12c4c37f13190fd9b6ef0.md#gaedc57b6d44b12c4c37f13190fd9b6ef0) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, double \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary double-precision floating-point index table that matches the lowerbound condition for a given secondary key. |
|int32\_t|[**db\_idx\_double\_upperbound**](group__database_ga7641eec51617092e0496c9522cbfd00a.md#ga7641eec51617092e0496c9522cbfd00a) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, double \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary double-precision floating-point index table that matches the upperbound condition for a given secondary key. |
|int32\_t|[**db\_idx\_double\_end**](group__database_ga29878ff539946d778ddcdf8191c421b6.md#ga29878ff539946d778ddcdf8191c421b6) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table) <br>Get an end iterator representing just-past-the-end of the last table row of a secondary double-precision floating-point index table. |
|int32\_t|[**db\_idx\_long\_double\_store**](group__database_ga86b5890f0e33b19be05965b49a31ba89.md#ga86b5890f0e33b19be05965b49a31ba89) (uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, uint64\_t id, const long double \* secondary) <br>Store an association of a quadruple-precision floating-point secondary key to a primary key in a secondary quadruple-precision floating-point index table. |
|void|[**db\_idx\_long\_double\_update**](group__database_gaf38e68c7cf06346f9e9e4d11eb02296c.md#gaf38e68c7cf06346f9e9e4d11eb02296c) (int32\_t iterator, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const long double \* secondary) <br>Update an association for a quadruple-precision floating-point secondary key to a primary key in a secondary quadruple-precision floating-point index table. |
|void|[**db\_idx\_long\_double\_remove**](group__database_ga758aaa59619b46af51e33f823c19af06.md#ga758aaa59619b46af51e33f823c19af06) (int32\_t iterator) <br>Remove a table row from a secondary quadruple-precision floating-point index table. |
|int32\_t|[**db\_idx\_long\_double\_next**](group__database_ga715571a131a495836fff374ad2beffd7.md#ga715571a131a495836fff374ad2beffd7) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row following the referenced table row in a secondary quadruple-precision floating-point index table. |
|int32\_t|[**db\_idx\_long\_double\_previous**](group__database_gadebcf8cd357d1ff5f42fa06bd44f4733.md#gadebcf8cd357d1ff5f42fa06bd44f4733) (int32\_t iterator, uint64\_t \* primary) <br>Find the table row preceding the referenced table row in a secondary quadruple-precision floating-point index table. |
|int32\_t|[**db\_idx\_long\_double\_find\_primary**](group__database_ga2e60c45b99c33117a86cd36fdb00baac.md#ga2e60c45b99c33117a86cd36fdb00baac) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, long double \* secondary, uint64\_t primary) <br>Find a table row in a secondary quadruple-precision floating-point index table by primary key. |
|int32\_t|[**db\_idx\_long\_double\_find\_secondary**](group__database_ga6b090dce51381f87ce9a9b86329d8576.md#ga6b090dce51381f87ce9a9b86329d8576) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, const long double \* secondary, uint64\_t \* primary) <br>Find a table row in a secondary quadruple-precision floating-point index table by secondary key. |
|int32\_t|[**db\_idx\_long\_double\_lowerbound**](group__database_ga3a6943d41bc35b02a9d4565174993cbd.md#ga3a6943d41bc35b02a9d4565174993cbd) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, long double \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary quadruple-precision floating-point index table that matches the lowerbound condition for a given secondary key. |
|int32\_t|[**db\_idx\_long\_double\_upperbound**](group__database_ga6a3326debe372bc025b3d12ac06e36b1.md#ga6a3326debe372bc025b3d12ac06e36b1) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table, long double \* secondary, uint64\_t \* primary) <br>Find the table row in a secondary quadruple-precision floating-point index table that matches the upperbound condition for a given secondary key. |
|int32\_t|[**db\_idx\_long\_double\_end**](group__database_ga3886b3d6d0ab2ac606c52b58c87d7546.md#ga3886b3d6d0ab2ac606c52b58c87d7546) (**[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** code, uint64\_t scope, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** table) <br>Get an end iterator representing just-past-the-end of the last table row of a secondary quadruple-precision floating-point index table. |


## Detailed Description



**Copyright**

defined in eos/LICENSE.txt 



## Functions Documentation

### function <a id="ga3f259dd4be6adc25d95e897b7724bbc4" href="#ga3f259dd4be6adc25d95e897b7724bbc4">db\_store\_i64</a>

```cpp
int32_t db.h::db_store_i64 (
    uint64_t scope,
    capi_name table,
    capi_name payer,
    uint64_t id,
    const void * data,
    uint32_t len
)
```

Store a record in a primary 64-bit integer index table. 

Store a record in a primary 64-bit integer index table


**Parameters:**


* **scope** - The scope where the table resides (implied to be within the code of the current receiver) 
* **table** - The table name 
* **payer** - The account that pays for the storage costs 
* **id** - ID of the entry 
* **data** - Record to store 
* **len** - Size of data 



**Precondition:**

`data` is a valid pointer to a range of memory at least `len` bytes long 




**Precondition:**

`\*((uint64\_t\*)data)` stores the primary key 




**Returns:**

iterator to the newly created table row 




**Post**

a new entry is created in the table 




### function <a id="gadfdbc05600fee5ea9d0741fa9ba7cc04" href="#gadfdbc05600fee5ea9d0741fa9ba7cc04">db\_update\_i64</a>

```cpp
void db.h::db_update_i64 (
    int32_t iterator,
    capi_name payer,
    const void * data,
    uint32_t len
)
```

Update a record in a primary 64-bit integer index table. 

Update a record in a primary 64-bit integer index table


**Parameters:**


* **iterator** - Iterator to the table row containing the record to update 
* **payer** - The account that pays for the storage costs (use 0 to continue using current payer) 
* **data** - New updated record 
* **len** - Size of data 



**Precondition:**

`data` is a valid pointer to a range of memory at least `len` bytes long 




**Precondition:**

`\*((uint64\_t\*)data)` stores the primary key 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the record contained in the table row pointed to by `iterator` is replaced with the new updated record 




### function <a id="ga3b3d2cb9baaffcca277aceafe15f885f" href="#ga3b3d2cb9baaffcca277aceafe15f885f">db\_remove\_i64</a>

```cpp
void db.h::db_remove_i64 (
    int32_t iterator
)
```

Remove a record from a primary 64-bit integer index table. 

Remove a record from a primary 64-bit integer index table


**Parameters:**


* **iterator** - Iterator to the table row to remove 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the table row pointed to by `iterator` is removed and the associated storage costs are refunded to the payer


Example:

```cpp
int32_t itr = db_find_i64(receiver, receiver, table1, N(alice));
eosio_assert(itr >= 0, "Alice cannot be removed since she was already not found in the table");
db_remove_i64(itr);
```

 

### function <a id="gae030353968fb545531c5286da8778808" href="#gae030353968fb545531c5286da8778808">db\_get\_i64</a>

```cpp
int32_t db.h::db_get_i64 (
    int32_t iterator,
    const void * data,
    uint32_t len
)
```

Get a record in a primary 64-bit integer index table. 

Get a record in a primary 64-bit integer index table


**Parameters:**


* **iterator** - The iterator to the table row containing the record to retrieve 
* **data** - Pointer to the buffer which will be filled with the retrieved record 
* **len** - Size of the buffer 



**Returns:**

size of the data copied into the buffer if `len > 0`, or size of the retrieved record if `len == 0`. 




**Precondition:**

`iterator` points to an existing table row in the table 




**Precondition:**

`data` is a valid pointer to a range of memory at least `len` bytes long 




**Post**

`data` will be filled with the retrieved record (truncated to the first `len` bytes if necessary)


Example:

```cpp
char value[50];
auto len = db_get_i64(itr, value, 0);
eosio_assert(len <= 50, "buffer to small to store retrieved record");
db_get_i64(itr, value, len);
```

 

### function <a id="gab8a30f733c6cd6c9111f25adae685899" href="#gab8a30f733c6cd6c9111f25adae685899">db\_next\_i64</a>

```cpp
int32_t db.h::db_next_i64 (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row following the referenced table row in a primary 64-bit integer index table. 

Find the table row following the referenced table row in a primary 64-bit integer index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the next table row 



**Returns:**

iterator to the table row following the referenced table row (or the end iterator of the table if the referenced table row is the last one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

`\*primary` will be replaced with the primary key of the table row following the referenced table row if it exists, otherwise `\*primary` will be left untouched


Example:

```cpp
int32_t charlie_itr = db_find_i64(receiver, receiver, table1, N(charlie));
// expect nothing after charlie
uint64_t prim = 0
int32_t  end_itr = db_next_i64(charlie_itr, &prim);
eosio_assert(end_itr < -1, "Charlie was not the last entry in the table");
```

 

### function <a id="ga2ffe62dd0ab43732ccebc441a2186139" href="#ga2ffe62dd0ab43732ccebc441a2186139">db\_previous\_i64</a>

```cpp
int32_t db.h::db_previous_i64 (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row preceding the referenced table row in a primary 64-bit integer index table. 

Find the table row preceding the referenced table row in a primary 64-bit integer index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the previous table row 



**Returns:**

iterator to the table row preceding the referenced table row assuming one exists (it will return -1 if the referenced table row is the first one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table or it is the end iterator of the table 




**Post**

`\*primary` will be replaced with the primary key of the table row preceding the referenced table row if it exists, otherwise `\*primary` will be left untouched


Example:

```cpp
uint64_t prim = 0;
int32_t  itr_prev = db_previous_i64(itr, &prim);
```

 

### function <a id="gaa1f2c0990c948f52c5d9d2b7ace3e9c5" href="#gaa1f2c0990c948f52c5d9d2b7ace3e9c5">db\_find\_i64</a>

```cpp
int32_t db.h::db_find_i64 (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint64_t id
)
```

Find a table row in a primary 64-bit integer index table by primary key. 

Find a table row in a primary 64-bit integer index table by primary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **id** - The primary key of the table row to look up 



**Returns:**

iterator to the table row with a primary key equal to `id` or the end iterator of the table if the table row could not be found


Example:

```cpp
int itr = db_find_i64(receiver, receiver, table1, N(charlie));
```

 

### function <a id="ga817277bc1a591237ac20e2c6a64242a4" href="#ga817277bc1a591237ac20e2c6a64242a4">db\_lowerbound\_i64</a>

```cpp
int32_t db.h::db_lowerbound_i64 (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint64_t id
)
```

Find the table row in a primary 64-bit integer index table that matches the lowerbound condition for a given primary key. 

Find the table row in a primary 64-bit integer index table that matches the lowerbound condition for a given primary key The table row that matches the lowerbound condition is the first table row in the table with the lowest primary key that is >= the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **id** - The primary key used to determine the lowerbound 



**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="gabca89e9c328d8ee50c35f93c6908cbf0" href="#gabca89e9c328d8ee50c35f93c6908cbf0">db\_upperbound\_i64</a>

```cpp
int32_t db.h::db_upperbound_i64 (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint64_t id
)
```

Find the table row in a primary 64-bit integer index table that matches the upperbound condition for a given primary key. 

Find the table row in a primary 64-bit integer index table that matches the upperbound condition for a given primary key The table row that matches the upperbound condition is the first table row in the table with the lowest primary key that is > the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **id** - The primary key used to determine the upperbound 



**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="gab755d000538cddee84abbae2ed06f821" href="#gab755d000538cddee84abbae2ed06f821">db\_end\_i64</a>

```cpp
int32_t db.h::db_end_i64 (
    capi_name code,
    uint64_t scope,
    capi_name table
)
```

Get an iterator representing just-past-the-end of the last table row of a primary 64-bit integer index table. 

Get an iterator representing just-past-the-end of the last table row of a primary 64-bit integer index table


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 



**Returns:**

end iterator of the table 




### function <a id="gaf00f39b0c943e762cd13ae1d73172261" href="#gaf00f39b0c943e762cd13ae1d73172261">db\_idx64\_store</a>

```cpp
int32_t db.h::db_idx64_store (
    uint64_t scope,
    capi_name table,
    capi_name payer,
    uint64_t id,
    const uint64_t * secondary
)
```

Store an association of a 64-bit integer secondary key to a primary key in a secondary 64-bit integer index table. 

Store an association of a 64-bit integer secondary key to a primary key in a secondary 64-bit integer index table


**Parameters:**


* **scope** - The scope where the table resides (implied to be within the code of the current receiver) 
* **table** - The table name 
* **payer** - The account that pays for the storage costs 
* **id** - The primary key to which to associate the secondary key 
* **secondary** - Pointer to the secondary key 



**Returns:**

iterator to the newly created table row 




**Post**

new secondary key association between primary key `id` and secondary key `\*secondary` is created in the secondary 64-bit integer index table 




### function <a id="ga118d635a2600ed798478ad829f36abac" href="#ga118d635a2600ed798478ad829f36abac">db\_idx64\_update</a>

```cpp
void db.h::db_idx64_update (
    int32_t iterator,
    capi_name payer,
    const uint64_t * secondary
)
```

Update an association for a 64-bit integer secondary key to a primary key in a secondary 64-bit integer index table. 

Update an association for a 64-bit integer secondary key to a primary key in a secondary 64-bit integer index table


**Parameters:**


* **iterator** - The iterator to the table row containing the secondary key association to update 
* **payer** - The account that pays for the storage costs (use 0 to continue using current payer) 
* **secondary** - Pointer to the **new** secondary key that will replace the existing one of the association 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the secondary key of the table row pointed to by `iterator` is replaced by `\*secondary` 




### function <a id="gaae117d50bba68c7a1f3b8f8828dc1699" href="#gaae117d50bba68c7a1f3b8f8828dc1699">db\_idx64\_remove</a>

```cpp
void db.h::db_idx64_remove (
    int32_t iterator
)
```

Remove a table row from a secondary 64-bit integer index table. 

Remove a table row from a secondary 64-bit integer index table


**Parameters:**


* **iterator** - Iterator to the table row to remove 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the table row pointed to by `iterator` is removed and the associated storage costs are refunded to the payer 




### function <a id="ga5fc70218dc661865823e57055801df24" href="#ga5fc70218dc661865823e57055801df24">db\_idx64\_next</a>

```cpp
int32_t db.h::db_idx64_next (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row following the referenced table row in a secondary 64-bit integer index table. 

Find the table row following the referenced table row in a secondary 64-bit integer index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the next table row 



**Returns:**

iterator to the table row following the referenced table row (or the end iterator of the table if the referenced table row is the last one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

`\*primary` will be replaced with the primary key of the table row following the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="ga243746a77546a54ce85345a537872cb7" href="#ga243746a77546a54ce85345a537872cb7">db\_idx64\_previous</a>

```cpp
int32_t db.h::db_idx64_previous (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row preceding the referenced table row in a secondary 64-bit integer index table. 

Find the table row preceding the referenced table row in a secondary 64-bit integer index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the previous table row 



**Returns:**

iterator to the table row preceding the referenced table row assuming one exists (it will return -1 if the referenced table row is the first one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table or it is the end iterator of the table 




**Post**

`\*primary` will be replaced with the primary key of the table row preceding the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="ga4dcc0485335856945baebf6b5e246c81" href="#ga4dcc0485335856945baebf6b5e246c81">db\_idx64\_find\_primary</a>

```cpp
int32_t db.h::db_idx64_find_primary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint64_t * secondary,
    uint64_t primary
)
```

Find a table row in a secondary 64-bit integer index table by primary key. 

Find a table row in a secondary 64-bit integer index table by primary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to a `uint64\_t` variable which will have its value set to the secondary key of the found table row 
* **primary** - The primary key of the table row to look up 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Returns:**

iterator to the table row with a primary key equal to `id` or the end iterator of the table if the table row could not be found 




### function <a id="ga87ebf49c63616d8f4db298daea4fb1ec" href="#ga87ebf49c63616d8f4db298daea4fb1ec">db\_idx64\_find\_secondary</a>

```cpp
int32_t db.h::db_idx64_find_secondary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    const uint64_t * secondary,
    uint64_t * primary
)
```

Find a table row in a secondary 64-bit integer index table by secondary key. 

Find a table row in a secondary 64-bit integer index table by secondary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key used to lookup the table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the first table row with a secondary key equal to `\*secondary` or the end iterator of the table if the table row could not be found 




### function <a id="ga6a94c6bbb0cc74ca76c6f0fd56c03e2a" href="#ga6a94c6bbb0cc74ca76c6f0fd56c03e2a">db\_idx64\_lowerbound</a>

```cpp
int32_t db.h::db_idx64_lowerbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint64_t * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary 64-bit integer index table that matches the lowerbound condition for a given secondary key. 

Find the table row in a secondary 64-bit integer index table that matches the lowerbound condition for a given secondary key The table row that matches the lowerbound condition is the first table row in the table with the lowest secondary key that is >= the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the lowerbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="gacf9f8b130509755bb05cc28b974caca4" href="#gacf9f8b130509755bb05cc28b974caca4">db\_idx64\_upperbound</a>

```cpp
int32_t db.h::db_idx64_upperbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint64_t * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary 64-bit integer index table that matches the upperbound condition for a given secondary key. 

Find the table row in a secondary 64-bit integer index table that matches the upperbound condition for a given secondary key The table row that matches the upperbound condition is the first table row in the table with the lowest secondary key that is > the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the upperbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga9596d2d4adea77798e08e7f3a5ac7586" href="#ga9596d2d4adea77798e08e7f3a5ac7586">db\_idx64\_end</a>

```cpp
int32_t db.h::db_idx64_end (
    capi_name code,
    uint64_t scope,
    capi_name table
)
```

Get an end iterator representing just-past-the-end of the last table row of a secondary 64-bit integer index table. 

Get an end iterator representing just-past-the-end of the last table row of a secondary 64-bit integer index table


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 



**Returns:**

end iterator of the table 




### function <a id="ga39cc50eac1593784a3c82a5a54d50d4b" href="#ga39cc50eac1593784a3c82a5a54d50d4b">db\_idx128\_store</a>

```cpp
int32_t db.h::db_idx128_store (
    uint64_t scope,
    capi_name table,
    capi_name payer,
    uint64_t id,
    const uint128_t * secondary
)
```

Store an association of a 128-bit integer secondary key to a primary key in a secondary 128-bit integer index table. 

Store an association of a 128-bit integer secondary key to a primary key in a secondary 128-bit integer index table


**Parameters:**


* **scope** - The scope where the table resides (implied to be within the code of the current receiver) 
* **table** - The table name 
* **payer** - The account that pays for the storage costs 
* **id** - The primary key to which to associate the secondary key 
* **secondary** - Pointer to the secondary key 



**Returns:**

iterator to the newly created table row 




**Post**

new secondary key association between primary key `id` and secondary key `\*secondary` is created in the secondary 128-bit integer index table 




### function <a id="ga97763f770a398b600a160ea69ed17cc3" href="#ga97763f770a398b600a160ea69ed17cc3">db\_idx128\_update</a>

```cpp
void db.h::db_idx128_update (
    int32_t iterator,
    capi_name payer,
    const uint128_t * secondary
)
```

Update an association for a 128-bit integer secondary key to a primary key in a secondary 128-bit integer index table. 

Update an association for a 128-bit integer secondary key to a primary key in a secondary 128-bit integer index table


**Parameters:**


* **iterator** - The iterator to the table row containing the secondary key association to update 
* **payer** - The account that pays for the storage costs (use 0 to continue using current payer) 
* **secondary** - Pointer to the **new** secondary key that will replace the existing one of the association 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the secondary key of the table row pointed to by `iterator` is replaced by `\*secondary` 




### function <a id="ga60e66048c40043424581fe8e507a353b" href="#ga60e66048c40043424581fe8e507a353b">db\_idx128\_remove</a>

```cpp
void db.h::db_idx128_remove (
    int32_t iterator
)
```

Remove a table row from a secondary 128-bit integer index table. 

Remove a table row from a secondary 128-bit integer index table


**Parameters:**


* **iterator** - Iterator to the table row to remove 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the table row pointed to by `iterator` is removed and the associated storage costs are refunded to the payer 




### function <a id="gaac074a8b2d42865e7ca1b7932e234655" href="#gaac074a8b2d42865e7ca1b7932e234655">db\_idx128\_next</a>

```cpp
int32_t db.h::db_idx128_next (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row following the referenced table row in a secondary 128-bit integer index table. 

Find the table row following the referenced table row in a secondary 128-bit integer index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the next table row 



**Returns:**

iterator to the table row following the referenced table row (or the end iterator of the table if the referenced table row is the last one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

`\*primary` will be replaced with the primary key of the table row following the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="gae56365d133e8c27ba709121ed1ad0db9" href="#gae56365d133e8c27ba709121ed1ad0db9">db\_idx128\_previous</a>

```cpp
int32_t db.h::db_idx128_previous (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row preceding the referenced table row in a secondary 128-bit integer index table. 

Find the table row preceding the referenced table row in a secondary 128-bit integer index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the previous table row 



**Returns:**

iterator to the table row preceding the referenced table row assuming one exists (it will return -1 if the referenced table row is the first one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table or it is the end iterator of the table 




**Post**

`\*primary` will be replaced with the primary key of the table row preceding the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="ga0c371690e732d2f18e7037e038d4685d" href="#ga0c371690e732d2f18e7037e038d4685d">db\_idx128\_find\_primary</a>

```cpp
int32_t db.h::db_idx128_find_primary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint128_t * secondary,
    uint64_t primary
)
```

Find a table row in a secondary 128-bit integer index table by primary key. 

Find a table row in a secondary 128-bit integer index table by primary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to a `uint128\_t` variable which will have its value set to the secondary key of the found table row 
* **primary** - The primary key of the table row to look up 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Returns:**

iterator to the table row with a primary key equal to `id` or the end iterator of the table if the table row could not be found 




### function <a id="ga157160bb9692e0f1f6a98337e751f713" href="#ga157160bb9692e0f1f6a98337e751f713">db\_idx128\_find\_secondary</a>

```cpp
int32_t db.h::db_idx128_find_secondary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    const uint128_t * secondary,
    uint64_t * primary
)
```

Find a table row in a secondary 128-bit integer index table by secondary key. 

Find a table row in a secondary 128-bit integer index table by secondary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key used to lookup the table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the first table row with a secondary key equal to `\*secondary` or the end iterator of the table if the table row could not be found 




### function <a id="ga32e8a5092c3ad1b31b512e2b240fc8a8" href="#ga32e8a5092c3ad1b31b512e2b240fc8a8">db\_idx128\_lowerbound</a>

```cpp
int32_t db.h::db_idx128_lowerbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint128_t * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary 128-bit integer index table that matches the lowerbound condition for a given secondary key. 

Find the table row in a secondary 128-bit integer index table that matches the lowerbound condition for a given secondary key The table row that matches the lowerbound condition is the first table row in the table with the lowest secondary key that is >= the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the lowerbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga00bc640a4a60ac49a07d538af3cadfd8" href="#ga00bc640a4a60ac49a07d538af3cadfd8">db\_idx128\_upperbound</a>

```cpp
int32_t db.h::db_idx128_upperbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint128_t * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary 128-bit integer index table that matches the upperbound condition for a given secondary key. 

Find the table row in a secondary 128-bit integer index table that matches the upperbound condition for a given secondary key The table row that matches the upperbound condition is the first table row in the table with the lowest secondary key that is > the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the upperbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="gad7250e200698d82294588a636356c555" href="#gad7250e200698d82294588a636356c555">db\_idx128\_end</a>

```cpp
int32_t db.h::db_idx128_end (
    capi_name code,
    uint64_t scope,
    capi_name table
)
```

Get an end iterator representing just-past-the-end of the last table row of a secondary 128-bit integer index table. 

Get an end iterator representing just-past-the-end of the last table row of a secondary 128-bit integer index table


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 



**Returns:**

end iterator of the table 




### function <a id="ga6e8c05d79f751ca250f454b5211fd149" href="#ga6e8c05d79f751ca250f454b5211fd149">db\_idx256\_store</a>

```cpp
int32_t db.h::db_idx256_store (
    uint64_t scope,
    capi_name table,
    capi_name payer,
    uint64_t id,
    const uint128_t * data,
    uint32_t data_len
)
```

Store an association of a 256-bit secondary key to a primary key in a secondary 256-bit index table. 

Store an association of a 256-bit secondary key to a primary key in a secondary 256-bit index table


**Parameters:**


* **scope** - The scope where the table resides (implied to be within the code of the current receiver) 
* **table** - The table name 
* **payer** - The account that pays for the storage costs 
* **id** - The primary key to which to associate the secondary key 
* **data** - Pointer to the secondary key data stored as an array of 2 `uint128\_t` integers 
* **data\_len** - Must be set to 2 



**Returns:**

iterator to the newly created table row 




**Post**

new secondary key association between primary key `id` and the specified secondary key is created in the secondary 256-bit index table 




### function <a id="ga68a96c6e3358a8efa1a6c0b1ac57fbb2" href="#ga68a96c6e3358a8efa1a6c0b1ac57fbb2">db\_idx256\_update</a>

```cpp
void db.h::db_idx256_update (
    int32_t iterator,
    capi_name payer,
    const uint128_t * data,
    uint32_t data_len
)
```

Update an association for a 256-bit secondary key to a primary key in a secondary 256-bit index table. 

Update an association for a 256-bit secondary key to a primary key in a secondary 256-bit index table


**Parameters:**


* **iterator** - The iterator to the table row containing the secondary key association to update 
* **payer** - The account that pays for the storage costs (use 0 to continue using current payer) 
* **data** - Pointer to the **new** secondary key data (which is stored as an array of 2 `uint128\_t` integers) that will replace the existing one of the association 
* **data\_len** - Must be set to 2 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the secondary key of the table row pointed to by `iterator` is replaced by the specified secondary key 




### function <a id="ga08729f61864b60499a55cc226bf9d51f" href="#ga08729f61864b60499a55cc226bf9d51f">db\_idx256\_remove</a>

```cpp
void db.h::db_idx256_remove (
    int32_t iterator
)
```

Remove a table row from a secondary 256-bit index table. 

Remove a table row from a secondary 256-bit index table


**Parameters:**


* **iterator** - Iterator to the table row to remove 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the table row pointed to by `iterator` is removed and the associated storage costs are refunded to the payer 




### function <a id="ga50b09ae563e297d986852b639fc569c4" href="#ga50b09ae563e297d986852b639fc569c4">db\_idx256\_next</a>

```cpp
int32_t db.h::db_idx256_next (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row following the referenced table row in a secondary 256-bit index table. 

Find the table row following the referenced table row in a secondary 256-bit index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the next table row 



**Returns:**

iterator to the table row following the referenced table row (or the end iterator of the table if the referenced table row is the last one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

`\*primary` will be replaced with the primary key of the table row following the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="ga658db41ae01fd5f9350dead6d90c301e" href="#ga658db41ae01fd5f9350dead6d90c301e">db\_idx256\_previous</a>

```cpp
int32_t db.h::db_idx256_previous (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row preceding the referenced table row in a secondary 256-bit index table. 

Find the table row preceding the referenced table row in a secondary 256-bit index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the previous table row 



**Returns:**

iterator to the table row preceding the referenced table row assuming one exists (it will return -1 if the referenced table row is the first one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table or it is the end iterator of the table 




**Post**

`\*primary` will be replaced with the primary key of the table row preceding the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="gab3174c648f526d352dcbc013ac9c7df4" href="#gab3174c648f526d352dcbc013ac9c7df4">db\_idx256\_find\_primary</a>

```cpp
int32_t db.h::db_idx256_find_primary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint128_t * data,
    uint32_t data_len,
    uint64_t primary
)
```

Find a table row in a secondary 128-bit integer index table by primary key. 

Find a table row in a secondary 256-bit index table by primary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **data** - Pointer to the an array of 2 `uint128\_t` integers which will act as the buffer to hold the retrieved secondary key of the found table row 
* **data\_len** - Must be set to 2 
* **primary** - The primary key of the table row to look up 



**Post**

If and only if the table row is found, the buffer pointed to by `data` will be filled with the secondary key of the found table row 




**Returns:**

iterator to the table row with a primary key equal to `id` or the end iterator of the table if the table row could not be found 




### function <a id="ga32b944f43d73d2dd8f81d4a8cbf1881a" href="#ga32b944f43d73d2dd8f81d4a8cbf1881a">db\_idx256\_find\_secondary</a>

```cpp
int32_t db.h::db_idx256_find_secondary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    const uint128_t * data,
    uint32_t data_len,
    uint64_t * primary
)
```

Find a table row in a secondary 256-bit index table by secondary key. 

Find a table row in a secondary 256-bit index table by secondary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **data** - Pointer to the secondary key data (which is stored as an array of 2 `uint128\_t` integers) used to lookup the table row 
* **data\_len** - Must be set to 2 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the first table row with a secondary key equal to the specified secondary key or the end iterator of the table if the table row could not be found 




### function <a id="gacb2612415bf82cd5c8363ae8d82b19e7" href="#gacb2612415bf82cd5c8363ae8d82b19e7">db\_idx256\_lowerbound</a>

```cpp
int32_t db.h::db_idx256_lowerbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint128_t * data,
    uint32_t data_len,
    uint64_t * primary
)
```

Find the table row in a secondary 256-bit index table that matches the lowerbound condition for a given secondary key. 

Find the table row in a secondary 256-bit index table that matches the lowerbound condition for a given secondary key The table row that matches the lowerbound condition is the first table row in the table with the lowest secondary key that is >= the given key (uses lexicographical ordering on the 256-bit keys)


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **data** - Pointer to the secondary key data (which is stored as an array of 2 `uint128\_t` integers) first used to determine the lowerbound and which is then replaced with the secondary key of the found table row 
* **data\_len** - Must be set to 2 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, the buffer pointed to by `data` will be filled with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga9be64d138ab8007da86302dc56b54f4b" href="#ga9be64d138ab8007da86302dc56b54f4b">db\_idx256\_upperbound</a>

```cpp
int32_t db.h::db_idx256_upperbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    uint128_t * data,
    uint32_t data_len,
    uint64_t * primary
)
```

Find the table row in a secondary 256-bit index table that matches the upperbound condition for a given secondary key. 

Find the table row in a secondary 256-bit index table that matches the upperbound condition for a given secondary key The table row that matches the upperbound condition is the first table row in the table with the lowest secondary key that is > the given key (uses lexicographical ordering on the 256-bit keys)


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **data** - Pointer to the secondary key data (which is stored as an array of 2 `uint128\_t` integers) first used to determine the upperbound and which is then replaced with the secondary key of the found table row 
* **data\_len** - Must be set to 2 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, the buffer pointed to by `data` will be filled with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga6890b5f6167a302e2b1dacdcdac24538" href="#ga6890b5f6167a302e2b1dacdcdac24538">db\_idx256\_end</a>

```cpp
int32_t db.h::db_idx256_end (
    capi_name code,
    uint64_t scope,
    capi_name table
)
```

Get an end iterator representing just-past-the-end of the last table row of a secondary 256-bit index table. 

Get an end iterator representing just-past-the-end of the last table row of a secondary 256-bit index table


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 



**Returns:**

end iterator of the table 




### function <a id="ga0a743bd3d42153ba725f8c7bff79bf2b" href="#ga0a743bd3d42153ba725f8c7bff79bf2b">db\_idx\_double\_store</a>

```cpp
int32_t db.h::db_idx_double_store (
    uint64_t scope,
    capi_name table,
    capi_name payer,
    uint64_t id,
    const double * secondary
)
```

Store an association of a double-precision floating-point secondary key to a primary key in a secondary double-precision floating-point index table. 

Store an association of a double-precision floating-point secondary key to a primary key in a secondary double-precision floating-point index table


**Parameters:**


* **scope** - The scope where the table resides (implied to be within the code of the current receiver) 
* **table** - The table name 
* **payer** - The account that pays for the storage costs 
* **id** - The primary key to which to associate the secondary key 
* **secondary** - Pointer to the secondary key 



**Returns:**

iterator to the newly created table row 




**Post**

new secondary key association between primary key `id` and secondary key `\*secondary` is created in the secondary double-precision floating-point index table 




### function <a id="ga5652bf74b99632b57e371ba3aaa3e8db" href="#ga5652bf74b99632b57e371ba3aaa3e8db">db\_idx\_double\_update</a>

```cpp
void db.h::db_idx_double_update (
    int32_t iterator,
    capi_name payer,
    const double * secondary
)
```

Update an association for a double-precision floating-point secondary key to a primary key in a secondary double-precision floating-point index table. 

Update an association for a double-precision floating-point secondary key to a primary key in a secondary double-precision floating-point index table


**Parameters:**


* **iterator** - The iterator to the table row containing the secondary key association to update 
* **payer** - The account that pays for the storage costs (use 0 to continue using current payer) 
* **secondary** - Pointer to the **new** secondary key that will replace the existing one of the association 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the secondary key of the table row pointed to by `iterator` is replaced by `\*secondary` 




### function <a id="ga7879ef20b38df7a936152cd875307002" href="#ga7879ef20b38df7a936152cd875307002">db\_idx\_double\_remove</a>

```cpp
void db.h::db_idx_double_remove (
    int32_t iterator
)
```

Remove a table row from a secondary double-precision floating-point index table. 

Remove a table row from a secondary double-precision floating-point index table


**Parameters:**


* **iterator** - Iterator to the table row to remove 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the table row pointed to by `iterator` is removed and the associated storage costs are refunded to the payer 




### function <a id="ga47f20afce175cd339e73bd124b756c8f" href="#ga47f20afce175cd339e73bd124b756c8f">db\_idx\_double\_next</a>

```cpp
int32_t db.h::db_idx_double_next (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row following the referenced table row in a secondary double-precision floating-point index table. 

Find the table row following the referenced table row in a secondary double-precision floating-point index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the next table row 



**Returns:**

iterator to the table row following the referenced table row (or the end iterator of the table if the referenced table row is the last one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

`\*primary` will be replaced with the primary key of the table row following the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="ga19bc2a710628fd89639c7ffbb8c63607" href="#ga19bc2a710628fd89639c7ffbb8c63607">db\_idx\_double\_previous</a>

```cpp
int32_t db.h::db_idx_double_previous (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row preceding the referenced table row in a secondary double-precision floating-point index table. 

Find the table row preceding the referenced table row in a secondary double-precision floating-point index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the previous table row 



**Returns:**

iterator to the table row preceding the referenced table row assuming one exists (it will return -1 if the referenced table row is the first one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table or it is the end iterator of the table 




**Post**

`\*primary` will be replaced with the primary key of the table row preceding the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="gaa53fb148f8152067cc849a3b3f660794" href="#gaa53fb148f8152067cc849a3b3f660794">db\_idx\_double\_find\_primary</a>

```cpp
int32_t db.h::db_idx_double_find_primary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    double * secondary,
    uint64_t primary
)
```

Find a table row in a secondary double-precision floating-point index table by primary key. 

Find a table row in a secondary double-precision floating-point index table by primary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to a `double` variable which will have its value set to the secondary key of the found table row 
* **primary** - The primary key of the table row to look up 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Returns:**

iterator to the table row with a primary key equal to `id` or the end iterator of the table if the table row could not be found 




### function <a id="gab2c538b679cfe4ca8e87cf076beb5f33" href="#gab2c538b679cfe4ca8e87cf076beb5f33">db\_idx\_double\_find\_secondary</a>

```cpp
int32_t db.h::db_idx_double_find_secondary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    const double * secondary,
    uint64_t * primary
)
```

Find a table row in a secondary double-precision floating-point index table by secondary key. 

Find a table row in a secondary double-precision floating-point index table by secondary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key used to lookup the table row 
* **primary** - Pointer to a `double` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the first table row with a secondary key equal to `\*secondary` or the end iterator of the table if the table row could not be found 




### function <a id="gaedc57b6d44b12c4c37f13190fd9b6ef0" href="#gaedc57b6d44b12c4c37f13190fd9b6ef0">db\_idx\_double\_lowerbound</a>

```cpp
int32_t db.h::db_idx_double_lowerbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    double * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary double-precision floating-point index table that matches the lowerbound condition for a given secondary key. 

Find the table row in a secondary double-precision floating-point index table that matches the lowerbound condition for a given secondary key The table row that matches the lowerbound condition is the first table row in the table with the lowest secondary key that is >= the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the lowerbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga7641eec51617092e0496c9522cbfd00a" href="#ga7641eec51617092e0496c9522cbfd00a">db\_idx\_double\_upperbound</a>

```cpp
int32_t db.h::db_idx_double_upperbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    double * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary double-precision floating-point index table that matches the upperbound condition for a given secondary key. 

Find the table row in a secondary double-precision floating-point index table that matches the upperbound condition for a given secondary key The table row that matches the upperbound condition is the first table row in the table with the lowest secondary key that is > the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the upperbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga29878ff539946d778ddcdf8191c421b6" href="#ga29878ff539946d778ddcdf8191c421b6">db\_idx\_double\_end</a>

```cpp
int32_t db.h::db_idx_double_end (
    capi_name code,
    uint64_t scope,
    capi_name table
)
```

Get an end iterator representing just-past-the-end of the last table row of a secondary double-precision floating-point index table. 

Get an end iterator representing just-past-the-end of the last table row of a secondary double-precision floating-point index table


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 



**Returns:**

end iterator of the table 




### function <a id="ga86b5890f0e33b19be05965b49a31ba89" href="#ga86b5890f0e33b19be05965b49a31ba89">db\_idx\_long\_double\_store</a>

```cpp
int32_t db.h::db_idx_long_double_store (
    uint64_t scope,
    capi_name table,
    capi_name payer,
    uint64_t id,
    const long double * secondary
)
```

Store an association of a quadruple-precision floating-point secondary key to a primary key in a secondary quadruple-precision floating-point index table. 

Store an association of a quadruple-precision floating-point secondary key to a primary key in a secondary quadruple-precision floating-point index table


**Parameters:**


* **scope** - The scope where the table resides (implied to be within the code of the current receiver) 
* **table** - The table name 
* **payer** - The account that pays for the storage costs 
* **id** - The primary key to which to associate the secondary key 
* **secondary** - Pointer to the secondary key 



**Returns:**

iterator to the newly created table row 




**Post**

new secondary key association between primary key `id` and secondary key `\*secondary` is created in the secondary quadruple-precision floating-point index table 




### function <a id="gaf38e68c7cf06346f9e9e4d11eb02296c" href="#gaf38e68c7cf06346f9e9e4d11eb02296c">db\_idx\_long\_double\_update</a>

```cpp
void db.h::db_idx_long_double_update (
    int32_t iterator,
    capi_name payer,
    const long double * secondary
)
```

Update an association for a quadruple-precision floating-point secondary key to a primary key in a secondary quadruple-precision floating-point index table. 

Update an association for a quadruple-precision floating-point secondary key to a primary key in a secondary quadruple-precision floating-point index table


**Parameters:**


* **iterator** - The iterator to the table row containing the secondary key association to update 
* **payer** - The account that pays for the storage costs (use 0 to continue using current payer) 
* **secondary** - Pointer to the **new** secondary key that will replace the existing one of the association 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the secondary key of the table row pointed to by `iterator` is replaced by `\*secondary` 




### function <a id="ga758aaa59619b46af51e33f823c19af06" href="#ga758aaa59619b46af51e33f823c19af06">db\_idx\_long\_double\_remove</a>

```cpp
void db.h::db_idx_long_double_remove (
    int32_t iterator
)
```

Remove a table row from a secondary quadruple-precision floating-point index table. 

Remove a table row from a secondary quadruple-precision floating-point index table


**Parameters:**


* **iterator** - Iterator to the table row to remove 



**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

the table row pointed to by `iterator` is removed and the associated storage costs are refunded to the payer 




### function <a id="ga715571a131a495836fff374ad2beffd7" href="#ga715571a131a495836fff374ad2beffd7">db\_idx\_long\_double\_next</a>

```cpp
int32_t db.h::db_idx_long_double_next (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row following the referenced table row in a secondary quadruple-precision floating-point index table. 

Find the table row following the referenced table row in a secondary quadruple-precision floating-point index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the next table row 



**Returns:**

iterator to the table row following the referenced table row (or the end iterator of the table if the referenced table row is the last one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table 




**Post**

`\*primary` will be replaced with the primary key of the table row following the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="gadebcf8cd357d1ff5f42fa06bd44f4733" href="#gadebcf8cd357d1ff5f42fa06bd44f4733">db\_idx\_long\_double\_previous</a>

```cpp
int32_t db.h::db_idx_long_double_previous (
    int32_t iterator,
    uint64_t * primary
)
```

Find the table row preceding the referenced table row in a secondary quadruple-precision floating-point index table. 

Find the table row preceding the referenced table row in a secondary quadruple-precision floating-point index table


**Parameters:**


* **iterator** - The iterator to the referenced table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the previous table row 



**Returns:**

iterator to the table row preceding the referenced table row assuming one exists (it will return -1 if the referenced table row is the first one in the table) 




**Precondition:**

`iterator` points to an existing table row in the table or it is the end iterator of the table 




**Post**

`\*primary` will be replaced with the primary key of the table row preceding the referenced table row if it exists, otherwise `\*primary` will be left untouched 




### function <a id="ga2e60c45b99c33117a86cd36fdb00baac" href="#ga2e60c45b99c33117a86cd36fdb00baac">db\_idx\_long\_double\_find\_primary</a>

```cpp
int32_t db.h::db_idx_long_double_find_primary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    long double * secondary,
    uint64_t primary
)
```

Find a table row in a secondary quadruple-precision floating-point index table by primary key. 

Find a table row in a secondary quadruple-precision floating-point index table by primary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to a `long double` variable which will have its value set to the secondary key of the found table row 
* **primary** - The primary key of the table row to look up 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Returns:**

iterator to the table row with a primary key equal to `id` or the end iterator of the table if the table row could not be found 




### function <a id="ga6b090dce51381f87ce9a9b86329d8576" href="#ga6b090dce51381f87ce9a9b86329d8576">db\_idx\_long\_double\_find\_secondary</a>

```cpp
int32_t db.h::db_idx_long_double_find_secondary (
    capi_name code,
    uint64_t scope,
    capi_name table,
    const long double * secondary,
    uint64_t * primary
)
```

Find a table row in a secondary quadruple-precision floating-point index table by secondary key. 

Find a table row in a secondary quadruple-precision floating-point index table by secondary key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key used to lookup the table row 
* **primary** - Pointer to a `long double` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the first table row with a secondary key equal to `\*secondary` or the end iterator of the table if the table row could not be found 




### function <a id="ga3a6943d41bc35b02a9d4565174993cbd" href="#ga3a6943d41bc35b02a9d4565174993cbd">db\_idx\_long\_double\_lowerbound</a>

```cpp
int32_t db.h::db_idx_long_double_lowerbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    long double * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary quadruple-precision floating-point index table that matches the lowerbound condition for a given secondary key. 

Find the table row in a secondary quadruple-precision floating-point index table that matches the lowerbound condition for a given secondary key The table row that matches the lowerbound condition is the first table row in the table with the lowest secondary key that is >= the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the lowerbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga6a3326debe372bc025b3d12ac06e36b1" href="#ga6a3326debe372bc025b3d12ac06e36b1">db\_idx\_long\_double\_upperbound</a>

```cpp
int32_t db.h::db_idx_long_double_upperbound (
    capi_name code,
    uint64_t scope,
    capi_name table,
    long double * secondary,
    uint64_t * primary
)
```

Find the table row in a secondary quadruple-precision floating-point index table that matches the upperbound condition for a given secondary key. 

Find the table row in a secondary quadruple-precision floating-point index table that matches the upperbound condition for a given secondary key The table row that matches the upperbound condition is the first table row in the table with the lowest secondary key that is > the given key


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 
* **secondary** - Pointer to secondary key first used to determine the upperbound and which is then replaced with the secondary key of the found table row 
* **primary** - Pointer to a `uint64\_t` variable which will have its value set to the primary key of the found table row 



**Post**

If and only if the table row is found, `\*secondary` will be replaced with the secondary key of the found table row 




**Post**

If and only if the table row is found, `\*primary` will be replaced with the primary key of the found table row 




**Returns:**

iterator to the found table row or the end iterator of the table if the table row could not be found 




### function <a id="ga3886b3d6d0ab2ac606c52b58c87d7546" href="#ga3886b3d6d0ab2ac606c52b58c87d7546">db\_idx\_long\_double\_end</a>

```cpp
int32_t db.h::db_idx_long_double_end (
    capi_name code,
    uint64_t scope,
    capi_name table
)
```

Get an end iterator representing just-past-the-end of the last table row of a secondary quadruple-precision floating-point index table. 

Get an end iterator representing just-past-the-end of the last table row of a secondary quadruple-precision floating-point index table


**Parameters:**


* **code** - The name of the owner of the table 
* **scope** - The scope where the table resides 
* **table** - The table name 



**Returns:**

end iterator of the table 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/db.h`
