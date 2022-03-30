import pyshark
import docker
# import PopTracker
from PopTracker import PopTracker
# from pyshark import LiveCapture

# Name of the virtual network device created with docker's default
# network configuration. Might need to programmatically discover
# them later when we have many containers, but for now all DNS requests
# from a container should be routed through docker0
net_device_name = "docker0"

# BPF for DNS queries
# Might change later to only include DNS responses rather than all queries
bpf = "port 53"

class Middleware:

    def __init__(self):
        self.containers = []
        self.networks = []
        self.client = docker.from_env()
        self.threshold = 1
        self.tracker = PopTracker()
        # self.capture = None
        self.capture = pyshark.LiveCapture(
            interface=net_device_name,
            bpf_filter=bpf,
            only_summaries=True
        )

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
        # network_list = client.networks.list()
        # for network in network_list:
        #     print(network.name)
        self.networks.append(net_device_name)
        '''self.capture = pyshark.LiveCapture(
            interface=self.networks[0],
            bpf_filter=bpf,
            only_summaries=summary
        )'''

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

if __name__ == "__main__":

    # Configure the middleware instance
    middleware = Middleware()
    middleware.discover_containers()
    middleware.discover_device_names()
    capture = middleware.get_capture()

    pack = None

    for packet in capture.sniff_continuously():
        if("response" in packet.info):
            pack = packet
            break

    print(pack.info)