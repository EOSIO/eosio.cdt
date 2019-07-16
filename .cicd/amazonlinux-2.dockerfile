FROM amazonlinux:2

# YUM dependencies.
RUN yum update -y \
 && yum install -y git gcc.x86_64 gcc-c++.x86_64 autoconf automake libtool make bzip2 \
	bzip2-devel.x86_64 openssl-devel.x86_64 gmp-devel.x86_64 libstdc++.x86_64 \
	python.x86_64 python3-devel.x86_64 libedit-devel.x86_64 doxygen.x86_64 graphviz.x86_64 perl

# CCACHE
RUN curl -LO http://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/c/ccache-3.3.4-1.el7.x86_64.rpm \
  && yum install -y ccache-3.3.4-1.el7.x86_64.rpm

# Build appropriate version of lcov.
RUN git clone https://github.com/linux-test-project/lcov.git \
  && cd lcov \
  && make install \
  && cd / \
  && rm -rf lcov/

# Build appropriate version of CMake.
RUN curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz \
  && tar -xzf cmake-3.10.2.tar.gz \
  && cd cmake-3.10.2 \
  && ./bootstrap --prefix=/usr/local \
  && make -j$(nproc) \
  && make install \
  && cd .. \
  && rm -f cmake-3.10.2.tar.gz
