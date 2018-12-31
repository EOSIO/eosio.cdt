#!/bin/bash

printf "=========== Building eosio.cdt ===========\n\n"

VERSION=2.0 # Build script version
CMAKE_BUILD_TYPE=Release
export DISK_MIN=20
DOXYGEN=false
ENABLE_COVERAGE_TESTING=false
CORE_SYMBOL_NAME="SYS"
START_MAKE=true

TIME_BEGIN=$( date -u +%s )
txtbld=$(tput bold)
bldred=${txtbld}$(tput setaf 1)
txtrst=$(tput sgr0)

export SRC_LOCATION=${HOME}/src
export OPT_LOCATION=${HOME}/opt
export VAR_LOCATION=${HOME}/var
export ETC_LOCATION=${HOME}/etc
export BIN_LOCATION=${HOME}/bin
export DATA_LOCATION=${HOME}/data
export CMAKE_VERSION_MAJOR=3
export CMAKE_VERSION_MINOR=10
export CMAKE_VERSION_PATCH=2
export CMAKE_VERSION=${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}
export MONGODB_VERSION=3.6.3
export MONGODB_ROOT=${OPT_LOCATION}/mongodb-${MONGODB_VERSION}
export MONGODB_CONF=${ETC_LOCATION}/mongod.conf
export MONGODB_LOG_LOCATION=${VAR_LOCATION}/log/mongodb
export MONGODB_LINK_LOCATION=${OPT_LOCATION}/mongodb
export MONGODB_DATA_LOCATION=${DATA_LOCATION}/mongodb
export MONGO_C_DRIVER_VERSION=1.10.2
export MONGO_C_DRIVER_ROOT=${SRC_LOCATION}/mongo-c-driver-${MONGO_C_DRIVER_VERSION}
export MONGO_CXX_DRIVER_VERSION=3.3
export MONGO_CXX_DRIVER_ROOT=${SRC_LOCATION}/mongo-cxx-driver-$MONGO_CXX_DRIVER_VERSION
export BOOST_VERSION_MAJOR=1
export BOOST_VERSION_MINOR=67
export BOOST_VERSION_PATCH=0
export BOOST_VERSION=${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}_${BOOST_VERSION_PATCH}
export BOOST_ROOT=${SRC_LOCATION}/boost_${BOOST_VERSION}
export BOOST_LINK_LOCATION=${OPT_LOCATION}/boost
export LLVM_CLANG_VERSION=release_40
export LLVM_CLANG_ROOT=${SRC_LOCATION}/llvm-${LLVM_CLANG_VERSION}
export LLVM_DIR=${LLVM_CLANG_ROOT}/lib/cmake/llvm
export WASM_LINK_LOCATION=${OPT_LOCATION}/wasm
export TINI_VERSION=0.18.0
export PATH=${MONGODB_LINK_LOCATION}/bin:${PATH}

# Setup directories
mkdir -p $SRC_LOCATION
mkdir -p $OPT_LOCATION
mkdir -p $VAR_LOCATION
mkdir -p $BIN_LOCATION
mkdir -p $VAR_LOCATION/log
mkdir -p $ETC_LOCATION
mkdir -p $MONGODB_LOG_LOCATION
mkdir -p $MONGODB_DATA_LOCATION

# Use current directory's tmp directory if noexec is enabled for /tmp
if (mount | grep "/tmp " | grep --quiet noexec); then
      mkdir -p $SOURCE_DIR/tmp
      TEMP_DIR="${SOURCE_DIR}/tmp"
      rm -rf $SOURCE_DIR/tmp/*
else # noexec wasn't found
      TEMP_DIR="/tmp"
fi

SOURCE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [ "${SOURCE_DIR}" == "${PWD}" ]; then
   BUILD_DIR="${PWD}/build"
else
   BUILD_DIR="${PWD}"
fi

if [ "$ARCH" == "Linux" ]; then
   export OS_NAME=$( cat /etc/os-release | grep ^NAME | cut -d'=' -f2 | sed 's/\"//gI' )
   case "$OS_NAME" in
      "Amazon Linux AMI"|"Amazon Linux")
         FILE="${SOURCE_DIR}/scripts/eosio_build_amazon.sh"
         CXX_COMPILER=g++
         C_COMPILER=gcc
      ;;
      "CentOS Linux")
         FILE="${SOURCE_DIR}/scripts/eosio_build_centos.sh"
         CXX_COMPILER=g++
         C_COMPILER=gcc
      ;;
      "elementary OS")
         FILE="${SOURCE_DIR}/scripts/eosio_build_ubuntu.sh"
         CXX_COMPILER=clang++-4.0
         C_COMPILER=clang-4.0
      ;;
      "Fedora")
         FILE="${SOURCE_DIR}/scripts/eosio_build_fedora.sh"
         CXX_COMPILER=g++
         C_COMPILER=gcc
      ;;
      "Linux Mint")
         FILE="${SOURCE_DIR}/scripts/eosio_build_ubuntu.sh"
         CXX_COMPILER=clang++-4.0
         C_COMPILER=clang-4.0
      ;;
      "Ubuntu")
         FILE="${SOURCE_DIR}/scripts/eosio_build_ubuntu.sh"
         CXX_COMPILER=clang++-4.0
         C_COMPILER=clang-4.0
      ;;
      "Debian GNU/Linux")
         FILE="${SOURCE_DIR}/scripts/eosio_build_ubuntu.sh"
         CXX_COMPILER=clang++-4.0
         C_COMPILER=clang-4.0
      ;;
      *)
         printf "\\nUnsupported Linux Distribution. Exiting now.\\n\\n"
         exit 1
   esac
fi

if [ "$ARCH" == "Darwin" ]; then
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

. "$FILE" # Execute OS specific build file

# check submodules
if [ $(( $(git submodule status --recursive | grep -c "^[+\-]") )) -gt 0 ]; then
   printf "\\n\\tgit submodules are not up to date.\\n"
   printf "\\tPlease run the command 'git submodule update --init --recursive'.\\n"
   exit 1
fi

mkdir -p $BUILD_DIR
pushd $BUILD_DIR &> /dev/null

if [ -z "${CMAKE}" ]; then
  CMAKE=$( command -v cmake 2>/dev/null )
fi

printf "\\n========================================================================\\n"
printf "======================= Starting EOSIO.CDT Build =======================\\n"

$CMAKE -DCMAKE_INSTALL_PREFIX=$OPT_LOCATION/eosio.cdt ../
if [ $? -ne 0 ]; then exit -1; fi
make -j$CORES
if [ $? -ne 0 ]; then exit -1; fi
popd &> /dev/null

TIME_END=$(( $(date -u +%s) - $TIME_BEGIN ))

printf "\n\n _______  _______  _______ _________ _______\n"
printf '(  ____ \(  ___  )(  ____ \\\\__   __/(  ___  )\n'
printf "| (    \/| (   ) || (    \/   ) (   | (   ) |\n"
printf "| (__    | |   | || (_____    | |   | |   | |\n"
printf "|  __)   | |   | |(_____  )   | |   | |   | |\n"
printf "| (      | |   | |      ) |   | |   | |   | |\n"
printf "| (____/\| (___) |/\____) |___) (___| (___) |\n"
printf "(_______/(_______)\_______)\_______/(_______)\n\n"

printf "\\nEOSIO.CDT has been successfully built. %02d:%02d:%02d\\n\\n" $(($TIME_END/3600)) $(($TIME_END%3600/60)) $(($TIME_END%60))
printf "${txtrst}==============================================================================================\\n"

printf "For more information:\\n"
printf "EOSIO website: https://eos.io\\n"
printf "EOSIO Telegram channel @ https://t.me/EOSProject\\n"
printf "EOSIO resources: https://eos.io/resources/\\n"
printf "EOSIO Stack Exchange: https://eosio.stackexchange.com\\n"
printf "EOSIO wiki: https://github.com/EOSIO/eos/wiki\\n\\n\\n"