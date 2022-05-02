# Container DNS
Container DNS is meant to keep a a dedicated DNS cache for each container for specific DNS queries it makes. DNS queries that are common across multiple containers would be kept on a cache on the host. This will enable each container to have a smaller DNS cache and will also reduce lookup time since it can query the DNS cache on the host (and get a faster response) if the response to the query was cached.

## Purpose of Each File
1. sniffer.c
    - Listens for and parses DNS requests.

2. PopTracker.c
    - Keeps track of containers and threshold for requests/responses

3. host_updater.c
    - Updates the /etc/hosts file for each container and the host

4. Middleware.c
    - The core of the project. Allows communication between the sniffer and the host_updater.

## Install and test (Tested on Ubuntu 18.04 and docker server 20.10.7)

1. First clone the repository and checkout to the c_version branch:

```bash
git clone https://github.com/larmRyan/ContainerDNS.git
git checkout c_version
```

2. Then, you need to download tshark for your distribution:

```bash
sudo apt-get install tshark
sudo pacman -Sy wireshark-cli
```
3. Then you will also need `docker` and `start/enable` the docker service. 

4. Go to the project directory and create a folder inside called _test_ (this will be used for bind mounting)

5. Build the container using the provided Dockerfile. We named the container _project_
```
docker build -t project .
```

6. Setup up the Environment
- Since docker bind mounts a host directory to a container directory we can not bind mound the /etc directory of the container to the host. Doing so will overwrite the /etc directory of the container with the bind mount on the host. Instead, we need to create a directory structure that will hold the /etc/hosts file for each container. To do this, please follow the following steps
- Create a master directory to hold container directories (we call this <projectdir>/test)
- Inside <projectdir>/test create subdirectories for each container (eg. <projectdir>/test/container1)
- Inside each of the subdirectories, create a hosts file


7. Run the container
```
docker run -it -v <path to project directory>/test/container1/hosts:/etc/hosts project
```
This will lauch the container and give access to a bash shell with the hosts file bind-mounted.

7.1 An Example that Shows Two Containers and One Sub-container
- First launch the first container
```
docker run -it -v <path to project directory>/test/container1/hosts:/etc/hosts project
```
- Then launch the second container
```
docker run -it -v <path to project directory>/test/container2/hosts:/etc/hosts -v /var/run/docker.sock:/var/run/docker.sock project
```
The second bind mount gives the container access to the docker socket on the host
- From within this second container run the following command
```
docker run -d -v <path to project dir>/test/container3/hosts:/etc/hosts project /bin/bash
```
Since container2 had access to the docker socket on the host, it will run a container in itself but will bind mount directories on the host.

8. Send a ping command from the container. An example is given below
```
ping -c 1 yahoo.com
```

When the command finishes executing, open the hosts file inside the _test_ directory and the updates will be reflected
