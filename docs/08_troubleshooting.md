## Troubleshooting

1. __Problem__: if when sending an action to the blockchian you get the error below
```{
    "code":500,
    "message":"Internal Service Error",
    "error":{
        "code":3090003,
        "name":"unsatisfied_authorization",
        "what":"Provided keys, permissions, and delays do not satisfy declared authorizations",
        "details":[
            {
                "message":"transaction declares authority '{"actor":"account_name","permission":"permission_name"}', but does not have signatures for it under a provided delay of 0 ms, provided permissions [], provided keys ["EOS5ZcMvpgtDMdVtvCFewAQYTyfN6Vqhg4kdgauffx3jiaKaeWfY1"], and a delay max limit of 3888000000 ms",
                "file":"authorization_manager.cpp",
                "line_number":524,
                "method":"check_authorization"
            }
        ]
    }
}
```
__Possbile solution__: verify if you did not forget to set code for contract, is it possbile that you only set the abi for the contract but not the code as well?

2. __Problem__: when sending an action to the blockchain an error similar to the one below is encountered:
```sh
Error 3015014: Pack data exception
Error Details:
Unexpected input encountered while processing struct 'action_name_here'
```
__Possible solution__: did not specified correct parameter when sending the action to the blockchain. When no parameter is needed the command should look like the one below:
```sh
cleos push action eostutorial1 get '[]' -p eostutorial1@active
```

The command above is one way of sending correctly `get` action with no parameters to the blockchain.

3. __Problem__: when sending an action to the blockchain an error similar to the one below is encountered:
```sh
error 2019-09-25T07:38:14.859 thread-0  main.cpp:3449                 main                 ] Failed with error: Assert Exception (10)
!action_type.empty(): Unknown action action_name in contract eostutorial1
```
__Possible solution__: verify if the action attribute `[[eosio::action]]` is used when defining and/or declaring the action `action_name` for the contract.

4. __Problem__: when deploying a contract code to the blockchain a similar error with the ones below is encountered:
```sh
Error 3160010: No abi file found
or
Error 3160009: No wasm file found
```
__Possible solution__: verify that abi and wasm files exist in the directory specified in the `cleos set contract` command, and that their names match the directory name.

5. __Problem__: Action triggers ram charge which cannot be initiated from a notification, 
__Possible solution__: The reason for this error is because the notification action doesn't have authorization to by the needed RAM.

6. __Problem__: You successfuly re-deployed the contract code, but when you query the table you get the custom message that you wrote when the table is not initialized (doesn't exist), or the system error message below in case you do not have code that checks first if table exist:
```sh
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: singleton does not exist
pending console output: 
```
__Possible solution__: it is possible that you changed the table name? That is the first, of `eosio::name` type, parameter which you passed to the `eosio::template` type alias definition. If you need to change the table structure definition there are some limitations and a couple of ways to do it which are explained in the [Data Design and Migration](./05_best-practices/04_data-design-and-migration.md) section.


7. __Problem__: You successfuly re-deployed the contract code, but when you query the table you get the fields of the row values swapped, that is, it appears the values stored in table rows are the same only that they are swapped between fields/columns.
__Possible solution__: it is possible that you changed the order of the fields the table struct definition? If you change the order of the table struct definition, if the swapped fields have the same type you will see the data in the fields correctly, however if the types of the fields are different the results could be of something undefined. If you need to change the table structure definition there are some limitations and a couple of ways to do it which are explained in the [Data Design and Migration](./05_best-practices/04_data-design-and-migration.md) section.


8. __Problem__: You successfuly re-deployed the contract code, but when you query the table you get a parse error, like the one below, or the returned data seems to be garbage.
```sh
error 2019-09-26T07:05:54.825 thread-0  main.cpp:3449                 main                 ] Failed with error: Parse Error (4)
Couldn't parse type_name
```
__Possible solution__: it is possible that you changed the type of the fields for the table struct definition? If you need to change the table structure definition there are some limitations and a couple of ways to do it which are explained in the [Data Design and Migration](./05_best-practices/04_data-design-and-migration.md) section.

8. __Problem__: eosio-cpp process never completes.
__Possible solution__: make sure you have at least 2 cores on the host that executes the eosio-cpp (e.g. docker container, VM, local sub-system)

9. __Problem__: you can not find the `now()` time function.
__Possible solution__: It has been replaced by `current_time_point().sec_since_epoch()`, it returns the time in microseconds from 1970 of the `current block` as a time_point. There's also available `current_block_time` which returns the time in microseconds from 1970 of the `current block` as a `block_timestamp`. Be aware that for time base functions, the assumption is when you call something like `now()` or `current_time()` you will get the exact now/current time, however that is not the case with EOSIO, you get __the block time__, and only ever get __the block time__ from the available `current_time()` or `now()` no matter how many times you call it.