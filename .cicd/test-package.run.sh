#!/bin/bash
set -euo pipefail

. "${0%/*}/libfunctions.sh"

echo '+++ :minidisc: Installing EOSIO CDT'

if [[ $(apt-get --version 2>/dev/null) ]]; then # debian family packaging
    perform 'apt-get update'
    perform 'apt-get install -y /eos/*.deb'
elif [[ $(yum --version 2>/dev/null) ]]; then # RHEL family packaging
    perform 'yum check-update || :'
    perform 'yum install -y /eos/*.rpm'
else
    echo 'ERROR: Package manager not detected!'
    exit 3
fi

taurus-cpp --version