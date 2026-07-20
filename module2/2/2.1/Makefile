CFLAGS=-I.


all: final

final: main.o contacts.o menu.o
	gcc main.o contacts.o menu.o -o final

main.o: main.c 
	gcc $(CFLAGS) -c main.c -o main.o

contacts.o: contacts.c 
	gcc $(CFLAGS) -c contacts.c -o contacts.o	
menu.o: menu.c 
	gcc $(CFLAGS) -c menu.c -o menu.o
clean:
	rm -rf *.o
