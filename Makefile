CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lreadline
TARGET = shell
SRC = shell.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
