#!/bin/bash
set -eou pipefail
VERSION=1

brew update && brew upgrade
brew unlink md5sha1sum
brew install coreutils gnu-sed grep tree automake cmake doxygen gettext git gmp graphviz lcov libtool python@3 wget 

echo 'export PATH="/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"' >> ~/.bash_profile
echo 'export PATH="/usr/local/opt/grep/libexec/gnubin:$PATH"' >> ~/.bash_profile
echo 'export PATH="/usr/local/opt/coreutils/libexec/gnubin:$PATH"' >> ~/.bash_profile
echo '' >> ~/.bash_profile
source ~/.bash_profile
