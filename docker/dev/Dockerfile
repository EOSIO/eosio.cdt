FROM ubuntu:18.04

# Install required packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates curl wget && rm -rf /var/lib/apt/lists/*

# Install CDT from deb package
ADD *.deb /
RUN for filename in $(ls builder/packages/*.deb); do /usr/bin/dpkg -i "$filename" && rm -f "$filename"; done

USER root