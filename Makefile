CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I/opt/homebrew/include -g
LDFLAGS = -L/opt/homebrew/lib -lsodium

TARGET = git-credential-c

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, build/%.o, $(SRCS))

.PHONY: all clean

all: build $(TARGET)

build:
	@mkdir -p build

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)
