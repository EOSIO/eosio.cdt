FROM centos:7.7.1908
# install dependencies
RUN yum update -y && \
    yum --enablerepo=extras install -y centos-release-scl && \
    yum install -y devtoolset-7 && \
    yum install -y git autoconf automake bzip2 \
    libtool ocaml.x86_64 doxygen graphviz-devel.x86_64 \
    libicu-devel.x86_64 bzip2.x86_64 bzip2-devel.x86_64 openssl-devel.x86_64 \
    gmp-devel.x86_64 gettext-devel.x86_64 gcc-c++.x86_64 perl \
    libffi-devel.x86_64

# build Python 3.7.4
RUN curl -LO https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz && \
    source /opt/rh/devtoolset-7/enable && \
    tar xzf Python-3.7.4.tgz && \
    cd Python-3.7.4 && \
    ./configure --enable-optimizations && \
    make -j$(nproc) altinstall && \
    cd .. && \
    rm -rf Python-3.7.4 && rm -rf Python-3.7.4.tgz

# build lcov
RUN git clone https://github.com/linux-test-project/lcov.git && \
    source /opt/rh/devtoolset-7/enable && \
    cd lcov && \
    make install && \
    cd / && \
    rm -rf lcov/

# build cmake
RUN curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz && \
    source /opt/rh/devtoolset-7/enable && \
    tar -xzf cmake-3.10.2.tar.gz && \
    cd cmake-3.10.2 && \
    ./bootstrap --prefix=/usr/local && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -f cmake-3.10.2.tar.gz

RUN ln -sfn /usr/local/bin/python3.7 /usr/local/bin/python3
