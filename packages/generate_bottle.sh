#! /bin/bash

VERSION=$1
NAME="eosio.cdt-${VERSION}.high_sierra.bottle.tar.gz"

mkdir -p eosio.cdt/${VERSION}/opt/eosio_cdt/lib/cmake

sed 's/_PREFIX_/\/usr\/local/g' ../build/modules/EosioWasmToolchainPackage.cmake &> eosio.cdt/${VERSION}/opt/eosio_cdt/lib/cmake/EosioWasmToolchain.cmake

bash generate_tarball.sh ${VERSION} eosio.cdt/${VERSION} eosio.cdt/${VERSION}/opt/eosio_cdt opt/eosio_cdt ${NAME}

hash=`openssl dgst -sha256 ${NAME} | awk 'NF>1{print $NF}'`

echo "class EosioCdt < Formula

   homepage \"https://github.com/eosio/eosio.cdt\"
   revision 0
   url \"https://github.com/larryk85/eosio.cdt/archive/v${VERSION}.tar.gz\"
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
      root_url \"https://github.com/larryk85/test/releases/download/1.0.5\"
      sha256 \"${hash}\" => :high_sierra
   end
   def install
      raise \"Error, only supporting binary packages at this time\"
   end
end
__END__" &> eosio.cdt.rb
