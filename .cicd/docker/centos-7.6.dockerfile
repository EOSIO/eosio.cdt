FROM centos:7

# YUM dependencies.
RUN yum update -y \
  && yum --enablerepo=extras install -y centos-release-scl \
  && yum install -y devtoolset-7 \
  && yum install -y python33.x86_64 git autoconf automake bzip2 \
  libtool ocaml.x86_64 doxygen graphviz-devel.x86_64 \
  libicu-devel.x86_64 bzip2.x86_64 bzip2-devel.x86_64 openssl-devel.x86_64 \
  gmp-devel.x86_64 python-devel.x86_64 gettext-devel.x86_64 gcc-c++.x86_64 perl

# Build appropriate version of lcov.
RUN git clone https://github.com/linux-test-project/lcov.git \
  && source /opt/rh/python33/enable \
  && source /opt/rh/devtoolset-7/enable \
  && cd lcov \
  && make install \
  && cd / \
  && rm -rf lcov/

# Build appropriate version of CMake.
RUN curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz \
  && source /opt/rh/python33/enable \
  && source /opt/rh/devtoolset-7/enable \
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