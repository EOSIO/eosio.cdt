# eosio.cdt
The [eosio.cdt](https://buildkite.com/EOSIO/eosio-dot-cdt) pipeline is the primary pipeline for the [eosio.cdt](https://github.com/EOSIO/eosio.cdt) repository.

<x>

## Index
1. [Configuration](README.md#configuration)
   1. [Variables](README.md#variables)
   1. [Examples](README.md#examples)
1. [Pipelines](README.md#pipelines)
1. [See Also](README.md#see-also)

## Configuration
Most EOSIO.CDT pipelines are run any time you push a commit or tag to an open pull request in [eosio.cdt](https://github.com/EOSIO/eosio.cdt), any time you merge a pull request, and nightly.

Platform configuration used pinned configuration and versions.

### Variables
Most pipelines in the organization have several environment variables that can be used to configure how the pipeline runs. These environment variables can be specified when manually triggering a build via the Buildkite UI.

Configure which platforms are run:
```bash
SKIP_LINUX='true|false'              # skip all steps on Linux distros
SKIP_MAC='true|false'                # skip all steps on Mac hardware
```
These will override more specific operating system declarations, and primarily exist to disable one of our two buildfleets should one be sick or the finite macOS agents are congested.

Configure which operating systems are built, tested, and packaged:
```bash
RUN_ALL_TESTS='true'                 # run all tests in the current build (including LRTs, overridden by SKIP* variables)
SKIP_AMAZON_LINUX_2='true|false'     # skip all steps for Amazon Linux 2
SKIP_CENTOS_7='true|false'           # skip all steps for Centos 7
SKIP_CENTOS_8='true|false'           # skip all steps for Centos 8
SKIP_MACOS_10_14='true|false'        # skip all steps for MacOS 10.14
SKIP_MACOS_10_15='true|false'        # skip all steps for MacOS 10.15
SKIP_UBUNTU_16_04='true|false'       # skip all steps for Ubuntu 16.04
SKIP_UBUNTU_18_04='true|false'       # skip all steps for Ubuntu 18.04
SKIP_UBUNTU_20_04='true|false'       # skip all steps for Ubuntu 20.04
```

Configure which steps are executed for each operating system:
```bash
SKIP_BUILD='true|false'              # skip all build steps
SKIP_TOOLCHAIN_TEST='true|false'     # skip all toolchain test steps
SKIP_UNIT_TESTS='true|false'         # skip all unit tests
SKIP_PACKAGE_BUILDER='true|false'    # skip all packaging steps
SKIP_INTEGRATION_TESTS='true|false'  # skip all integration tests
```

Configure how the steps are executed:
```bash
TIMEOUT='##'                         # set timeout in minutes for all steps
```

### Examples
Build and test on Linux only:
```bash
SKIP_MAC='true'
```

Build and test on MacOS only:
```bash
SKIP_LINUX='true'
```

Skip all tests:
```bash
SKIP_TOOLCHAIN_TEST='true|false'     # skip all toolchain test steps
SKIP_UNIT_TESTS='true|false'         # skip all unit tests
```

## Pipelines
There are several eosio pipelines that exist and are triggered by pull requests, pipelines, or schedules:

Pipeline | Details
---|---
[eosio.cdt](https://buildkite.com/EOSIO/eosio-dot-cdt) | [eosio.cdt](https://github.com/EOSIO/eosio.cdt) build, tests, and packaging; runs on every pull request and base branch commit, and nightly
[eosio.cdt - Version Check](https://buildkite.com/EOSIO/eosio-dot-cdt-version-check) | [eosio.cdt](https://github.com/EOSIO/eosio.cdt) for tagged builds (hence releases). Tests release artifacts.

## See Also
- Buildkite
  - [DevDocs](https://github.com/EOSIO/devdocs/wiki/Buildkite)
  - [Run Your First Build](https://buildkite.com/docs/tutorials/getting-started#run-your-first-build)
- [#help-automation](https://blockone.slack.com/archives/CMTAZ9L4D) Slack Channel

</x>
