#!/bin/bash
set -eou pipefail
VERSION=1

brew update && brew upgrade
brew install automake cmake doxygen gettext git gmp graphviz lcov libtool python@3 wget coreutils gnu-sed grep tree

echo 'export PATH="/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"' >> ~/.bash_profile
echo 'export PATH="/usr/local/opt/grep/libexec/gnubin:$PATH"' >> ~/.bash_profile
echo 'export PATH="/usr/local/opt/coreutils/libexec/gnubin:$PATH"' >> ~/.bash_profile
echo '' >> ~/.bash_profile
source ~/.bash_profile