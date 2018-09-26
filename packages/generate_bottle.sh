#! /bin/bash

VERSION=$1
NAME="eosio.cdt-${VERSION}.high_sierra.bottle.tar.gz"

./generate_tarball.sh ${VERSION} eosio.cdt/${VERSION} eosio.cdt/${VERSION}/opt/eosio_cdt ${NAME}

hash=`openssl dgst -sha256 ${NAME} | awk 'NF>1{print $NF}'`

echo "class EosioCdt < Formula

   homepage \"https://github.com/eosio/eosio.cdt\"
   revision 0
   url \"https://github.com/larryk85/eosio.cdt/archive/v${VERSION}.tar.gz\"
   version \"${VERSION}\"
   #sha1 yada yada
   
   option :universal

   depends_on \"cmake\" => :build
   depends_on :xcode => :build
   depends_on :macos => :leopard
   depends_on :arch =>  :intel
   
   bottle do
      root_url \"https://github.com/larryk85/test/releases/download/1.0.5\"
      #cellar :any_skip_relocation
      sha256 \"${hash}\" => :high_sierra
   end
   def install
      system \"./build.sh\"
      system \"./install.sh\"
   end
end
__END__" &> eosio.cdt.rb
