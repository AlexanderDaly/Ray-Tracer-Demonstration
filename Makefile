# Ray Tracer Demonstration - Makefile
# Project: CPU-based ray tracer in ISO C11
# Author: Morpheus (Project Manager & Architect)
# Date: June 2025

# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O3
DEBUG_FLAGS = -g -DDEBUG -fsanitize=address,undefined
TEST_FLAGS = -fprofile-arcs -ftest-coverage
INCLUDES = -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
OBJ_DIR = build
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/raydemo

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_TARGET = $(BIN_DIR)/test_runner

# Unity test framework (local copy)
UNITY_SRC = $(TEST_DIR)/unity/unity.c
UNITY_OBJ = $(OBJ_DIR)/unity.o

# Default target
.PHONY: all clean test debug coverage help
.DEFAULT_GOAL := all

all: $(TARGET)

# Create directories
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Main executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(OBJECTS) -o $@ -lm

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

# Test executable
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(UNITY_OBJ) $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS)) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $^ -o $@ -lm

# Test object files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) -I$(TEST_DIR) -c $< -o $@

# Unity framework
$(UNITY_OBJ): $(UNITY_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Coverage report
coverage: test
	gcovr --root . --html --html-details -o coverage.html
	@echo "Coverage report generated: coverage.html"

# Format code
format:
	find $(SRC_DIR) $(INCLUDE_DIR) $(TEST_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f *.gcda *.gcno *.html

# Install dependencies (Unity test framework)
deps:
	@if [ ! -d "$(TEST_DIR)/unity" ]; then \
		echo "Downloading Unity test framework..."; \
		mkdir -p $(TEST_DIR)/unity; \
		curl -L https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.c -o $(TEST_DIR)/unity/unity.c; \
		curl -L https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity.h -o $(TEST_DIR)/unity/unity.h; \
		curl -L https://raw.githubusercontent.com/ThrowTheSwitch/Unity/master/src/unity_internals.h -o $(TEST_DIR)/unity/unity_internals.h; \
		echo "Unity test framework installed"; \
	else \
		echo "Unity test framework already installed"; \
	fi

# Run sample render
demo: $(TARGET)
	./$(TARGET) -w 400 -h 225 -o output/demo.ppm
	@echo "Demo render complete: output/demo.ppm"

# Help target
help:
	@echo "Ray Tracer Demonstration - Available targets:"
	@echo "  all      - Build release version (default)"
	@echo "  debug    - Build with debug flags and sanitizers"
	@echo "  test     - Run unit tests"
	@echo "  coverage - Generate test coverage report"
	@echo "  format   - Format code with clang-format"
	@echo "  demo     - Render sample scene"
	@echo "  deps     - Download Unity test framework"
	@echo "  clean    - Remove build artifacts"
	@echo "  help     - Show this help message" 