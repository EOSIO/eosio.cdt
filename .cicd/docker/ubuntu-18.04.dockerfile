FROM ubuntu:18.04
# install dependencies
RUN apt-get update && apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-7 \
    lldb-7 libclang-7-dev llvm-7-dev cmake make automake libbz2-dev libssl-dev \
	  libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
    python3-dev autoconf libtool curl zlib1g-dev doxygen graphviz