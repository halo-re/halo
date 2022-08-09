FROM ubuntu:22.04
RUN DEBIAN_FRONTEND=noninteractive apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -qy \
      build-essential \
      cmake \
      clang \
      lld \
      python3-pip \
      git

COPY requirements.txt /tmp/requirements.txt
RUN pip install -r /tmp/requirements.txt
