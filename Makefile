CC=gcc
TARGET=Middleware

all:
	$(CC) Middleware.c -o $(TARGET)

clean:
	rm $(TARGET)