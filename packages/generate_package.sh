#! /bin/bash

VERSION="1.2.1"
VARIANT=$1

mkdir tmp

if [[ ${VARIANT} == "brew" ]]; then
   bash generate_bottle.sh ${VERSION}
elif [[ ${VARIANT} == "deb" ]]; then
   bash generate_deb.sh ${VERSION}
else
   echo "Error, unknown package type. Use either ['brew', 'deb', 'rpm']."
   exit -1
fi
rm -r tmp
