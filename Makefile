# Get OS type
UNAME_S=$(shell uname -s)

# Configure compiler
CC=clang
CFLAGS=-std=gnu11 -g -O3 -Wall -Wextra -Wpedantic -Isrc
LDFLAGS=-lm

CMAKE_GEN=

# Linux libraries
ifeq ($(UNAME_S), Linux)
	LDFLAGS+=-ldl -lpthread
# MacOS frameworks
else ifeq ($(UNAME_S), Darwin)
	LDFLAGS+=-framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
# Windows libs
else
	LDFLAGS+=-lglu32 -lopengl32 -luser32 -lgdi32 -lkernel32
	CMAKE_GEN=-G "MinGW Makefiles"
endif

# Configure libraries
CFLAGS+=-Ilib/glfw/include -Ilib/glad/include -Ilib/cglm/include -Ilib/log/src -Ilib/stb -Ilib/cvec/src
LDFLAGS+=lib/glfw/build/src/libglfw3.a lib/glad/src/glad.o lib/cglm/build/libcglm.a lib/log/src/log.o lib/cvec/src/cvec.o

SRC=$(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ=$(SRC:.c=.o)
BIN=bin/cmine

.PHONY: all libs dirs clean clean-all

all: libs dirs $(BIN)

libs:
	cd lib/glfw && cmake -S . -B ./build $(CMAKE_GEN) && cmake --build ./build
	cd lib/cglm && cmake -S . -B ./build $(CMAKE_GEN) -DCGLM_STATIC=ON && cmake --build ./build
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/log && $(CC) -o src/log.o -Isrc -c src/log.c -DLOG_USE_COLOR
	cd lib/cvec && $(CC) -o src/cvec.o -Isrc -c src/cvec.c

dirs:
	mkdir -p bin
	mkdir -p log

run: all
	./$(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) -r bin log
	$(RM) $(OBJ)

clean-all: clean
	$(RM) -r lib/glfw/build
	$(RM) -r lib/cglm/build
	$(RM) lib/log/src/log.o
	$(RM) lib/glad/src/glad.o
