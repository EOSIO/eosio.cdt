FROM ubuntu:16.04

# APT-GET dependencies.
RUN apt-get update && apt-get upgrade -y \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y git clang-4.0 \
  lldb-4.0 libclang-4.0-dev cmake make automake libbz2-dev libssl-dev \
	libgmp3-dev autotools-dev build-essential libicu-dev python2.7-dev \
  python3-dev autoconf libtool curl zlib1g-dev doxygen graphviz

# Build appropriate version of ccache.
RUN curl -LO https://github.com/ccache/ccache/releases/download/v3.4.1/ccache-3.4.1.tar.gz \
  && tar -xzf ccache-3.4.1.tar.gz \
  && cd ccache-3.4.1 \
  && ./configure \
  && make \
  && make install \
  && cd / && rm -rf ccache-3.4.1/
