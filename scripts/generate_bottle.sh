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

NAME="${PROJECT}-${VERSION}.${MAC_VERSION}.bottle.tar.gz"

#mkdir -p ${PROJECT}/${VERSION}/opt/eosio_cdt/lib/cmake

PREFIX="${PROJECT}/${VERSION}"
SPREFIX="\/usr\/local"
SUBPREFIX="opt/${PROJECT}"
SSUBPREFIX="opt\/${PROJECT}"

export PREFIX
export SPREFIX
export SUBPREFIX
export SSUBPREFIX

bash generate_tarball.sh ${NAME}

hash=`openssl dgst -sha256 ${NAME} | awk 'NF>1{print $NF}'`

echo "class Blanc < Formula

   homepage \"${URL}\"
   revision 0
   url \"https://github.com/turnpike/blanc/archive/${VERSION}.tar.gz\"
   version \"${VERSION}\"
   
   option :universal

   depends_on \"cmake\" => :build
   depends_on \"llvm\" => :build
   depends_on :xcode => :build
   depends_on :macos => :catalina
   depends_on :arch =>  :intel
  
   bottle do
      root_url \"https://github.com/turnpike/blanc/releases/download/${VERSION}\"
      sha256 ${MAC_VERSION}: \"${hash}\"
   end
   def install
      raise \"Error, only supporting binary packages at this time\"
   end
end
__END__" &> ${PROJECT}.rb

rm -r ${PROJECT}
