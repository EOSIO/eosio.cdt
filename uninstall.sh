#! /bin/bash
OPT_LOCATION=${HOME}/opt

binaries=(eosio-ranlib
          eosio-ar
          eosio-objdump
          eosio-readelf
          eosio-abigen
          eosio-wasm2wast
          eosio-wast2wasm
          eosio-pp
          eosio-cc
          eosio-cpp
          eosio-ld)

if [ -d $OPT_LOCATION/eosio.cdt ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd $OPT_LOCATION &> /dev/null
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
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

if [ -d $OPT_LOCATION/eosio.wasmsdk ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            pushd $OPT_LOCATION &> /dev/null
            rm -rf eosio.wasmsdk
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;

         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
