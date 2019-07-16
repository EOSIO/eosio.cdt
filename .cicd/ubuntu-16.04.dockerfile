FROM ubuntu:18.04

# APT-GET dependencies.
RUN apt-get update && apt-get upgrade -y \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-4.0 \
  lldb-4.0 libclang-4.0-dev cmake make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
  python3-dev autoconf libtool curl zlib1g-dev doxygen graphviz

# Actually build the thing.
RUN git clone https://github.com/EOSIO/eosio.cdt \
  && cd eosio.cdt \
  && git checkout release/1.6.x \
  && git submodule update --init --recursive \
  && mkdir build \
  && cd build \
  && cmake /eosio.cdt \
  && make -j12 \
  && ctest -j12 -L unit_tests -V -T Test
  