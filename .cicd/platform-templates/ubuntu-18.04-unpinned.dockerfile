FROM ubuntu:18.04
ENV EOSIO_CDT_LOCATION=/root/eosio/cdt
ENV EOSIO_CDT_INSTALL_LOCATION=/root/eosio/install
ENV VERSION 1
# Commands from the documentation are inserted right below this line
# Anything below here is exclusive to our CI/CD
## Set WORKDIR to location we mount into the container
WORKDIR /root
## install ccache
RUN apt-get install -y ccache
## Cleanup EOSIO.CDT directory 
RUN rm -rf ${EOSIO_CDT_LOCATION}