FROM ubuntu:16.04
# install dependencies
RUN apt-get update && apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-4.0 \
    lldb-4.0 libclang-4.0-dev make automake libbz2-dev libssl-dev \
	  libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
    autoconf libtool curl zlib1g-dev doxygen graphviz \
    wget libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev

# install cmake
RUN curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz && \
    tar -xzf cmake-3.10.2.tar.gz && \
    cd cmake-3.10.2 && \
    ./bootstrap --prefix=/usr/local && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -f cmake-3.10.2.tar.gz

# install Python 3.7.4
RUN curl -LO https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz && \
    tar xzf Python-3.7.4.tgz && \
    cd Python-3.7.4 && \
    ./configure --enable-optimizations && \
    make -j$(nproc) altinstall  && \
    cd .. && \
    rm -rf Python-3.7.4 && rm -rf Python-3.7.4.tar.gz

RUN ln -sfn /usr/local/bin/python3.7 /usr/local/bin/python3
