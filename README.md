# Container DNS

---

## TODO

1. Popularity Tracker
    - Maintains internal structure for tracking popularity of an particular URL
2. `/etc/hosts` file manager
    - Handles updates to the `/etc/hosts` file for each container
3. Intercepting network traffic
    - Need to figure out how to track a single DNS container's network traffic
4. Get container to run the HostsManager as a daemon
5. Get Middleware to communicate with the HostsManager and PopTracker
