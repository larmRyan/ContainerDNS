import pyshark
import docker

class Middleware:
    
    def __init__(self):
        self.containers = []
        self.client = docker.from_env()
        self.num_containers = 1

    def discover_containers(self):
        '''
        Store IDs of all active containers
        ID of container eventually sent to PopTracker
        '''
        docker_containers = self.client.containers.list()
        for container in docker_containers:
            self.containers.append(container.id)

    def discover_device_names(self):
        '''
        Stores names of each virtual network device for monitoring
        Used by pyshark for watching DNS requests
        '''
        network_list = client.networks.list()
        for network in network_list:
            pass

    def set_threshold(self, num_containers):
        self.num_containers = num_containers

    def get_threshold(self):
        return self.num_containers

if __name__ == "__main__":

    # Configure the middleware instance
    middleware = Middleware()
    middleware.discover_containers()
    middleware.discover_device_names()