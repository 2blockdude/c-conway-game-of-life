CC = gcc
CFLAGS = -g
LIBS = 
C_SOURCES :=$(wildcard *.c)
C_EXECUTABLE :=$(C_SOURCES:.c=)

all:$(C_EXECUTABLE)

$(C_EXECUTABLES): $(C_SOURCES)
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f malloc-game-of-life
	rm -f onemap-game-of-life
	rm -f twomap-game-of-life
