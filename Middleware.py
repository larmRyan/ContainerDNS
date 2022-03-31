import pyshark
import docker
from PopTracker import PopTracker
from HostsManager import HostsManager
import utils

# Name of the virtual network device created with docker's default
# network configuration. Might need to programmatically discover
# them later when we have many containers, but for now all DNS requests
# from a container should be routed through docker0
net_device_name = "docker0"

# BPF for DNS queries
# Might change later to only include DNS responses rather than all queries
bpf = "port 53"

# The name of the field where the response address is
address_field = "a"

# Field name containing URL
url_field = "resp_name"

# Path to the file hosts file
path = "/etc/hosts"

class Middleware:

    def __init__(self):
        self.containers = []
        self.networks = []
        self.client = docker.from_env()
        self.threshold = 1
        self.tracker = PopTracker()
        self.capture = pyshark.LiveCapture(
            interface=net_device_name,
            bpf_filter=bpf,
            only_summaries=False
        )
        self.manager = HostsManager(path)

    def discover_containers(self):
        '''
        Store IDs of all active containers
        ID of container eventually sent to PopTracker
        '''
        docker_containers = self.client.containers.list()
        for container in docker_containers:
            self.containers.append(container.id)

    def discover_device_names(self, summary=True):
        '''
        Stores names of each virtual network device for monitoring
        Used by pyshark for watching DNS requests (e.g. eth0)

        :param summary: Only display a summary of the DNS packet
        '''
        self.networks.append(net_device_name)

    def set_threshold(self, num_containers):
        self.threshold = num_containers

    def get_threshold(self):
        return self.threshold

    def add_entry_to_tracker(self, ip, container_id):
        self.tracker.add_entry(ip, container_id)

    def del_entry_from_tracker(self, ip, container_id):
        self.tracker.del_entry(ip, container_id)

    def get_capture(self):
        return self.capture

    def get_tracker(self):
        return self.tracker
    
    def get_manager(self):
        return self.manager

    def get_containers(self, index):
        return self.containers[index]

if __name__ == "__main__":

    # Configure the middleware instance
    middleware = Middleware()
    middleware.discover_containers()
    middleware.discover_device_names()
    capture = middleware.get_capture()
    tracker = middleware.get_tracker()
    manager = middleware.get_manager()

    ip = None
    url = None

    # Get the url and ip from DNS response packet
    for packet in capture.sniff_continuously():
        if address_field in packet.dns.field_names:
            ip = packet.dns.get(address_field)
            url = packet.dns.get(url_field)
            manager.add_host(ip, url)
            tracker.add_entry(ip, 0)