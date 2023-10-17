CC = gcc
CFLAGS = -Wall

SRC = main.c
OUTPUT = br

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

clean:
	rm -f $(OUTPUT)

.PHONY: all clean
