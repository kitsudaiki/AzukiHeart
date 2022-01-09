FROM ubuntu:20.04

RUN apt-get update; \
    apt-get install -y openssl libuuid1 libcrypto++6 libsqlite3-0; \
    mkdir /etc/azuki
COPY upload/AzukiHeart /usr/bin/AzukiHeart
CMD AzukiHeart
