class PopTracker:
    def __init__(self, verbose):
        self.entries = {}
        self.verbose = verbose

    def add_entry(self, ip, container_id):
        '''
        Adds a new cache entry to structure
        '''
        if ip not in self.entries:
            self.entries.update({ip: [container_id]})

    def del_entry(self, ip):
        '''
        Drops IP address from structure
        '''
        if ip in this.entries:
            this.entries.pop(ip)

    def add_new_container_to_list(self, ip, container_id):
        '''
        Adds new container to list for particular ip address
        '''
        try:
            containers = self.entries.get(ip)
        except KeyError:
            raise KeyError("Could not find IP in list")
        else:
            if container_id not in containers:
                containers.append(container_id)
                sefl.entries.update({ip: containers})

    def del_container_from_list(self, ip, container_id):
        '''
        Removes given container from the list
        '''
        try:
            containers = self.entries.get(ip)
        except KeyError:
            raise KeyError("Could not find ip in list")
        else:
            if container_id in containers:
                containers.remove(container_id)
                self.entries.update({ip: containers})

    def get_count(self, ip):
        '''
        Returns number of unique containers to access
        particular item or -1 if the item doesn't exist
        '''
        if ip in self.entries:
            return len(self.entries.get(ip))
        return -1

    def get_containers(self, ip):
        '''
        Returns list of unique containers to access particular item
        '''
        if ip in self.entries:
            return self.entries.get(ip)
        else:
            return None

    def check_ip_exists(self, ip):
        '''
        Checks if the given ip exists in the structure
        '''
        return True if ip in self.entries else False
    
    def check_container_in_list(self, ip, container_id):
        '''
        Checks if the given container is listed for the given IP address
        '''
        if ip in self.entries:
            return True if container_id in self.entries.get(ip) else False
        return False