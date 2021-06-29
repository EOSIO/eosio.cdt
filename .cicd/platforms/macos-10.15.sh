#!/bin/bash
set -eou pipefail
VERSION=1

brew update && brew upgrade
brew install automake cmake doxygen gettext git gmp graphviz lcov libtool python@3 wget
