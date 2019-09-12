CC = gcc
CFLAGS = -Wall -std=c99

cnp: cnp.o document.o buffer.o

cnp.o: cnp.c buffer.h document.h

buffer.o: buffer.c buffer.h document.h

document.o: document.c document.h

clean:
	rm -f cnp.o buffer.o document.o
	rm -f cnp
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt