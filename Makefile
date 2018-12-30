CPP=gcc
#CFLAGS=

LDFLAGS= -lpthread

all: client server

client:
	gcc -o client client.c $(CFLAGS) $(LDFLAGS)

server:
	gcc -o server server.c $(CFLAGS) $(LDFLAGS)

clean:
	rm client server