#! /bin/bash

VERSION="1.2.1"
VARIANT=$1

mkdir tmp
#./generate_bottle.sh ${VERSION}
./generate_deb.sh ${VERSION}
rm -r tmp 
