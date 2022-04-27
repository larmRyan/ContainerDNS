FROM ubuntu:18.04
RUN apt-get update -y
RUN apt-get install iputils-ping -y
RUN apt-get install net-tools -y
