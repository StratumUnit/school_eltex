CFLAGS=-I.


all: final

final: main.o contacts.o 
	gcc main.o contacts.o -o final

main.o: main.c 
	gcc $(CFLAGS) -c main.c -o main.o

contacts.o: contacts.c 
	gcc $(CFLAGS) -c contacts.c -o contacts.o

clean:
	rm -rf *.o
