CC=gcc
CFLAGS=-O2 -Wall -Wextra -pedantic -std=c11 -Iinclude
PREFIX=$(HOME)/.local
BINDIR=$(PREFIX)/bin
TARGET=kley

SRCS=src/main.c src/util.c src/notify.c src/help.c src/status.c src/on.c src/off.c src/toggle.c src/cancel.c src/extend.c src/db_install.c src/db_remove.c src/db_info.c
OBJS=$(SRCS:.c=.o)

MAKEFLAGS += -j$(shell nproc)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	@mkdir -p $(DESTDIR)$(BINDIR)
	install -Dm755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "✅ Installed to $(DESTDIR)$(BINDIR)/$(TARGET)"
	@if ! echo "$(PATH)" | tr ':' '\n' | grep -qx "$(HOME)/.local/bin"; then \
		echo "⚠  $(HOME)/.local/bin not in PATH — add: export PATH=\"\$$HOME/.local/bin:\$$PATH\""; \
	fi

uninstall:
	@rm -vf $(DESTDIR)$(BINDIR)/$(TARGET)
	@echo "🗑️  Uninstalled from $(DESTDIR)$(BINDIR)/$(TARGET)"
	@if [ -f /etc/hosts.monster ]; then echo "⚠  Note: /etc/hosts.monster still exists — run 'kley off' to restore if needed"; fi

debug:
	$(CC) $(CFLAGS) -g -O0 -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all install uninstall clean debug
