CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -pie -fpie
SERVICES = -L services.o

.DEFAULT: all

all: status

services:
	$(CC) $(CFLAGS) $(@).h -o $(@).o

lcdctl: services
	$(CC) $(CFLAGS) $(@).c $(SERVICES) -o $(@)

status: lcdctl services
	$(CC) $(CFLAGS) $(@).c $(SERVICES) -o $(@)

clean: services lcdctl status
	rm services lcdctl status
