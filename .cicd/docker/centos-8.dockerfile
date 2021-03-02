FROM centos:8
# install dependencies
RUN yum update -y && \
    yum install -y gdisk && \
    yum install -y cmake && \
    yum install -y git autoconf automake bzip2 \
    libtool make \
    libicu-devel.x86_64 bzip2.x86_64 bzip2-devel.x86_64 openssl-devel.x86_64 \
    gmp-devel.x86_64 python38 python3-devel gettext-devel.x86_64 gcc-c++.x86_64 perl
