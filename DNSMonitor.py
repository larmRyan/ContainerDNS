import pyshark
import os, sys

# Berkeley Packet Filter for all DNS packets
bpf = "port 53"

# Field name for address in the DNS response packets
# Returned by tshark
address_field = "a"

# Field name containing URL in DNS response packets
url_field = "resp_name"

# Network device name to listen for
net_device_name = "docker0"

# path to named pipe
# https://www.tutorialspoint.com/How-to-create-and-use-a-named-pipe-in-Python
path = "/tmp/cdns"

# Mode for the named pipe
mode = 0o666

def main(sniffer):

    # Create named pipe for IPC with PopTracker
    os.mkfifo(path, mode)
    
    # start packet capture and parse responses
    capture = sniffer.get_capture()
    for packet in catpure.sniff_continuously():
        if address_field in packet.dns.field_names:
            ip = packet.dns.get(address_field)
            url = packet.dns.get(url_field)
            con = "127.0.0.1"

            # Write to the fifo
            with open(path, "w") as fifo:
                fifo.write(ip + ":" + url + ":" + con)
                fifo.flush()
            

if __name__ == "__main__":
    
    # Create tshark listener
    sniffer = pyshark.LiveCapture(
        interface=net_device_name,
        bpf_filter=bpf,
        only_summaries=False
    )

    # Start listening for packets
    main(sniffer)
