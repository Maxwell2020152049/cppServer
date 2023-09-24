all: server client

server: server.o err.o
	g++ server.o err.o -o server

client: client.o err.o
	g++ client.o err.o -o client

server.o: server.cpp
	g++ -c server.cpp -o server.o 

client.o: client.cpp
	g++ -c client.cpp -o client.o 

err.o: err.cpp
	g++ -c err.cpp -o err.o

clean:
	rm client server *.o