# EOSIO.CDT (Contract Development Toolkit)
## Version : 1.7.0

EOSIO.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate smart contract development for the EOSIO platform. In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that EOSIO.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are incomplete or not available.

## New Introductions
As of this release two new repositories are under the suite of tools provided by **EOSIO.CDT**.  These are the [Ricardian Template Toolkit](https://github.com/eosio/ricardian-template-toolkit) and the [Ricardian Specification](https://github.com/eosio/ricardian-spec).  The **Ricardian Template Toolkit** is a set of libraries to facilitate smart contract writers in crafting their Ricardian contracts.  The Ricardian specification is the working specification for the above mentioned toolkit.  Please note that both projects are **alpha** releases and are subject to change.

## Important!
EOSIO.CDT Version 1.3.x introduced quite a few breaking changes. To have binary releases we needed to remove the concept of a core symbol from EOSIO.CDT. This meant drastic changes to symbol, asset and other types/functions that were connected to them. Since these changes would be disruptive, we decided to add as many disruptive changes needed for future contract writing, so that disruption should only occur once. Please read the [Upgrade guide from 1.2 to 1.3](./upgrading/1.2-to-1.3) section of this readme.

## Concepts

### Dispatcher and Notifications
[Dispatcher API reference](https://eosio.github.io/eosio.cdt/1.6.0/group__dispatcher.html)
[schemata:link to glossary]

### Multi Index Table and RAM
[Multi Index Tables explained by example](https://developers.eos.io/eosio-cpp/docs/using-multi-index-tables)
[schemata:link to glossary]

### Smart Contract
[schemata:link to glossary]

### Action
[schemata:link to glossary]

### Transaction and Deferred Transaction
[schemata:link to glossary]

## Contributing

[Contributing Guide](../CONTRIBUTING.md)

[Code of Conduct](../CONTRIBUTING.md#conduct)

## License

[MIT](../LICENSE)

## Important

See LICENSE for copyright and license terms.  Block.one makes its contribution on a voluntary basis as a member of the EOSIO community and is not responsible for ensuring the overall performance of the software or any related applications.  We make no representation, warranty, guarantee or undertaking in respect of the software or any related documentation, whether expressed or implied, including but not limited to the warranties or merchantability, fitness for a particular purpose and noninfringement. In no event shall we be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or documentation or the use or other dealings in the software or documentation.  Any test results or performance figures are indicative and will not reflect performance under all conditions.  Any reference to any third party or third-party product, service or other resource is not an endorsement or recommendation by Block.one.  We are not responsible, and disclaim any and all responsibility and liability, for your use of or reliance on any of these resources. Third-party resources may be updated, changed or terminated at any time, so the information here may be out of date or inaccurate.
