FROM ubuntu:15.04

ENV DEBIAN_FRONTEND noninteractive 

RUN apt-get update
RUN apt-get install -y g++
RUN apt-get install -y cmake
RUN apt-get install -y pkg-config
RUN apt-get install -y libboost-dev
RUN apt-get install -y libboost-system-dev
RUN apt-get install -y libboost-filesystem-dev
RUN apt-get install -y libboost-thread-dev
RUN apt-get install -y vim
RUN apt-get install -y gdb

RUN adduser me
