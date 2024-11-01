CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -pie -fpie
SERVICES = -L services.o

.SILENT: all services lcdctl status
.DEFAULT: all

all: status

services:
	$(CC) $(CFLAGS) $(@).h -o $(@).o
	echo Updated $(@).o

lcdctl: services
	$(CC) $(CFLAGS) $(@).c $(SERVICES) -o $(@)
	echo Updated $(@)

status: lcdctl services
	$(CC) $(CFLAGS) $(@).c $(SERVICES) -o $(@)
	echo Updated $(@)
