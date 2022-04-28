CC=gcc
MIDDLEWARE=Middleware
SNIFFER=sniffer

all:
	$(CC) Middleware.c PopTracker.c PopTracker.h host_updater.c host_updater.h -o $(MIDDLEWARE)
	$(CC) sniffer.c -o $(SNIFFER)

clean:
	rm Middleware sniffer