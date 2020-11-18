FROM ubuntu:18.04
# aptitude
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y ca-certificates curl openssl wget && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
# Install CDT from deb package
ADD build/packages/*.deb /
RUN for filename in $(ls *.deb); do /usr/bin/dpkg -i "$filename" && rm -f "$filename"; done

USER root
