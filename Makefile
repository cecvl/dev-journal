CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lsqlite3
TARGET = devjournal
SRC = devjournal.c
DB = journal.db
SCHEMA = schema.sql

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(DB): $(SCHEMA)
	sqlite3 $(DB) < $(SCHEMA)
	@echo "Database created successfully"

init: $(DB)

clean:
	rm -f $(TARGET)

cleandb:
	rm -f $(DB)

cleanall: clean cleandb

run: $(TARGET) $(DB)
	./$(TARGET)

.PHONY: all init clean cleandb cleanall run
