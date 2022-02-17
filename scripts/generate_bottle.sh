#! /bin/bash

VERS=`sw_vers -productVersion | awk '/^10\.15\..*/{print $0}'`
if [[ -z "$VERS" ]];
then
   VERS=`sw_vers -productVersion | awk '/^11\..*/{print $0}'`
   if [[ -z "$VERS" ]];
   then
      echo "Error, unsupported MacOS version"
      exit -1
   fi
   MAC_VERSION="big_sur"
else
   MAC_VERSION="catalina"
fi

NAME_NO_EXT="${PROJECT}-${VERSION}.${MAC_VERSION}.bottle"
NAME="${NAME_NO_EXT}.tar.gz"

#mkdir -p ${PROJECT}/${VERSION}/opt/eosio_cdt/lib/cmake

PREFIX="${PROJECT}/${VERSION}"
SPREFIX="usr\/local"
SUBPREFIX="opt/${PROJECT}"
SSUBPREFIX="opt\/${PROJECT}\/opt\/${PROJECT}"

export PREFIX
export SPREFIX
export SUBPREFIX
export SSUBPREFIX

cpack --config $BUILD_DIR/CPackConfig.cmake -G TGZ -D CPACK_INCLUDE_TOPLEVEL_DIRECTORY=OFF -D PACK_PACKAGE_FILE_NAME=${NAME_NO_EXT}

hash=`openssl dgst -sha256 ${NAME} | awk 'NF>1{print $NF}'`

echo "class EosioCdt < Formula

   homepage \"${URL}\"
   revision 0
   url \"https://github.com/eosio/eosio.cdt/archive/${VERSION}.tar.gz\"
   version \"${VERSION}\"
   
   option :universal

   depends_on \"cmake\" => :build
   depends_on \"llvm\" => \"13.0.0\"
   depends_on :xcode => :build
   depends_on :macos => :catalina
   depends_on :arch =>  :intel
  
   bottle do
      root_url \"https://github.com/eosio/eosio.cdt/releases/download/${VERSION}\"
      sha256 ${MAC_VERSION}: \"${hash}\"
   end
   def install
      raise \"Error, only supporting binary packages at this time\"
   end
end
__END__" &> ${PROJECT}.rb

rm -r ${PROJECT}
