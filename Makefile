CC=gcc
CFLAGS=-O2 -Wall -Wextra -pedantic -std=c11 -Iinclude
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin

SRCS=src/main.c src/util.c src/notify.c src/help.c src/status.c src/on.c src/off.c src/toggle.c src/cancel.c src/extend.c src/db_install.c src/db_remove.c src/db_info.c
OBJS=$(SRCS:.c=.o)

all: kley

kley: $(OBJS)
	$(CC) $(CFLAGS) -o kley $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: kley
	install -Dm755 kley $(DESTDIR)$(BINDIR)/kley

clean:
	rm -f $(OBJS) kley

.PHONY: all install clean
