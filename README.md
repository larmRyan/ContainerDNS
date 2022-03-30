# Container DNS

## TODO

1. Popularity Tracker
    - Maintains internal structure for tracking popularity of an particular URL
2. `/etc/hosts` file manager
    - Handles updates to the `/etc/hosts` file for each container
3. Intercepting network traffic
    - Need to figure out how to track a single DNS container's network traffic
    - Probably need to use OS package or something else to query the system for the list of network devices because I don't think docker api has that information
4. Get container to run the HostsManager as a daemon
5. Get Middleware to communicate with the HostsManager and PopTracker

---

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