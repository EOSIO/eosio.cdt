#!/bin/bash

printf "\t=========== Building eosio.cdt ===========\n\n"

RED='\033[0;31m'
NC='\033[0m'
txtbld=$(tput bold)
bldred=${txtbld}$(tput setaf 1)
txtrst=$(tput sgr0)

export DISK_MIN=10
export TEMP_DIR="/tmp"
TEMP_DIR='/tmp'
DISK_MIN=10

CMAKE_VERSION_MAJOR=3
CMAKE_VERSION_MINOR=10
CMAKE_VERSION_PATCH=2
CMAKE_VERSION=${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}
MONGODB_VERSION=3.6.3
MONGO_C_DRIVER_VERSION=1.10.2
MONGO_CXX_DRIVER_VERSION=3.3
SRC_LOCATION=/usr/local/src
BOOST_VERSION_MAJOR=1
BOOST_VERSION_MINOR=67
BOOST_VERSION_PATCH=0
BOOST_VERSION=${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}_${BOOST_VERSION_PATCH}
LLVM_CLANG_VERSION=release_40
TINI_VERSION=0.18.0

unamestr=`uname`
if [[ "${unamestr}" == 'Darwin' ]]; then
   BOOST=/usr/local
   CXX_COMPILER=g++
   ARCH=$unamestr
   bash ./scripts/eosio_build_darwin.sh
else
   OS_NAME=$( cat /etc/os-release | grep ^NAME | cut -d'=' -f2 | sed 's/\"//gI' )
   case "$OS_NAME" in
      "Amazon Linux AMI")
         ARCH=$OS_NAME
         bash ./scripts/eosio_build_amazon.sh
         ;;
      "CentOS Linux")
         ARCH="Centos"
         bash ./scripts/eosio_build_centos.sh
         ;;
      "elementary OS")
         ARCH=$OS_NAME
         bash ./scripts/eosio_build_ubuntu.sh
         ;;
      "Fedora")
         ARCH=$OS_NAME
         bash ./scripts/eosio_build_fedora.sh
         ;;
      "Linux Mint")
         ARCH=$OS_NAME
         bash ./scripts/eosio_build_ubuntu.sh
         ;;
      "Ubuntu")
         ARCH=$OS_NAME
         bash ./scripts/eosio_build_ubuntu.sh
         ;;
      "Debian GNU/Linux")
         ARCH="Debian"
	      bash ./scripts/eosio_build_ubuntu.sh
	      ;;
      *)
         printf "\\n\\tUnsupported Linux Distribution. Exiting now.\\n\\n"
         exit 1
   esac
fi

if [[ `uname` == 'Darwin' ]]; then
   FREE_MEM=`vm_stat | grep "Pages free:"`
   read -ra FREE_MEM <<< "$FREE_MEM"
   FREE_MEM=$((${FREE_MEM[2]%?}*(4096))) # free pages * page size
else
   FREE_MEM=`LANG=C free | grep "Mem:" | awk '{print $4}'`
fi

CORES_AVAIL=`getconf _NPROCESSORS_ONLN`
MEM_CORES=$(( ${FREE_MEM}/4000000 )) # 4 gigabytes per core
MEM_CORES=$(( $MEM_CORES > 0 ? $MEM_CORES : 1 ))
CORES=$(( $CORES_AVAIL < $MEM_CORES ? $CORES_AVAIL : $MEM_CORES ))

#check submodules
if [ $(( $(git submodule status --recursive | grep -c "^[+\-]") )) -gt 0 ]; then
   printf "\\n\\tgit submodules are not up to date.\\n"
   printf "\\tPlease run the command 'git submodule update --init --recursive'.\\n"
   exit 1
fi

mkdir -p build
pushd build &> /dev/null

if [ -z "$CMAKE" ]; then
  CMAKE=$( command -v cmake 2>/dev/null )
fi

printf "\\n\\n======================= Starting EOSIO.CDT Build =======================\\n\\n"

"$CMAKE" -DCMAKE_INSTALL_PREFIX=/usr/local/eosio.cdt ../
if [ $? -ne 0 ]; then
   exit -1;
fi
make -j${CORES}
if [ $? -ne 0 ]; then
   exit -1;
fi
popd &> /dev/null

printf "\n${bldred}\t      ___           ___           ___                       ___\n"
printf "\t     /  /\\         /  /\\         /  /\\        ___          /  /\\ \n"
printf "\t    /  /:/_       /  /::\\       /  /:/_      /  /\\        /  /::\\ \n"
printf "\t   /  /:/ /\\     /  /:/\\:\\     /  /:/ /\\    /  /:/       /  /:/\\:\\ \n"
printf "\t  /  /:/ /:/_   /  /:/  \\:\\   /  /:/ /::\\  /__/::\\      /  /:/  \\:\\ \n"
printf "\t /__/:/ /:/ /\\ /__/:/ \\__\\:\\ /__/:/ /:/\\:\\ \\__\\/\\:\\__  /__/:/ \\__\\:\\ \n"
printf "\t \\  \\:\\/:/ /:/ \\  \\:\\ /  /:/ \\  \\:\\/:/~/:/    \\  \\:\\/\\ \\  \\:\\ /  /:/ \n"
printf "\t  \\  \\::/ /:/   \\  \\:\\  /:/   \\  \\::/ /:/      \\__\\::/  \\  \\:\\  /:/ \n"
printf "\t   \\  \\:\\/:/     \\  \\:\\/:/     \\__\\/ /:/       /__/:/    \\  \\:\\/:/ \n"
printf "\t    \\  \\::/       \\  \\::/        /__/:/        \\__\\/      \\  \\::/ \n"
printf "\t     \\__\\/         \\__\\/         \\__\\/                     \\__\\/ \n${txtrst}"

printf "\\tFor more information:\\n"
printf "\\tEOSIO website: https://eos.io\\n"
