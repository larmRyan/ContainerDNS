import os, sys

path = "/tmp/test"

# Mode for the named pipe
mode = 0o666

os.mkfifo(path, mode)

while True:

    test_string = input("Enter some text to read from FIFO: ")
    with open(path, "w") as fifo:
        fifo.write(test_string)
        fifo.flush()