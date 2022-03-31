def list_to_dict(entries_list):
    '''
    Converts list of tab-delimited entires from
    /etc/hosts and converts it to a dictionary
    '''
    entries_dict = {}
    for entry in entries_list:

        if entry.startswith("#") or entry.startswith("\n"):
            pass
        else:
            try:
                ip = entry.split("\t")[0]
                url = entry.split("\t")[1]
                url = url.strip()
                entries_dict.update({ip: url})
            except:
                pass
    return entries_dict
