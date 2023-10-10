INCS := -I ..

all: server client

server: server.o err.o Socket.o
	g++ server.o err.o Socket.o -o server

client: client.o err.o Socket.o
	g++ client.o err.o Socket.o -o client

%.o: %.cpp
	g++ -c $(INCS) $< -o $@

clean:
	rm client server *.o

check:
	cpplint --verbose=0 *.cpp *.h