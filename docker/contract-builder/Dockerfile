FROM ubuntu:18.04

ARG CDT_BRANCH
ARG EOS_BRANCH

RUN apt-get update \
 && apt-get install -yq \
     binutils-gold \
     build-essential \
     clang-tools-8 \
     curl \
     g++-8 \
     git \
     libcurl4-gnutls-dev \
     libgflags-dev \
     libgmp3-dev \
     libssl-dev \
     libusb-1.0-0-dev \
     lld-8 \
     llvm-7 \
     llvm-7-dev \
     locales \
     ninja-build \
     pkg-config \
     python \
     software-properties-common \
     wget \
     xz-utils \
     zlib1g-dev \
 && update-alternatives --remove-all cc \
 && update-alternatives --install /usr/bin/cc cc /usr/bin/gcc-8 100 \
 && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 100 \
 && update-alternatives --remove-all c++ \
 && update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-8 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 100 \
 && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-8 100 \
 && locale-gen en_US.UTF-8 \
 && curl -sL https://deb.nodesource.com/setup_10.x | bash -

ENV LANG=en_US.UTF-8

### cmake
WORKDIR /root
RUN curl -LO https://cmake.org/files/v3.13/cmake-3.13.2.tar.gz \
 && tar -xzf cmake-3.13.2.tar.gz \
 && cd cmake-3.13.2 \
 && ./bootstrap --prefix=/usr/local --parallel=8 \
 && make -j$(getconf _NPROCESSORS_ONLN) \
 && make install \
 && cd /root \
 && rm -rf cmake-3.13.2.tar.gz cmake-3.13.2

### boost
RUN curl -LO https://boostorg.jfrog.io/artifactory/main/release/1.72.0/source/boost_1_72_0.tar.bz2 \
 && tar -xjf boost_1_72_0.tar.bz2 \
 && cd boost_1_72_0 \
 && ./bootstrap.sh --prefix=/usr/local \
 && ./b2 --with-iostreams --with-date_time --with-filesystem --with-system --with-program_options --with-chrono --with-test -j$(nproc) install \
 && cd /root \
 && rm -rf boost_1_72_0.tar.bz2 boost_1_72_0

### CDT
RUN git clone https://github.com/b1-as/taurus-cdt.git eosio.cdt \
 && cd eosio.cdt \
 && git checkout ${CDT_BRANCH} \
 && git submodule update --init --recursive
RUN mkdir -p eosio.cdt/build \
 && cd eosio.cdt/build \
 && cmake -GNinja -DCMAKE_BUILD_TYPE=Release .. \
 && ninja

### eos
RUN git clone https://github.com/b1-as/taurus-node.git eos \
 && cd eos \
 && git checkout ${EOS_BRANCH} \
 && git submodule update --init --recursive

RUN mkdir -p eos/build \
 && cd eos/build \
 && CC=clang-8 CXX=clang++-8 cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_AR=/usr/bin/llvm-ar-8 -DCMAKE_RANLIB=/usr/bin/llvm-ranlib-8 -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld -DPORTABLE=1 .. \
 && CC=clang-8 CXX=clang++-8 ninja

ENV PATH="/root/eos/build/bin/:/root/eosio.cdt/build/bin/:${PATH}"

### More deps
RUN apt-get install -y colordiff nodejs \
 && npm i -g yarn