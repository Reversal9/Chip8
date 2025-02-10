# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = `sdl2-config --cflags --libs`

# Directories
SRC_DIR = src
BUILD_DIR = build

# File extensions
SRC_EXT = .c
OBJ_EXT = .o

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*$(SRC_EXT))
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%$(SRC_EXT)=$(BUILD_DIR)/%$(OBJ_EXT))

# Target executable
TARGET = build/main

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(OBJ_FILES) $(LDFLAGS)

# Compile .c files to .o files
$(BUILD_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean up build files
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild

