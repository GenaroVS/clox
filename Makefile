# === Makefile ===

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c17 -I.

# Folders
SRC_DIR := src
BUILD_DIR := build
TARGET := clox

# Collect all .c files and map them to build/*.o
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule for building .o files inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build folder if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
.PHONY: clean
clean:
	rm -r $(BUILD_DIR) $(TARGET)

