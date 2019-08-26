#! /bin/bash

VERS=`sw_vers -productVersion | awk '/10\.13\..*/{print $0}'`
if [[ -z "$VERS" ]];
then
   VERS=`sw_vers -productVersion | awk '/10\.14.*/{print $0}'`
   if [[ -z "$VERS" ]];
   then
      echo "Error, unsupported OS X version"
      exit -1
   fi
   MAC_VERSION="mojave"
else
   MAC_VERSION="high_sierra"
fi

NAME="${PACKAGE}-${VERSION}.${MAC_VERSION}.bottle.tar.gz"

mkdir -p ${PACKAGE}/${VERSION}/opt/eosio_cdt/lib/cmake

PREFIX="${PACKAGE}/${VERSION}"
SPREFIX="\/usr\/local"
SUBPREFIX="opt/${PACKAGE}"
SSUBPREFIX="opt\/${PACKAGE}"

export PREFIX
export SPREFIX
export SUBPREFIX
export SSUBPREFIX

bash generate_tarball.sh ${NAME}

hash=`openssl dgst -sha256 ${NAME} | awk 'NF>1{print $NF}'`

echo "class Eoscc < Formula

   homepage \"${URL}\"
   revision 0
   url \"https://github.com/turnpike2/eoscc/archive/v${VERSION}.tar.gz\"
   version \"${VERSION}\"
   
   option :universal

   depends_on \"cmake\" => :build
   depends_on \"automake\" => :build
   depends_on \"libtool\" => :build
   depends_on \"wget\" => :build
   depends_on \"gmp\" => :build
   depends_on \"gettext\" => :build
   depends_on \"doxygen\" => :build
   depends_on \"graphviz\" => :build
   depends_on \"lcov\" => :build
   depends_on :xcode => :build
   depends_on :macos => :high_sierra
   depends_on :arch =>  :intel
  
   bottle do
      root_url \"https://github.com/turnpike2/eoscc/releases/download/v${VERSION}\"
      sha256 \"${hash}\" => :${MAC_VERSION}
   end

   conflicts_with \"eosio.cdt\", :because => \"Provides same executables and libraries\"

   def install
      raise \"Error, only supporting binary packages at this time\"
   end
end
__END__" &> ${PACKAGE}.rb

rm -r ${PACKAGE}
