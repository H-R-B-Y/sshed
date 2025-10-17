FROM ubuntu:24.04

RUN apt update && apt install -y \
    build-essential cmake git pkg-config \
    notcurses-bin libnotcurses-dev \
	libtinfo-dev \
    && apt clean

WORKDIR /workspace