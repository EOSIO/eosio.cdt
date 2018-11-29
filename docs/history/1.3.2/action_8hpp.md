---
search:
    keywords: ['action.hpp', 'eosio::permission_level', 'eosio::action', 'eosio::inline_dispatcher', 'eosio::inline_dispatcher< void(T::*)(Args...), Name >', 'INLINE_ACTION_SENDER3', 'INLINE_ACTION_SENDER2', 'INLINE_ACTION_SENDER', 'SEND_INLINE_ACTION']
---

# file action.hpp

**[Go to the source code of this file.](action_8hpp_source.md)**
## Classes

|Type|Name|
|-----|-----|
|struct|[**eosio::permission\_level**](structeosio_1_1permission__level.md)|
|struct|[**eosio::action**](structeosio_1_1action.md)|
|struct|[**eosio::inline\_dispatcher**](structeosio_1_1inline__dispatcher.md)|
|struct|[**eosio::inline\_dispatcher< void(T::\*)(Args...), Name >**](structeosio_1_1inline__dispatcher_3_01void_07_t_1_1_5_08_07_args_8_8_8_08_00_01_name_01_4.md)|


## Defines

|Type|Name|
|-----|-----|
||[**INLINE\_ACTION\_SENDER3**](action_8hpp_a20c19e0f931fad16c4db6d29480acddf.md#1a20c19e0f931fad16c4db6d29480acddf)|
||[**INLINE\_ACTION\_SENDER2**](action_8hpp_abeecc07548f70308a4aab9fcdb2d22f1.md#1abeecc07548f70308a4aab9fcdb2d22f1)|
||[**INLINE\_ACTION\_SENDER**](action_8hpp_a1bb288791cd436d07bddf3d43f4740d4.md#1a1bb288791cd436d07bddf3d43f4740d4)|
||[**SEND\_INLINE\_ACTION**](group__action_ga7c267f408db88d60ce0a5c74845596ee.md#ga7c267f408db88d60ce0a5c74845596ee)<br>Send inline action. |


## Detailed Description



**Copyright**

defined in eos/LICENSE.txt 



## Defines Documentation

### define <a id="1a20c19e0f931fad16c4db6d29480acddf" href="#1a20c19e0f931fad16c4db6d29480acddf">INLINE\_ACTION\_SENDER3</a>

```cpp
define INLINE_ACTION_SENDER3;
```



### define <a id="1abeecc07548f70308a4aab9fcdb2d22f1" href="#1abeecc07548f70308a4aab9fcdb2d22f1">INLINE\_ACTION\_SENDER2</a>

```cpp
define INLINE_ACTION_SENDER2;
```



### define <a id="1a1bb288791cd436d07bddf3d43f4740d4" href="#1a1bb288791cd436d07bddf3d43f4740d4">INLINE\_ACTION\_SENDER</a>

```cpp
define INLINE_ACTION_SENDER;
```



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





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/action.hpp`
