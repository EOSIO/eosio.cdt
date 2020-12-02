---
content_title: C++ Coding Guidelines
---

In general, the EOSIO software follows the [C++ Core Guidelines](https://isocpp.org/wiki/faq/coding-standards) by Stroustrup and Sutter, while adheres to the C++ naming conventions of the C++ Standard Template Library (STL), with a few exceptions regarding indentation, comments, and other best coding practices:

## C++ Naming Conventions

Type | Standardized Method | Example
-|-|-
Structs | lowercase with _ between words | `struct `**`signed_block_header`**` : public block_header { ...`
Classes | lowercase with _ between words | `class `**`net_plugin`**` : public appbase::plugin<net_plugin>`
Functions | lowercase with _ between words | `uint32_t block_header::`**`num_from_id`**`(const block_id_type& id) { ...`
Constants | lowercase with _ between words<br>within appropriate scope | `const static `**`int block_interval_ms`**` = 500;`<br>`const static name `**`system_account_name`**` { N(eosio) };`<br>`const static auto `**`default_blocks_dir_name`**` = "blocks";`
Types | lowercase with _ between words | `using `**`decompose_t`**` = block_header_extension_types::decompose_t`<br>`typedef vector<std::pair<uint16_t,vector<char>>> `**`extensions_type`**
Template<br>Parameters | CamelCase, no spaces | `template<typename `**`ResultVariant>`**<br>`template<typename `**`Strand`**`>`
Template<br>Arguments | lowercase with _ between words | `flat_multimap<`**`uint16_t`**`, `**`block_header_extension`**`>`
Preprocessor<br>Macros | UPPERCASE | `#define `**`DEFINE_INTRINSIC_FUNCTION0`**`(module,cName,name,returnType)`
Preprocessor<br>Flags | UPPERCASE | `#define `**`EOSIO_EOS_VM_RUNTIME_ENABLED`**

## C++ Best Practices

Construct | Standardized Method | Example
-|-|-
`const` | on the left when it can;<br>on the right when it must | **`const`**` static uint32_t default_max_kv_key_size = 1024;`<br>`int* `**`const`**` null_int = NULL;`<br>`virtual state get_state()`**`const`**` = 0;`
`*` (pointer)<br>`&` (reference) | next to the type, not the variable | `const vector<int>`**`&`**` foo`<br>`int`**`*`**` const null_int = NULL;`
`if-else`, `switch`<br>`for`, `while`, `do-while` | leave space for clarity | `if (cond) ...` &nbsp;&nbsp;&nbsp;&nbsp; `switch (var) ...`<br>`for (i=0; i<n; i++) ...`  &nbsp;&nbsp;&nbsp;&nbsp; `do { ...` &nbsp;&nbsp;&nbsp;&nbsp; `while (cond) ...`
`class` | for public/private data-code ADTs | `class abstract_plugin { ... };`<br>`class plugin : public abstract_plugin { ... };`
`struct` | for PODs and public data-code ADTs | `struct block_header { ... };`<br>`struct signed_block_header : block_header { ... };`

## Indentation

Use three (3) spaces; don't use tabs please.

## Comments
```cpp
// single-line comments like this are OK

/* single-line comments like this are discouraged */

// multi-line comments
// or block comments
// like this are OK

/**
 * multi-line comments
 * or block comments
 * like this are OK
 */

/*
multi-line comments
or block comments like
this are discouraged
*/
```
