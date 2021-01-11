#!/usr/bin/env bash

VERSION=1.0

# Ensure we're in the repo root and not inside of scripts
cd $( dirname "${BASH_SOURCE[0]}" )/..

# Load eosio specific helper functions
. ./scripts/helpers/eosio.sh

[[ ! -f ${BUILD_DIR}/CMakeCache.txt ]] && printf "${COLOR_RED}Please run ${SCRIPT_DIR}/blanc_build.sh first!${COLOR_NC}" && exit 1
echo "${COLOR_CYAN}====================================================================================="
echo "========================== ${COLOR_WHITE}Starting Blanc Installation${COLOR_CYAN} ==============================${COLOR_NC}"
execute cd $BUILD_DIR
if ! execute make install; then
   echo ""
   echo "${COLOR_RED}Installing Blanc has exited with the above error.${COLOR_NC}"
   exit -1
fi
execute cd ..

echo "                                          "
echo "██████╗ ██╗      █████╗ ███╗   ██╗ ██████╗"
echo "██╔══██╗██║     ██╔══██╗████╗  ██║██╔════╝"
echo "██████╔╝██║     ███████║██╔██╗ ██║██║     "
echo "██╔══██╗██║     ██╔══██║██║╚██╗██║██║     "
echo "██████╔╝███████╗██║  ██║██║ ╚████║╚██████╗"
echo "╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝"
echo "=========================================${COLOR_NC}"

echo "=============================================================================================="
echo "${COLOR_GREEN}Blanc has been installed into ${CACHED_INSTALL_PATH}/bin${COLOR_NC}"
echo "${COLOR_YELLOW}Uninstall with: ${SCRIPT_DIR}/blanc_uninstall.sh${COLOR_NC}"
echo "=============================================================================================="

echo ""
resources
