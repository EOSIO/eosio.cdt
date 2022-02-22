FROM ubuntu:18.04
# install dependencies

RUN apt-get update && apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev \
    autoconf libtool curl zlib1g-dev \
    libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev ninja-build

# install Python 3.7.4
RUN curl -LO https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz && \
    tar xzf Python-3.7.4.tgz && \
    cd Python-3.7.4 && \
    ./configure --enable-optimizations && \
    make -j$(nproc) altinstall  && \
    cd .. && \
    rm -rf Python-3.7.4 && rm -rf Python-3.7.4.tar.gz

RUN ln -sfn /usr/local/bin/python3.7 /usr/local/bin/python3

# install cmake
RUN curl -L https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null &&  \
    apt-get update && apt-get install -y cmake

#install llvm-13
RUN curl -L https://apt.llvm.org/llvm-snapshot.gpg.key|  apt-key add - && \
    echo 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-13 main' | tee /etc/apt/sources.list.d/llvm.list >/dev/null && \
    apt-get update -y && \
    apt-get install -y clang-13 libclang-13-dev lld-13

ENV LLVM_ROOT=/usr/lib/llvm-13
ENV Clang_ROOT=/usr/lib/llvm-13
