FROM ubuntu:18.04
RUN apt-get update -y
RUN apt-get install iputils-ping -y
RUN apt-get install net-tools -y
RUN apt-get -yqq update && apt-get -yqq install docker.io
CMD ["/bin/bash"]
