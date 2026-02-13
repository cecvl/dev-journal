CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lsqlcipher
TARGET = devjournal
SRC = devjournal.c
DB = journal.db
SCHEMA = schema.sql

# Installation paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(DB): $(SCHEMA)
	sqlite3 $(DB) < $(SCHEMA)
	@echo "Database created successfully"

init: $(DB)

install: $(TARGET)
	@echo "Installing $(TARGET) to $(BINDIR)..."
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/$(TARGET)
	@echo "Installation complete!"
	@echo "Run 'devjournal' from anywhere to use the application"
	@echo "Database will be stored in ~/.local/share/devjournal/"

uninstall:
	@echo "Removing $(TARGET) from $(BINDIR)..."
	rm -f $(BINDIR)/$(TARGET)
	@echo "Uninstallation complete!"
	@echo "To remove data, run: rm -rf ~/.local/share/devjournal/"

clean:
	rm -f $(TARGET)

cleandb:
	rm -f $(DB)

cleanall: clean cleandb

run: $(TARGET)
	./$(TARGET)

.PHONY: all init install uninstall clean cleandb cleanall run
