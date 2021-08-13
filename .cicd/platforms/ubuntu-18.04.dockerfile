FROM ubuntu:18.04
# install dependencies

RUN apt-get update && apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-4.0 \
    lldb-4.0 libclang-4.0-dev cmake make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
    autoconf libtool curl zlib1g-dev doxygen graphviz \
    libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev

# install Python 3.7.4
RUN curl -LO https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz && \
    tar xzf Python-3.7.4.tgz && \
    cd Python-3.7.4 && \
    ./configure --enable-optimizations && \
    make -j$(nproc) altinstall  && \
    cd .. && \
    rm -rf Python-3.7.4 && rm -rf Python-3.7.4.tar.gz

RUN ln -sfn /usr/local/bin/python3.7 /usr/local/bin/python3
