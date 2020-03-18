#!/usr/bin/env bash

SCRIPT_VERSION=1.0

TIME_BEGIN=$( date -u +%s )
export CURRENT_WORKING_DIR=$(pwd) # relative path support

# Ensure we're in the repo root and not inside of scripts
cd $( dirname "${BASH_SOURCE[0]}" )/..

# Load eosio specific helper functions
. ./scripts/helpers/eosio.sh

$VERBOSE && echo "Build Script Version: ${SCRIPT_VERSION}"
echo "EOSCC Version: ${EOSIO_VERSION_FULL}"
echo "$( date -u )"
echo "User: ${CURRENT_USER}"
# echo "git head id: %s" "$( cat .git/refs/heads/master )"
echo "Current branch: $( execute git rev-parse --abbrev-ref HEAD 2>/dev/null )"

# Ensure sudo is available (only if not using the root user)
ensure-sudo
# Test that which is on the system before proceeding
ensure-which
# Prevent a non-git clone from running
ensure-git-clone
# Prompt user for installation path (Set EOSIO_INSTALL_DIR)
install-directory-prompt
# If the same version has already been installed...
previous-install-prompt
# Setup directories and envs we need (must come last)
setup

execute cd $REPO_ROOT

# Submodules need to be up to date
ensure-submodules-up-to-date

# Check if cmake already exists
( [[ -z "${CMAKE}" ]] && [[ ! -z $(command -v cmake 2>/dev/null) ]] ) && export CMAKE=$(command -v cmake 2>/dev/null) && export CMAKE_CURRENT_VERSION=$($CMAKE --version | grep -E "cmake version[[:blank:]]*" | sed 's/.*cmake version //g')
# If it exists, check that it's > required version +
if [[ ! -z $CMAKE_CURRENT_VERSION ]] && [[ $((10#$( echo $CMAKE_CURRENT_VERSION | awk -F. '{ printf("%03d%03d%03d\n", $1,$2,$3); }' ))) -lt $((10#$( echo $CMAKE_REQUIRED_VERSION | awk -F. '{ printf("%03d%03d%03d\n", $1,$2,$3); }' ))) ]]; then
   export CMAKE=
   if [[ $ARCH == 'Darwin' ]]; then
      echo "${COLOR_RED}The currently installed cmake version ($CMAKE_CURRENT_VERSION) is less than the required version ($CMAKE_REQUIRED_VERSION). Cannot proceed."
      exit 1
   else
      echo "${COLOR_YELLOW}The currently installed cmake version ($CMAKE_CURRENT_VERSION) is less than the required version ($CMAKE_REQUIRED_VERSION). We will be installing $CMAKE_VERSION.${COLOR_NC}"
   fi
fi

# Use existing cmake on system (either global or specific to eosio)
# Setup based on architecture
if [[ $ARCH == "Linux" ]]; then
   export CMAKE=${CMAKE:-${EOSIO_INSTALL_DIR}/bin/cmake}
   [[ ! -e /etc/os-release ]] && print_supported_linux_distros_and_exit
   case $NAME in
      "Ubuntu")
         FILE="${REPO_ROOT}/scripts/eoscc_build_ubuntu.sh"
      ;;
      *) print_supported_linux_distros_and_exit;;
   esac
   CMAKE_PREFIX_PATHS="${EOSIO_INSTALL_DIR}"
fi

if [ "$ARCH" == "Darwin" ]; then
   CMAKE_PREFIX_PATHS="${EOSIO_INSTALL_DIR}"
   FILE="${SCRIPT_DIR}/eoscc_build_darwin.sh"
   export CMAKE=${CMAKE}
fi

echo "${COLOR_CYAN}====================================================================================="
echo "======================= ${COLOR_WHITE}Starting EOSCC Dependency Install${COLOR_CYAN} ===========================${COLOR_NC}"
execute cd $SRC_DIR
set_system_vars # JOBS, Memory, disk space available, etc
echo "Architecture: ${ARCH}"
. $FILE # Execute OS specific build file
execute cd $REPO_ROOT

echo ""
echo "${COLOR_CYAN}========================================================================"
echo "======================= ${COLOR_WHITE}Starting EOSCC Build${COLOR_CYAN} ===========================${COLOR_NC}"
if $VERBOSE; then
   echo "CXX: $CXX"
   echo "CC: $CC"
fi
execute cd $BUILD_DIR

execute bash -c "$CMAKE -DCMAKE_BUILD_TYPE='${CMAKE_BUILD_TYPE}' -DCMAKE_INSTALL_PREFIX='${EOSIO_INSTALL_DIR}' '${REPO_ROOT}'"
if ! execute make -j$JOBS; then
   echo ""
   echo "${COLOR_RED}Building EOSCC has exited with the above error.${COLOR_NC}"
   exit -1
fi
execute cd $REPO_ROOT 1>/dev/null

TIME_END=$(( $(date -u +%s) - $TIME_BEGIN ))

echo "                                         "
echo "███████╗ ██████╗ ███████╗ ██████╗ ██████╗"
echo "██╔════╝██╔═══██╗██╔════╝██╔════╝██╔════╝"
echo "█████╗  ██║   ██║███████╗██║     ██║     "
echo "██╔══╝  ██║   ██║╚════██║██║     ██║     "
echo "███████╗╚██████╔╝███████║╚██████╗╚██████╗"
echo "╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝"
echo "=========================================${COLOR_NC}"

echo "${COLOR_GREEN}EOSCC has been successfully built. $(($TIME_END/3600)):$(($TIME_END%3600/60)):$(($TIME_END%60))"
echo "${COLOR_GREEN}You can now install using: ${SCRIPT_DIR}/eoscc_install.sh${COLOR_NC}"
echo "${COLOR_YELLOW}Uninstall with: ${SCRIPT_DIR}/eoscc_uninstall.sh${COLOR_NC}"

echo ""
resources
