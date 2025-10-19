FROM ubuntu:24.04

# Install dependencies and notcurses from Ubuntu packages
RUN apt update && apt install -y \
	build-essential cmake git pkg-config \
	libtinfo-dev gdb \
	notcurses-bin libnotcurses-dev \
	locales \
	curl unzip \
	&& apt clean

# Configure locales 
RUN locale-gen en_US.UTF-8 \
	&& update-locale LANG=en_US.UTF-8 LANGUAGE=en_US:en LC_ALL=en_US.UTF-8

ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

WORKDIR /workspace
