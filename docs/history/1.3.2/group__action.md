---
search:
    keywords: ['action', 'eosio::permission_level', 'eosio::action', 'unpack_action_data', 'require_recipient', 'require_recipient', 'require_auth', 'require_auth', 'has_auth', 'is_account', 'SEND_INLINE_ACTION']
---

# group action

Defines type-safe C++ wrapers for querying action and sending action. [More...](#detailed-description)
## Classes

|Type|Name|
|-----|-----|
|struct|[**eosio::permission\_level**](structeosio_1_1permission__level.md)|
|struct|[**eosio::action**](structeosio_1_1action.md)|


## Defines

## Detailed Description



**Note:**

There are some methods from the **[Action C++ API](.md#group__action)** that can be used directly from C++ 



## Functions Documentation

### function <a id="ga3ce0a297276d44f03c9b5dc17fcd2b31" href="#ga3ce0a297276d44f03c9b5dc17fcd2b31">unpack\_action\_data</a>

```cpp
T action::unpack_action_data ()
```

Interpret the action body as type T. 



**Returns:**

Unpacked action data casted as T.


Example:

```cpp
struct dummy_action {
  char a; //1
  unsigned long long b; //8
  int  c; //4

  EOSLIB_SERIALIZE( dummy_action, (a)(b)(c) )
};
dummy_action msg = unpack_action_data<dummy_action>();
```

 

### function <a id="ga4e1838d05857e38ddf8916e616698460" href="#ga4e1838d05857e38ddf8916e616698460">require\_recipient</a>

```cpp
void action::require_recipient (
    name notify_account
)
```

Add the specified account to set of accounts to be notified. 

Add the specified account to set of accounts to be notified


**Parameters:**


* **notify\_account** - name of the account to be verified 



### function <a id="gaa61e47227ac315887caf1e028191ff7d" href="#gaa61e47227ac315887caf1e028191ff7d">require\_recipient</a>

```cpp
void action::require_recipient (
    name notify_account,
    accounts... remaining_accounts
)
```

Notify an account for this action. 

All of the listed accounts will be added to the set of accounts to be notified
This helper method enables you to add multiple accounts to accounts to be notified list with a single call rather than having to call the similar C API multiple times.


**Note:**

action.code is also considered as part of the set of notified accounts




**Parameters:**


* **notify\_account** account to be notified 
* **remaining\_accounts** accounts to be notified

Example:

```cpp
require_recipient(N(Account1), N(Account2), N(Account3)); // throws exception if any of them not in set.
```

 

### function <a id="ga47b4afe79f1de07376e2ecdd541f92c7" href="#ga47b4afe79f1de07376e2ecdd541f92c7">require\_auth</a>

```cpp
void action::require_auth (
    name n
)
```

Verify specified account exists in the set of provided auths. 

Verifies that **[name](structeosio_1_1name.md)** exists in the set of provided auths on a action. Fails if not found.


**Parameters:**


* **name** - name of the account to be verified 



### function <a id="gaa9c6bd255a286073e7dc8a337f2b11a2" href="#gaa9c6bd255a286073e7dc8a337f2b11a2">require\_auth</a>

```cpp
void action::require_auth (
    const permission_level & level
)
```

Require the specified authorization for this action. 

Require the specified authorization for this action. If this action doesn't contain the specified auth, it will fail.


**Parameters:**


* **level** - Authorization to be required 



### function <a id="ga9e4650a61bbe0809cc62e6b2af8252d3" href="#ga9e4650a61bbe0809cc62e6b2af8252d3">has\_auth</a>

```cpp
bool action::has_auth (
    name n
)
```

Verifies that n has auth. 

Verifies that n has auth.


**Parameters:**


* **n** - name of the account to be verified 



### function <a id="ga887cd0fed2350c5ae52565ba6e290db3" href="#ga887cd0fed2350c5ae52565ba6e290db3">is\_account</a>

```cpp
bool action::is_account (
    name n
)
```

Verifies that n is an existing account. 

Verifies that n is an existing account.


**Parameters:**


* **n** - name of the account to check 



## Defines Documentation

### define <a id="ga7c267f408db88d60ce0a5c74845596ee" href="#ga7c267f408db88d60ce0a5c74845596ee">SEND\_INLINE\_ACTION</a>

```cpp
define SEND_INLINE_ACTION;
```

Send inline action. 

Send inline action


**Parameters:**


* **CONTRACT** - The account this action is intended for 
* **NAME** - The name of the action 
* **...** - The member of the action specified as ("action\_member1\_name", action\_member1\_value)("action\_member2\_name", action\_member2\_value) 



