def list_to_dict(entries_list):
    '''
    Converts list of tab-delimited entires from
    /etc/hosts and converts it to a dictionary
    '''
    entries_dict = {}
    for entry in entries_list:
        ip, url = entry.split("\t", 1)
        url = url.strip()
        entries_dict.update({ip: url})
    return entries_dict


def parse_response(respone):
    url = response.split(' ')[5]
    ip = response.split(' ')[7]
    return (ip, url)