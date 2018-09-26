class EosioCdt < Formula

   homepage "https://github.com/eosio/eosio.cdt"
   revision 0
   url "https://github.com/larryk85/eosio.cdt/archive/v1.2.1.tar.gz"
   version "1.2.1"
   #sha1 yada yada
   
   option :universal

   depends_on "cmake" => :build
   depends_on :xcode => :build
   depends_on :macos => :leopard
   depends_on :arch =>  :intel
   
   bottle do
      root_url "https://github.com/larryk85/test/releases/download/1.0.5"
      #cellar :any_skip_relocation
      sha256 "4b69ccf2100acda42a069a14ffb602cf6ea2157dac92d349a363229f08e869bb" => :high_sierra
   end
   def install
      system "./build.sh"
      system "./install.sh"
   end
end
__END__
