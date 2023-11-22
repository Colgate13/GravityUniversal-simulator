SRC_DIR = src
INCLUDE_DIR = includes
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

CC = clang
CFLAGS = -Wall -Wextra `pkg-config --cflags raylib`
LIBS = `pkg-config --libs raylib` -lm -ldl -lpthread
TARGET = $(BIN_DIR)/newTon
SRC = main.c 

SOURCES = $(SRC_DIR)/$(SRC) # sources
HEADERS = $(INCLUDE_DIR)/Main.h # headers

all: $(TARGET)

$(TARGET): $(SOURCES) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -f $(TARGET)

.PHONY: all clean
