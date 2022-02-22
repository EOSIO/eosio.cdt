FROM ubuntu:20.04
# install dependencies

RUN apt-get update && apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git cmake make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev python3 \
    autoconf libtool curl zlib1g-dev doxygen graphviz \
    libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev

RUN ln -sfn /usr/bin/python3 /usr/local/bin/python3

#install llvm-13
RUN curl -L https://apt.llvm.org/llvm-snapshot.gpg.key |  apt-key add - && \
    echo 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-13 main' | tee /etc/apt/sources.list.d/llvm.list >/dev/null && \
    apt-get update -y && \
    apt-get install -y clang-13 libclang-13-dev lld-13

ENV LLVM_ROOT=/usr/lib/llvm-13
ENV Clang_ROOT=/usr/lib/llvm-13
