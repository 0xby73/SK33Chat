# Installation 

```sh
sh setup.sh
make all
```

Hosting a server is ```./server``` <br>
Running the client is ```./client IP PORT``` <br> <br>

server.cpp holds the main server file <br>
client.cpp holds the main client file <br> <br>

# What is SK33Chat
FOSS Chat application to circumvent the heavy telemetry from companies to provide a cleaner and safer resource of communication

# Known Issues 
The kick command doesn't close the client, the client has to close manually by typing a message on it so the server bugs ( still usable ) until its removed. Also another reason why some of the error handling is commented out. <br>
UUID Doesn't assign to anything so it's useless at the moment <br>

# TODO List
Add encryption
Add a TUI/GUI
Split up server.cpp into it's necessary header files to avoid chunky and unclean code
