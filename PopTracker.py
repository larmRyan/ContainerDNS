class PopTracker:
    def __init__(self, verbose):
        self.entries = {}
        self.verbose = verbose

    def addEntry(self, ip):
        '''
        Adds a new cache entry to structure
        '''
        if ip not in self.entries:
            self.entries.update({ip: []}) # Could use dictionary and remove checks
        else:
            print("Element already exists")

    def delEntry(self, ip):
        '''
        Drops IP address from structure
        '''
        if ip in this.entries:
            this.entries.pop(ip)

    def addNewContainerToList(self, ip, containerId):
        '''
        Adds new container to list for particular ip address
        '''
        if ip in self.entries:
            containers = self.entries[ip]
            if containerId not in containers:
                containers.append(containerId)
                self.entries[ip] = containers

    def delContainerFromList(self, ip, containerId):
        '''
        Removes given container from the list
        '''
        if ip in self.entries:
            containers = self.entries[ip]
            if containerId in containers:
                containers.remove(containerId)
                self.entries[ip] = containers

    def getCount(self, ip):
        '''
        Returns number of unique containers to access particular item
        '''
        if ip in self.entries:
            return len(self.entries[ip])
        else:
            return -1

    def listContainers(self, ip):
        '''
        Returns list of unique containers to access particular item
        '''
        if ip in self.entries:
            return self.entries[ip]
        

    def checkIpExists(self, ip):
        '''
        Checks if the given ip exists in the structure
        '''
        return True if ip in self.entries else False
    
    def checkContainerInList(self, ip, containerId):
        '''
        Checks if the given container is listed for the given IP address
        '''
        if ip in self.entries:
            return True if containerId in self.entries[ip] else False
        else:
            return False


