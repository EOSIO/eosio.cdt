FROM centos:7.6.1810
# install dependencies
RUN yum update -y && \
    yum --enablerepo=extras install -y centos-release-scl && \
    yum install -y devtoolset-7 && \
    yum install -y python33.x86_64 git autoconf automake bzip2 \
    libtool ocaml.x86_64 doxygen graphviz-devel.x86_64 \
    libicu-devel.x86_64 bzip2.x86_64 bzip2-devel.x86_64 openssl-devel.x86_64 \
    gmp-devel.x86_64 python-devel.x86_64 gettext-devel.x86_64 gcc-c++.x86_64 perl
# build lcov
RUN git clone https://github.com/linux-test-project/lcov.git && \
    source /opt/rh/python33/enable && \
    source /opt/rh/devtoolset-7/enable && \
    cd lcov && \
    make install && \
    cd / && \
    rm -rf lcov/
# build cmake
RUN curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz && \
    source /opt/rh/python33/enable && \
    source /opt/rh/devtoolset-7/enable && \
    tar -xzf cmake-3.10.2.tar.gz && \
    cd cmake-3.10.2 && \
    ./bootstrap --prefix=/usr/local && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -f cmake-3.10.2.tar.gz