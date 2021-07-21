#!/bin/bash
set -euo pipefail

. "${0%/*}/libfunctions.sh"

echo '+++ :minidisc: Installing EOSIO CDT'

if [[ $(apt-get --version 2>/dev/null) ]]; then # debian family packaging
    perform 'apt-get update'
    perform 'apt-get install -y /eosio.cdt/*.deb'
elif [[ $(yum --version 2>/dev/null) ]]; then # RHEL family packaging
    perform 'yum check-update || :'
    perform 'yum install -y /eosio.cdt/*.rpm'
elif [[ $(brew --version 2>/dev/null) ]]; then # homebrew packaging
    perform 'brew update'
    perform 'mkdir homebrew-eosio.cdt'
    perform 'git init homebrew-eosio.cdt'
    perform 'cp *.rb homebrew-eosio.cdt'
    perform "sed -i.bk -e 's/url \".*\"/url \"http:\/\/127.0.0.1:7800\"/' homebrew-eosio.cdt/*.rb"
    perform "pushd homebrew-eosio.cdt && git add *.rb && git commit -m 'test it!' && popd"
    perform "brew tap eosio/eosio homebrew-eosio.cdt"
    perform '{ python3 -m http.server 7800 & } && export HTTP_SERVER_PID=$!'
    perform 'sleep 20s'
    perform 'brew install eosio.cdt'
    perform 'kill $HTTP_SERVER_PID'
else
    echo 'ERROR: Package manager not detected!'
    exit 3
fi

eosio-cpp --version
