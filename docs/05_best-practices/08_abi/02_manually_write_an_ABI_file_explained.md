---
content_title: Manually write, or fix, an ABI file
---

- Advanced features of the newest version of the ABI will require manual construction of the ABI, and odd and advanced C++ patterns could capsize the generator's type deductions. So having a good knowledge of how to write an ABI should be an essential piece of knowledge of a smart contract writer.
- Please refer to [developers.eos.io "How to Write an ABI File"](https://developers.eos.io/eosio-cpp/docs/how-to-write-an-abi) to learn about the different sections of an ABI.

## Adding Ricardian Contracts and Clauses to ABI
- The ABI generator will try to automatically import contracts and clauses into the generated ABI.  There are a few caveats to this, one is a strict naming policy of the files and an HTML tag used to mark each Ricardian contract and each clause.
- The Ricardian contracts should be housed in a file with the name `<contract name>.contracts.md` and the clauses should be in a file named `<contract name>.clauses.md`.
 - For each Ricardian contract the header `<h1 class="contract">ActionName</h1>` should be used, as this directs the ABI generator to attach this Ricardian contract to the specified action.
 - For each Ricardian clause, the header `<h1 class="clause">ClauseID</h1>` should be used, as this directs the ABI generator to the clause id and the subsequent body.
 - The option `-R` has been added to [`eosio-cpp`](../../03_command-reference/eosio-cpp.md) and [`eosio-abigen`](../../03_command-reference/eosio-abigen.md) to add "resource" paths to search from, so you can place these files in any directory structure you like and use `-R<path to file>` in the same vein as `-I` for include paths.
 - For exemplification see [hello.contracts.md](https://github.com/EOSIO/eosio.cdt/blob/master/examples/hello/ricardian/hello.contracts.md).
