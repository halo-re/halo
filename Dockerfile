FROM ubuntu:22.04
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -qy \
      build-essential \
      clang \
      libclang-dev \
      lld \
      python-is-python3 \
      python3-pip \
      gcc-multilib

ADD https://raw.githubusercontent.com/XboxDev/extract-xiso/master/extract-xiso.c /tmp/extract-xiso.c
RUN cc -o /usr/bin/extract-xiso -D__LINUX__ -m32 /tmp/extract-xiso.c

COPY requirements.txt /tmp/requirements.txt
RUN pip install -r /tmp/requirements.txt
