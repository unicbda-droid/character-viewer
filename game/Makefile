CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lSDL2 -lGLEW -lGL -lm

SRC = src/main.c src/shader.c src/renderer.c src/world.c src/player.c src/camera.c
OBJ = $(SRC:.c=.o)
BIN = gta6

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN)

.PHONY: all clean run
