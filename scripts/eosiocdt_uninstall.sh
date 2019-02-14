#! /bin/bash

OPT_LOCATION=$HOME/opt

binaries=(
   eosio-ranlib
   eosio-ar
   eosio-objdump
   eosio-readelf
   eosio-abigen
   eosio-wasm2wast
   eosio-wast2wasm
   eosio-pp
   eosio-cc
   eosio-cpp
   eosio-ld
   eosio-abidiff
   eosio-init
   llvm-readelf
   llvm-objdump
   llvm-ar
   llvm-ranlib
)

if [ -d "/usr/local/eosio.cdt" ]; then
   printf "Do you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\nThis requires sudo, please run ./uninstall.sh with sudo\n\n"
               exit -1
            fi
            pushd /usr/local &> /dev/null
            rm -rf eosio.cdt
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            pushd lib/cmake &> /dev/null
            rm -rf eosio.cdt
            popd &> /dev/null
            break;;
         [Nn]* ) 
            printf "Aborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

if [ -d "/usr/local/eosio.wasmsdk" ]; then
   printf "Do you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\nThis requires sudo, please run ./uninstall.sh with sudo\n\n"
               exit -1
            fi
            pushd /usr/local &> /dev/null
            rm -rf eosio.wasmsdk
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;

         [Nn]* ) 
            printf "Aborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

if [ -d $OPT_LOCATION/eosio.cdt ] || [[ $1 == "force-new" ]]; then
   printf "Do you wish to remove this install?\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd $HOME &> /dev/null
            pushd opt &> /dev/null
            rm -rf eosio.cdt
            popd &> /dev/null
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            pushd lib/cmake &> /dev/null
            rm -rf eosio.cdt
            popd &> /dev/null
            break;;
         [Nn]* )
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi