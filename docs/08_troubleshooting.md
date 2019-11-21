---
content_title: Troubleshooting
---

## When sending an action to the blockchain you get the error below
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
__Possible solution__: Verify if you did not forget to set code for contract, is it possible that you only set the `abi` for the contract but not the code as well?

## When sending an action to the blockchain an error similar to the one below is encountered:
```sh
Error 3015014: Pack data exception
Error Details:
Unexpected input encountered while processing struct 'action_name_here'
```
__Possible solution__: You did not specify correctly the parameter when sending the action to the blockchain. When no parameter is needed the command should look like the one below:
```sh
cleos push action eostutorial1 get '[]' -p eostutorial1@active
```
The command above is one way of sending correctly `get` action with no parameters to the blockchain.

## When sending an action to the blockchain an error similar to the one below is encountered:
```sh
error 2019-09-25T07:38:14.859 thread-0  main.cpp:3449                 main                 ] Failed with error: Assert Exception (10)
!action_type.empty(): Unknown action action_name in contract eostutorial1
```
__Possible solution__: Verify if the action attribute `[[eosio::action]]` is used when defining and/or declaring the action `action_name` for the contract.

## When deploying a contract code to the blockchain a similar error with the ones below is encountered:
```sh
Error 3160010: No abi file found
or
Error 3160009: No wasm file found
```
__Possible solution__: Verify that `abi` and `wasm` files exist in the directory specified in the `cleos set contract` command, and that their names match the directory name.

## Action triggers ram charge which cannot be initiated from a notification.

__Possible solution__: The reason for this error is because the notification action doesn't have authorization to buy the needed RAM. In the context of multi index tables, there’s a table payer and a row payer. Only the contract can modify rows. The contract can create rows with a payer that didn’t authorize the action if the total amount of ram charged that payer doesn’t increase (e.g. delete a row and add another with the same payer). The table payer can’t change until the last row is deleted. For the purposes of billing, a table is identified by the tuple `contract, scope, table`. When you create a row for a `contract, scope, table` tuple that doesn’t exist, you create a table with the same payer. This can outlive the original row which created it, if other rows were created with that combination and this prevents the original payer from getting their ram back. Secondary indexes throw in more complexity since they use the lower 4 bits of the table name, producing additional `contract, scope, table` tuples combinations. Key takeaway: payer is about billing, not access control

## You successfully re-deployed the contract code, but when you query the table you get the custom message that you coded when the table is not initialized (doesn't exist), or the system error message below in case you do not have code that checks first if table exist:
```sh
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: singleton does not exist
pending console output: 
```
__Possible solution__: It is possible that you changed the table name? That is the first, of `eosio::name` type, parameter which you passed to the `eosio::template` type alias definition. Or did you change the table structure definition at all? If you need to change the table structure definition there are some limitations and a couple of ways to do it which are explained in the [Data Design and Migration](./05_best-practices/04_data-design-and-migration.md) section.

## You successfully re-deployed the contract code, but when you query the table you get the fields of the row values swapped, that is, it appears the values stored in table rows are the same only that they are swapped between fields/columns.

__Possible solution__: It is possible that you changed the order of the fields the table struct definition? If you change the order of the table struct definition, if the swapped fields have the same type you will see the data in the fields correctly, however if the types of the fields are different the results could be of something undefined. If you need to change the table structure definition there are some limitations and a couple of ways to do it which are explained in the [Data Design and Migration](./05_best-practices/04_data-design-and-migration.md) section.

## You successfully re-deployed the contract code, but when you query the table you get a parse error, like the one below, or the returned data seems to be garbage.
```sh
error 2019-09-26T07:05:54.825 thread-0  main.cpp:3449                 main                 ] Failed with error: Parse Error (4)
Couldn't parse type_name
```
__Possible solution__: It is possible that you changed the type of the fields for the table struct definition? If you need to change the table structure definition there are some limitations and a couple of ways to do it which are explained in the [Data Design and Migration](./05_best-practices/04_data-design-and-migration.md) section.

## eosio-cpp process never completes.

__Possible solution__: make sure you have at least 2 cores on the host that executes the eosio-cpp (e.g. docker container, VM, local sub-system)

## You can not find the `now()` time function, or the result of the `current_time_point` functions are not what you expected them to be.

__Possible solution__: The `now()` function has been replaced by `current_time_point().sec_since_epoch()`, it returns the time in microseconds from 1970 of the `current block` as a time_point. There's also available `current_block_time()` which returns the time in microseconds from 1970 of the `current block` as a `block_timestamp`. Be aware that for time base functions, the assumption is when you call something like `now()` or `current_time()` you will get the exact now/current time, however that is not the case with EOSIO, you get __the block time__, and only ever get __the block time__ from the available `sec_since_epoch()` or `current_block_time()` no matter how many times you call it.

## You successfully re-deployed the contract code, but when you broadcast one of the contracts methods to the blockchain you get below error message:
```sh
Error 3050004: eosio_assert_code assertion failure
Error Details:
assertion failure with error code: 8000000000000000000
```
__Possible solution__: If you are referencing a smart contract from another smart contract and each of them have at least one action with the same name you will experience the above error when sending to the blockchain one of those actions, so what you have to do is to make sure the action names between those two contracts are not common.

## Print statements from smart contract code are not seen in the output.

__Possible solution__: There are a few reasons print statements do not show up in the output. One reason could be because an error occurs, in which case the whole transaction is rolled back and the print statements output is replaced by the error that occurs instead; Another reason is if you are in a loop, iterating through a table's rows for example and for each row you have a print statement that prints also the new line char at the `'\n'` only the chars before the new line char from the first iteration will be printed, nothing else after that, nothing from the second iteration onwards either.

The below code will print just the first line of the iteration.

```cpp
  auto index=0;
  for (auto& item : testtab)
  {
    eosio::print_f("{item %}={%, %, %} \n", ++index, item.test_primary, item.secondary, item.datum);
  }
```

The below code will print all lines of the iteration separated by `'|'` char.
```cpp
  auto index=0;
  for (auto& item : testtab)
  {
    eosio::print_f("{item %}={%, %, %} |", ++index, item.test_primary, item.secondary, item.datum);
  }
```

## Print statements from smart contract code are not shown in the `expected order`.

__Possible solution__: The key point here is the `expected order` and what you think it should be. Although the EOSIO is single threaded, when looking at your smart contract action code implementation, which let's say it has a series of `print` (either `print_f` or `printf`) statements, they might not necessarily be outputted in the order the `apparent` code workflow is. One example is when inline transactions are sent from your smart contract action code, and you expect to see the `print` statements from within the inline action code outputted before the `print` statements made after the inline action `send` statement. For better exemplification let's look at the code below:

```cpp
[[eosio::action]] void multi_index_example::mod( name user, uint64_t n ) {

  // `mod` action implementation code goes here...

  print_f("Output line before the inline send action.")

  singleton_set_action singleton_set("eostutorial1"_n, {get_self(), "active"_n});
  singleton_set.send(get_self(), n, get_self());

  print_f("Output line after the inline send action.")
}
```

The code above has one `print` statement before the `singleton_set.send` and another one after the `singleton_set.send`. If you wrote some more `print` statements in the code that implements the `singleton_set.send` action and expect to see them before the second `print` statement then it is a wrong assumption. The inline actions are broadcasted to the network and they are executed at a different time, asynchronous of the current execution thread of the current `multi_index_example::mod` action, therefor it is impossible to predict when the `print` statements from inline action code will be outputted.