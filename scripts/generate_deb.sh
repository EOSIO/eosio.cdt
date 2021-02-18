#! /bin/bash

OS=$1

PREFIX="usr"
SPREFIX=${PREFIX}
SUBPREFIX="opt/${PROJECT}/${VERSION}"
SSUBPREFIX="opt\/${PROJECT}\/${VERSION}"
RELEASE="${VERSION_SUFFIX}"

# default release to "1" if there is no suffix
if [[ -z $RELEASE ]]; then
  RELEASE="1"
fi

NAME="${PROJECT}_${VERSION_NO_SUFFIX}-${RELEASE}_amd64"

mkdir -p ${PROJECT}/DEBIAN
echo "Package: ${PROJECT} 
Version: ${VERSION_NO_SUFFIX}-${RELEASE}
Section: devel
Priority: optional
Architecture: amd64
Homepage: ${URL} 
Maintainer: ${EMAIL} 
Description: ${DESC}" &> ${PROJECT}/DEBIAN/control
cat ${PROJECT}/DEBIAN/control

export PREFIX
export SUBPREFIX
export SPREFIX
export SSUBPREFIX

. ./generate_tarball.sh ${NAME} ${OS}
echo "Unpacking tarball: ${NAME}.tar.gz..."
tar -xzvf ${NAME}.tar.gz -C ${PROJECT} || exit 1
dpkg-deb --build ${PROJECT} || exit 1
mv ${PROJECT}.deb ${NAME}.deb || exit 1
rm -r ${PROJECT} || exit 1

exit $BUILDSTATUS
