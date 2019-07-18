FROM ubuntu:18.04

COPY ./ClangExternalProject.txt /tmp/ClangExternalProject.txt

# APT-GET dependencies.
RUN apt-get update && apt-get upgrade -y \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-4.0 \
  lldb-4.0 libclang-4.0-dev cmake make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
  python3-dev autoconf libtool curl zlib1g-dev doxygen graphviz

# EOSIO_LLVM requires things in the main repo. Make it in a different location so we can reference it later.
RUN cd /opt && git clone https://github.com/EOSIO/eosio.cdt \
  && cd eosio.cdt \
  && git checkout master \
  && git submodule update --init --recursive \
  && mkdir build && cd build \
  && cmake .. \
  && make -j$(nproc) \
  && cd / \