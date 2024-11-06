server:
		g++ ./src/server.cpp -I./src/includes -I./src/events -o server

client:
		g++ ./src/client.cpp -I./src/includes -I./src/events -o client
all:
		g++ ./src/server.cpp -I./src/includes -I./src/events -o server
		g++ ./src/client.cpp -I./src/includes -I./src/events -o client

clean:
		rm client server
