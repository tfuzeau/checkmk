ARG DOCKER_REGISTRY
FROM ${DOCKER_REGISTRY}/sles-12sp3-base

SHELL ["/bin/bash", "-c"]

ARG ARTIFACT_STORAGE
ARG PACKAGES

RUN zypper addrepo -G ${ARTIFACT_STORAGE}/repository/sles12sp3 sles12sp3 \
    && zypper addrepo -G ${ARTIFACT_STORAGE}/repository/sles12spx-web-scripting web-scripting \
    && zypper -n --no-gpg-checks in --replacefiles \
    curl \
    cyrus-sasl-devel \
    enchant-devel \
    git \
    krb5-devel \
    libmysqlclient-devel \
    make \
    nodejs \
    npm \
    openldap2-devel \
    python \
    python-devel \
    python-openssl \
    python-xml \
    rrdtool-devel \
    sudo \
    && zypper clean -a
RUN curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py \
    && python get-pip.py
RUN pip install --upgrade setuptools \
    && pip install https://github.com/smokey42/pipenv/archive/last-checked.tar.gz \
    && zypper -n --no-gpg-checks in --replacefiles \
    $PACKAGES \
    && zypper clean -a

ARG VERS_TAG
RUN echo $VERS_TAG > /version.txt
