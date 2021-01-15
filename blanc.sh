#!/bin/bash
################################################################################
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
################################################################################
#
# This script will install the llvm toolchain on the different
# Debian and Ubuntu versions

set -eux

# Check for required tools
needed_binaries=(lsb_release wget curl)
missing_binaries=()
for binary in "${needed_binaries[@]}"; do
    if ! which $binary &>/dev/null ; then
        missing_binaries+=($binary)
    fi
done
if [[ ${#missing_binaries[@]} -gt 0 ]] ; then
    echo "You are missing some tools this script requires: ${missing_binaries[@]}"
    echo "(hint: apt install lsb-release wget software-properties-common)"
    exit 4
fi

DISTRO=$(lsb_release -is)
VERSION=$(lsb_release -sr)
DIST_VERSION="${DISTRO}-${VERSION}"

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root!"
   exit 1
fi

if [[ ${DIST_VERSION} == "Ubuntu-20.04" ]] ; then
   DOWNLOAD_URL=$(curl https://api.github.com/repos/turnpike/blanc/releases/latest | awk '/browser_download_url.*20.04_amd64.deb/{ print $2 }' | tr -d '"')
elif [[ ${DIST_VERSION} == "Ubuntu-18.04" ]] ; then
   DOWNLOAD_URL=$(curl https://api.github.com/repos/turnpike/blanc/releases/latest | awk '/browser_download_url.*18.04_amd64.deb/{ print $2 }' | tr -d '"')
else
   echo "Distribution '$DISTRO' in version '$VERSION' is not supported by this script (${DIST_VERSION})."
   exit 2
fi
FILE_NAME=$(echo $DOWNLOAD_URL | awk -F "/" '{ print $NF }')

# install everything
wget $DOWNLOAD_URL
apt install ./$FILE_NAME
rm -f $FILE_NAME
