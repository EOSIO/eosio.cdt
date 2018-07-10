#! /bin/bash

printf "\t=========== Building eosio.wasmsdk ===========\n\n"

RED='\033[0;31m'
NC='\033[0m'

if [ ! -d "/usr/local/eosio" ]; then
   printf "${RED}Error, please ensure that eosio is installed correctly!\n\n${NC}"
   exit -1
fi

export DISK_MIN=10

unamestr=`uname`
if [[ "${unamestr}" == 'Darwin' ]]; then
   BOOST=/usr/local
   CXX_COMPILER=g++
   bash ./scripts/eosio_build_darwin.sh
   export ARCH="Darwin"
else
   BOOST=~/opt/boost
   OS_NAME=$( cat /etc/os-release | grep ^NAME | cut -d'=' -f2 | sed 's/\"//gI' )
	
   case "$OS_NAME" in
      "Amazon Linux AMI")
         bash ./scripts/eosio_build_amazon.sh
         export ARCH="Amazon Linux AMI"
         ;;
      "CentOS Linux")
         bash ./scripts/eosio_build_centos.sh
         export ARCH="Centos"
         ;;
      "elementary OS")
         bash ./scripts/eosio_build_ubuntu.sh
         export ARCH="elementary OS"
         ;;
      "Fedora")
         bash ./scripts/eosio_build_fedora.sh
         export ARCH="Fedora"
         ;;
      "Linux Mint")
         bash ./scripts/eosio_build_ubuntu.sh
         export ARCH="Linux Mint"
         ;;
      "Ubuntu")
         bash ./scripts/eosio_build_ubuntu.sh
         export ARCH="Ubuntu"
         ;;
      *)
         printf "\\n\\tUnsupported Linux Distribution. Exiting now.\\n\\n"
         exit 1
   esac
fi

if [ $# -ge 1 ]; then
   CORE_SYMBOL=$1
fi

CORES=`getconf _NPROCESSORS_ONLN`
mkdir -p build
pushd build &> /dev/null
cmake -DBOOST_ROOT="${BOOST}" -DCORE_SYMBOL_NAME="${CORE_SYMBOL}" ../
make -j${CORES}
popd &> /dev/null
