CC=gcc
CFLAGS=-lWarn -pedantic

temperature: temperature.o libmyifttt.a
	$(CC) temperature.o -L. -lmyifttt -lcurl -lwiringPi -o temperature


blink: blink.o 
	cc blink.o -lwiringPi -o blink

button: button.o 
	cc button.o -lwiringPi -o button

libmyifttt.a: ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

temperature.o:	temperature.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

all: temperature blink button

clean:
	rm  temperature *.o
