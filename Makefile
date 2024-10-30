server:
	g++ ./server.cpp -I./src/includes -I./src/events -o server
	
client:
	g++ ./client.cpp -I./src/includes -I./src/events -o client
all:
	g++ ./server.cpp -I./src/includes -I./src/events -o server
	g++ ./client.cpp -I./src/includes -I./src/events -o client

clean:
	rm client server
