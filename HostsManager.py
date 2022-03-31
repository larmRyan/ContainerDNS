from os.path import exists
import utils

class HostsManager:

    def __init__(self, path):

        # Try to create the file if it doesn't already
        # exist
        self.path = path
        with open(self.path, "r") as file:
            self.entries = utils.list_to_dict(file.readlines())

    def add_host(self, ip, url):
        '''
        Adds an entry to the dictionary
        and updates the file
        '''
        if ip not in self.entries:
            self.entries.update({ip: url})
            self.update_host_file()

    def get_url_from_ip(self, ip):
        if ip in self.entries:
            return self.entries.get(ip)
        return None

    def remove_host(self, ip):
        '''
        Removes an entry from the dictionary
        and updates the file
        '''
        if ip in self.entries:
            self.entries.pop(ip)
            self.update_host_file()

    def update_host_file(self):
        '''
        Updates contents of container's host file
        given current state of the 
        '''
        with open(self.path, "a") as host_file:
            for item in self.entries.items():
                (ip, url) = item
                host_file.write(ip + "\t" + url + "\n")