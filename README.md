# Container DNS
Container DNS is meant to keep a a dedicated DNS cache for each container for specific DNS queries it makes. DNS queries that are common across multiple containers would be kept on a cache on the host. This will enable each container to have a smaller DNS cache and will also reduce lookup time since it can query the DNS cache on the host (and get a faster response) if the response to the query was cached.

## Purpose of Each File
1. HostsManager.py
    - Updates the /etc/hosts file of each container

2. Middleware.py
    - Captures packets and communicates with the other modules to make updates to the /etc/hosts file

3. PopTracker.py
    - Keep track of containers and threshold for IPs

## Install and test

First clone the repository and install the required modules in a virtual environment:

```bash
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

Then, you need to download tshark for your distribution:

```bash
sudo apt-get install tshark
sudo pacman -Sy wireshark-cli
```
Then you will also need `docker` and `start/enable` the docker service. 

Then you can test that the program  works by running a container and pinging a URL:

```bash
docker run -it --rm archlinux bash -c "ping espn.com"
```

## TODO

1. Reimplement the modules in C
2. Popularity Tracker
    - Maintains internal structure for tracking popularity of an particular URL
3. `/etc/hosts` file manager
    - Handles updates to the `/etc/hosts` file for each container
4. Intercepting network traffic
    - Need to figure out how to track a single DNS container's network traffic
    - Probably need to use OS package or something else to query the system for the list of network devices because I don't think docker api has that information
5. Get container to run the HostsManager as a daemon
6. Get Middleware to communicate with the HostsManager and PopTracker