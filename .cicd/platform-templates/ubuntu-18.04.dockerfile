FROM ubuntu:18.04
ENV HOME=/root
ENV VERSION 1
# Commands from the documentation are inserted right below this line
# Anything below here is exclusive to our CI/CD
## Set WORKDIR to location we mount into the container
WORKDIR ${HOME}
## install ccache
RUN apt-get install -y ccache
## Cleanup EOSIO.CDT directory 
RUN rm -rf ${EOSIO_CDT_LOCATION}