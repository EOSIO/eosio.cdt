#! /bin/bash

PREFIX="usr"
SPREFIX=${PREFIX}
SUBPREFIX="opt/${PACKAGE}/${VERSION}"
SSUBPREFIX="opt\/${PACKAGE}\/${VERSION}"
RELEASE="${VERSION_SUFFIX}"

# default release to "1" if there is no suffix
if [[ -z $RELEASE ]]; then
  RELEASE="1"
fi

NAME="${PACKAGE}_${VERSION_NO_SUFFIX}-${RELEASE}_amd64"

mkdir -p ${PACKAGE}/DEBIAN
echo "Package: ${PACKAGE}
Version: ${VERSION_NO_SUFFIX}-${RELEASE}
Provides: ${PROJECT}
Conflicts: ${PROJECT}
Replaces: ${PROJECT}
Section: devel
Priority: optional
Architecture: amd64
Homepage: ${URL} 
Maintainer: ${EMAIL} 
Description: ${DESC}" &> ${PACKAGE}/DEBIAN/control

export PREFIX
export SUBPREFIX
export SPREFIX
export SSUBPREFIX

bash generate_tarball.sh ${NAME}.tar.gz

tar -xvzf ${NAME}.tar.gz -C ${PACKAGE}
dpkg-deb --build ${PACKAGE}
BUILDSTATUS=$?
mv ${PACKAGE}.deb ${NAME}.deb
rm -r ${PACKAGE}

exit $BUILDSTATUS
