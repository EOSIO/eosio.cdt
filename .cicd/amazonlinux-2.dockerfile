FROM amazonlinux:2

COPY ./ClangExternalProject.txt /tmp/ClangExternalProject.txt

# YUM dependencies.
RUN yum update -y \
 && yum install -y git gcc.x86_64 gcc-c++.x86_64 autoconf automake libtool make bzip2 \
	bzip2-devel.x86_64 openssl-devel.x86_64 gmp-devel.x86_64 libstdc++.x86_64 \
	python.x86_64 python3-devel.x86_64 libedit-devel.x86_64 doxygen.x86_64 graphviz.x86_64 perl

# Build appropriate version of lcov.
RUN git clone https://github.com/linux-test-project/lcov.git \
  && cd lcov \
  && make install \
  && cd / \
  && rm -rf lcov/

# Build appropriate version of CMake.
RUN curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz \
  && tar -xzf cmake-3.10.2.tar.gz \
  && cd cmake-3.10.2 \
  && ./bootstrap --prefix=/usr/local \
  && make -j$(nproc) \
  && make install \
  && cd .. \
  && rm -f cmake-3.10.2.tar.gz

# EOSIO_LLVM requires things in the main repo. Make it in a different location so we can reference it later.
RUN cd /opt && git clone https://github.com/EOSIO/eosio.cdt \
  && cd eosio.cdt \
  && git checkout master \
  && git submodule update --init --recursive \
  && mkdir build && cd build \
  && cmake .. \
  && make -j$(nproc) \
  && cd /

CMD bash -c " $PRECOMMANDS \
  rm -f /workdir/modules/ClangExternalProject.txt && ln -s /tmp/ClangExternalProject.txt /workdir/modules/ClangExternalProject.txt && \
  mkdir /workdir/build && cd /workdir/build && ln -s /opt/eosio.cdt/build/eosio_llvm/ /workdir/build/eosio_llvm && cmake .. && make -j $(getconf _NPROCESSORS_ONLN) && \
  ctest -j$(getconf _NPROCESSORS_ONLN) -L unit_tests -V -T Test"
