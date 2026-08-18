CC=gcc
CFLAGS=-O2 -Wall -Wextra -pedantic -std=c11
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin

all: kley

kley: src/kley.c
	$(CC) $(CFLAGS) -o kley src/kley.c

install: kley
	install -Dm755 kley $(DESTDIR)$(BINDIR)/kley

clean:
	rm -f kley

.PHONY: all install clean
