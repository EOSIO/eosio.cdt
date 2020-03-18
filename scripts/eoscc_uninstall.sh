#!/usr/bin/env bash

VERSION=1.0
# User input handling
DEP_PROCEED=false

function usage() {
   printf "Usage: $0 OPTION...
  -i DIR      Directory where eosio is installed)
  -y          Noninteractive mode (answers yes to every prompt)
   \\n" "$0" 1>&2
   exit 1
}

TIME_BEGIN=$( date -u +%s )
if [ $# -ne 0 ]; then
   while getopts "i:y" opt; do
      case "${opt}" in
         i )
            INSTALL_LOCATION=$OPTARG
         ;;
         y )
            NONINTERACTIVE=true
            PROCEED=true
            DEP_PROCEED=true
         ;;
         h )
            usage
         ;;
         ? )
            echo "Invalid Option!" 1>&2
            usage
         ;;
         : )
            echo "Invalid Option: -${OPTARG} requires an argument." 1>&2
            usage
         ;;
         * )
            usage
         ;;
      esac
   done
fi

export CURRENT_WORKING_DIR=$(pwd) # relative path support

# Ensure we're in the repo root and not inside of scripts
cd $( dirname "${BASH_SOURCE[0]}" )/..

# Load bash script helper functions
. ./scripts/helpers/eosio.sh

# Support relative paths : https://github.com/EOSIO/eos/issues/7560
( [[ ! -z $INSTALL_LOCATION ]] && [[ ! $INSTALL_LOCATION =~ ^\/ ]] ) && export INSTALL_LOCATION="${CURRENT_WORKING_DIR}/$INSTALL_LOCATION"

INSTALL_PATHS=()

export EOSIO_INSTALL_DIR=${INSTALL_LOCATION:-$EOSIO_INSTALL_DIR}

function uninstall() {
   if [ -d "$1" ]; then
      echo "[$2 installation found: $1]" && INSTALL_PATHS+=("$1") # EOSIO_INSTALL_DIR set in .environment
      while true; do
         [[ $NONINTERACTIVE == false ]] && read -p "Do you wish to remove the installation in $1? (y/n) " PROCEED
         case $PROCEED in
            "" ) echo "What would you like to do?";;
            0 | true | [Yy]* )
               if [[ $3 && "$(id -u)" -ne 0 ]]; then
                  echo "This requires sudo, please run ./eoscc_uninstall.sh with sudo"
                  echo ""
                  exit -1
               fi
               # Version < 1.8.0; Before we started using ~/eosio/1.8.x
               # Arrays should return with newlines (IFS=\n;helpers.sh) as Application\ Support will split into two
               for INSTALL_PATH in ${INSTALL_PATHS[@]}; do
                  execute rm -rf $INSTALL_PATH
               done
               echo " - $2 Removal Complete"
               break;;
            1 | false | [Nn]* ) echo " - Cancelled $2 Removal!"; exit -1;;
            * ) echo "Please type 'y' for yes or 'n' for no.";;
         esac
      done
   else
      echo "[$2 installation ${COLOR_YELLOW}NOT${COLOR_NC} found in $1]"
   fi
   exit 1
}
               
uninstall "${EOSIO_INSTALL_DIR}" "eoscc"
