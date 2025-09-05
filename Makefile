# Makefile for AutoCommitBot

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = 

# Source files
SRC = src/autocommitbot.c
TARGET = autocommitbot

# Installation paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
SYSTEMD_USER_DIR = $(HOME)/.config/systemd/user

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

install: all
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/
	install -d $(DESTDIR)$(SYSTEMD_USER_DIR)
	install -m 644 conf/autocommit.service $(DESTDIR)$(SYSTEMD_USER_DIR)/
	systemctl --user daemon-reload
	systemctl --user enable autocommit.service
	@echo "Installation complete!"

uninstall:
	@echo "Stopping and disabling service..."
	systemctl --user disable --now autocommit.service || true
	@echo "Removing installed files..."
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(SYSTEMD_USER_DIR)/autocommit.service
	systemctl --user daemon-reload
	@echo "Uninstallation complete!"
