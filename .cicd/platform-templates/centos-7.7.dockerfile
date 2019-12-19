FROM centos:7.7.1908
ENV EOSIO_CDT_LOCATION=/root/eosio/cdt
ENV EOSIO_CDT_INSTALL_LOCATION=/root/eosio/install
ENV VERSION 1
# Commands from the documentation are inserted right below this line
# Anything below here is exclusive to our CI/CD
## Set WORKDIR to location we mount into the container
WORKDIR /root
## install ccache
RUN curl -LO http://download-ib01.fedoraproject.org/pub/epel/7/x86_64/Packages/c/ccache-3.3.4-1.el7.x86_64.rpm && \
    yum install -y ccache-3.3.4-1.el7.x86_64.rpm && rm ccache-3.3.4-1.el7.x86_64.rpm
## fix ccache for centos
RUN cd /usr/lib64/ccache && ln -s ../../bin/ccache c++
ENV CCACHE_PATH="/opt/rh/devtoolset-7/root/usr/bin"
## Cleanup EOSIO.CDT directory 
RUN rm -rf ${EOSIO_CDT_LOCATION}