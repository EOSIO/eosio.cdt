---
search:
    keywords: ['eosio::action', 'account', 'name', 'authorization', 'data', 'action', 'action', 'action', 'EOSLIB_SERIALIZE', 'send_context_free', 'data_as']
---

# struct eosio::action

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**action**](structeosio_1_1action.md)


Packed representation of an action. [More...](#detailed-description)
## Public Attributes

|Type|Name|
|-----|-----|
|**[name](structeosio_1_1name.md)**|[**account**](structeosio_1_1action_a4fc65ee0b7c184e7fcb8cdf6242220af.md#1a4fc65ee0b7c184e7fcb8cdf6242220af)<br>Name of the account the action is intended for. |
|**[name](structeosio_1_1name.md)**|[**name**](structeosio_1_1action_a407ffb98514612d2e9683dd7d88af91a.md#1a407ffb98514612d2e9683dd7d88af91a)<br>Name of the action. |
|std::vector< **[permission\_level](structeosio_1_1permission__level.md)** >|[**authorization**](structeosio_1_1action_a65851242b65f483e6b84d602d599f4e5.md#1a65851242b65f483e6b84d602d599f4e5)<br>List of permissions that authorize this action. |
|std::vector< char >|[**data**](structeosio_1_1action_a51845c4ffbe1c97fc8d05115d05a3ab1.md#1a51845c4ffbe1c97fc8d05115d05a3ab1)<br>Payload data. |


## Public Functions

|Type|Name|
|-----|-----|
||[**action**](structeosio_1_1action_aebc29a38547efa71db8ec20cc56d69a4.md#1aebc29a38547efa71db8ec20cc56d69a4) () = default <br>Construct a new action object. |
||[**action**](structeosio_1_1action_a9ef6c7fc0a06f53c754a1cc8af513d00.md#1a9ef6c7fc0a06f53c754a1cc8af513d00) (const **[permission\_level](structeosio_1_1permission__level.md)** & auth, struct **[name](structeosio_1_1name.md)** a, struct **[name](structeosio_1_1name.md)** n, T && value) <br>Construct a new action object with the given permission, action receiver, action name, action struct. |
||[**action**](structeosio_1_1action_aa6b336f4f7a8c61d68e14d99a0a9cd9d.md#1aa6b336f4f7a8c61d68e14d99a0a9cd9d) (std::vector< **[permission\_level](structeosio_1_1permission__level.md)** > auths, struct **[name](structeosio_1_1name.md)** a, struct **[name](structeosio_1_1name.md)** n, T && value) <br>Construct a new action object with the given list of permissions, action receiver, action name, action struct. |
||[**EOSLIB\_SERIALIZE**](structeosio_1_1action_a848cfa77395aa8447c5468c619c4a42a.md#1a848cfa77395aa8447c5468c619c4a42a) (**[action](structeosio_1_1action.md)**, (**[account](structeosio_1_1action_a4fc65ee0b7c184e7fcb8cdf6242220af.md#1a4fc65ee0b7c184e7fcb8cdf6242220af)**)(**[name](structeosio_1_1name.md)**)(**[authorization](structeosio_1_1action_a65851242b65f483e6b84d602d599f4e5.md#1a65851242b65f483e6b84d602d599f4e5)**)(**[data](structeosio_1_1action_a51845c4ffbe1c97fc8d05115d05a3ab1.md#1a51845c4ffbe1c97fc8d05115d05a3ab1)**)) const <br>Send the action as inline action. |
|void|[**send\_context\_free**](structeosio_1_1action_a984a41daf2d1c4d6506cac0c98ac7a2a.md#1a984a41daf2d1c4d6506cac0c98ac7a2a) () const <br>Send the action as inline context free action. |
|T|[**data\_as**](structeosio_1_1action_ab10f4621dd9e24a33642d8fb240b1a01.md#1ab10f4621dd9e24a33642d8fb240b1a01) () <br>Retrieve the unpacked data as T. |


## Detailed Description

This is the packed representation of an action along with meta-data about the authorization levels. 
## Public Attributes Documentation

### variable <a id="1a4fc65ee0b7c184e7fcb8cdf6242220af" href="#1a4fc65ee0b7c184e7fcb8cdf6242220af">account</a>

```cpp
name eosio::action::account;
```

Name of the account the action is intended for. 

Name of the account the action is intended for 

### variable <a id="1a407ffb98514612d2e9683dd7d88af91a" href="#1a407ffb98514612d2e9683dd7d88af91a">name</a>

```cpp
name eosio::action::name;
```

Name of the action. 

Name of the action 

### variable <a id="1a65851242b65f483e6b84d602d599f4e5" href="#1a65851242b65f483e6b84d602d599f4e5">authorization</a>

```cpp
std::vector<permission_level> eosio::action::authorization;
```

List of permissions that authorize this action. 

List of permissions that authorize this action 

### variable <a id="1a51845c4ffbe1c97fc8d05115d05a3ab1" href="#1a51845c4ffbe1c97fc8d05115d05a3ab1">data</a>

```cpp
std::vector<char> eosio::action::data;
```

Payload data. 

Payload data 

## Public Functions Documentation

### function <a id="1aebc29a38547efa71db8ec20cc56d69a4" href="#1aebc29a38547efa71db8ec20cc56d69a4">action (1/3)</a>

```cpp
eosio::action::action () = default
```

Construct a new action object. 

Default Constructor 

### function <a id="1a9ef6c7fc0a06f53c754a1cc8af513d00" href="#1a9ef6c7fc0a06f53c754a1cc8af513d00">action (2/3)</a>

```cpp
eosio::action::action (
    const permission_level & auth,
    struct name a,
    struct name n,
    T && value
)
```

Construct a new action object with the given permission, action receiver, action name, action struct. 

Construct a new action object with the given action struct


**Template parameters:**


* **T** - Type of action struct, must be serializable by `pack(...)` 



**Parameters:**


* **auth** - The permissions that authorizes this action 
* **a** - The name of the account this action is intended for (action receiver) 
* **n** - The name of the action 
* **value** - The action struct that will be serialized via pack into data 



### function <a id="1aa6b336f4f7a8c61d68e14d99a0a9cd9d" href="#1aa6b336f4f7a8c61d68e14d99a0a9cd9d">action (3/3)</a>

```cpp
eosio::action::action (
    std::vector< permission_level > auths,
    struct name a,
    struct name n,
    T && value
)
```

Construct a new action object with the given list of permissions, action receiver, action name, action struct. 

Construct a new action object with the given action struct


**Template parameters:**


* **T** - Type of action struct, must be serializable by `pack(...)` 



**Parameters:**


* **auths** - The list of permissions that authorize this action 
* **a** - The name of the account this action is intended for (action receiver) 
* **n** - The name of the action 
* **value** - The action struct that will be serialized via pack into data 



### function <a id="1a848cfa77395aa8447c5468c619c4a42a" href="#1a848cfa77395aa8447c5468c619c4a42a">EOSLIB\_SERIALIZE</a>

```cpp
eosio::action::EOSLIB_SERIALIZE (
    action ,
    (account)(name)(authorization)(data) 
) const
```

Send the action as inline action. 

Send the action as inline action 

### function <a id="1a984a41daf2d1c4d6506cac0c98ac7a2a" href="#1a984a41daf2d1c4d6506cac0c98ac7a2a">send\_context\_free</a>

```cpp
void eosio::action::send_context_free () const
```

Send the action as inline context free action. 

Send the action as inline context free action


**Precondition:**

This action should not contain any authorizations 




### function <a id="1ab10f4621dd9e24a33642d8fb240b1a01" href="#1ab10f4621dd9e24a33642d8fb240b1a01">data\_as</a>

```cpp
T eosio::action::data_as ()
```

Retrieve the unpacked data as T. 

Retrieve the unpacked data as T


**Template parameters:**


* **T** expected type of data 



**Returns:**

the action data 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/action.hpp`
