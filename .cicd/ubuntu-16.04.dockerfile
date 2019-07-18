FROM ubuntu:16.04

COPY ./ClangExternalProject.txt /tmp/ClangExternalProject.txt

# APT-GET dependencies.
RUN apt-get update && apt-get upgrade -y \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-4.0 \
  lldb-4.0 libclang-4.0-dev make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
  python3-dev autoconf libtool curl zlib1g-dev doxygen graphviz

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
