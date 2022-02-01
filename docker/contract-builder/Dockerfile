FROM blockone-b1x-taurus-docker-local.jfrog.io/taurus/taurus-node/pinned/base:eos-ubuntu-18.04-pinned-db41af5779c750edbeee053c80e46d0dcbb4e196/
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
COPY . eosio.cdt

RUN mkdir -p eosio.cdt/build \
 && rm -rf eosio.cdt/eos \
 && cd eosio.cdt/build \
 && cmake -GNinja -DCMAKE_BUILD_TYPE=Release .. \
 && ninja

### eos
ENV PATH="/root/eos/build/bin/:/root/eosio.cdt/build/bin/:${PATH}"

### More deps
RUN apt-get install -y colordiff nodejs \
 && npm i -g yarn