import pyshark
import docker
import PopTracker

class Middleware:

    def __init__(self):
        self.containers = []
        self.client = docker.from_env()
        self.threshold = 1
        self.tracker = PopTracker()

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
        Used by pyshark for watching DNS requests (e.g. eth0)
        '''
        pass
        # network_list = client.networks.list()
        # for network in network_list:
        #     print(network.name)

    def set_threshold(self, num_containers):
        self.threshold = num_containers

    def get_threshold(self):
        return self.threshold

    def add_entry_to_tracker(self, ip, container_id):
        self.tracker.add_entry(ip, container_id)

    def del_entry_from_tracker(self, ip, container_id):
        self.tracker.del_entry(ip, contaienr_id)

if __name__ == "__main__":

    # Configure the middleware instance
    middleware = Middleware()
    middleware.discover_containers()
    middleware.discover_device_names()


    '''
    # TODO / ALGO

    - Create middleware instance
        - The instance should then discover any containers and network devices for tracking
    - Register the containers for tracking

    When an entry is detected by pyshark:

    procedure entry_found
        Send entry with ip / container id to the tracker
        tracker should test that:
            1. Is the ip being tracked
            2. Is the container already in the list for that ip
            3. If it is then skip it
            4. else add the entry to the list
        Check popularity for given item
            if the popularity = threshold
                remove the entry from all containers hosts file
            else
                write the entry to the hosts file for that container


    # General

    - Threshold should be configuraable in the manager
    - Manager should periodically query for new containers (say every 5 minutes) (do later)
    - Manager should then check for new network devices (again every 5 min but do later)
        - This period should be configurable
    - Will need to parse arguments for this configuration options
    '''