CC = gcc
CFLAGS = -Wall -std=c99 -g

fwsim: fwsim.o command.o policy.o packet.o

fwsim.o: fwsim.c command.h policy.h packet.h

command.o: command.c command.h

policy.o: policy.c policy.h

packet.o: packet.c packet.h policy.h command.h

clean:
	rm -f fwsim.o command.o policy.o packet.o
	rm -f fwsim
	rm -f output.txt
