FROM debian:9

SHELL ["/bin/bash", "-c"]
ENV DEBIAN_FRONTEND=noninteractive
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

ARG PACKAGES

RUN apt-get update \
    && apt-get install -y \
    default-libmysqlclient-dev \
    gdebi \
    git \
    libenchant1c2a \
    libkrb5-dev \
    libldap2-dev \
    librrd-dev \
    libsasl2-dev \
    libssl-dev \
    make \
    python-dev \
    python-dev \
    python3-pip \
    rrdtool \
    strace \
    vim \
    && pip3 install --upgrade setuptools \
    && pip3 install https://github.com/smokey42/pipenv/archive/last-checked.tar.gz \
    && curl -sL https://deb.nodesource.com/setup_12.x | bash - \
    && apt-get install -y nodejs \
    && apt-get install -y \
    $PACKAGES \
    && rm -rf /var/lib/apt/lists/*

RUN rm -rf /bin/systemctl \
    && ln -s /bin/true /bin/systemctl

ARG VERS_TAG
RUN echo $VERS_TAG > /version.txt
