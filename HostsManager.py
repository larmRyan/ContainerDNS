from os.path import exists
import utils
class HostsManager:

    def __init__(self, path):
        try:
            with open(path, "x") as file:
                pass
        except:
            print("File already exists")
        self.path = path
        with open(self.path, "r") as file:
            self.entries = utils.list_to_dict(file.readlines())

    def add_host(self, ip, url):
        if ip not in self.entries:
            self.entries.update({ip: url})
            self.update_host_file()

    def get_url_from_ip(self, ip):
        if ip in self.entries:
            return self.entries.get(ip)
        return None

    def remove_host(self, ip):
        if ip in self.entries:
            self.entripes.pop(ip)
            self.update_host_file()

    def update_host_file(self):
        with open(self.path, "w") as host_file:
            for item in self.entries.items():
                (ip, url) = item
                hosts_file.write(ip + "\t" + url + "\n")