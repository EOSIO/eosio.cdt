FROM ubuntu:20.04
# install dependencies

RUN apt-get update && apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-8 \
    lldb-8 libclang-8-dev cmake make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev python3 \
    autoconf libtool curl zlib1g-dev doxygen graphviz \
    libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev

RUN ln -sfn /usr/bin/python3 /usr/local/bin/python3
