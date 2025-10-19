
CC        = gcc
CFLAGS    = -Wall -Wextra -std=c99 -g
LDFLAGS   = -lcurses

# Directories
SRC_DIR    = src
INCLUDE_DIR= include
BUILD_DIR  = build
BIN_DIR    = bin

$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS    = $(OBJECTS:.o=.d)

TARGET = $(BIN_DIR)/file_manager

INC_FLAGS = -I$(INCLUDE_DIR)
CPPFLAGS  = $(INC_FLAGS) -MMD -MP

.PHONY: all clean install uninstall run debug release test-files clean-test structure help

all: $(TARGET)
	@echo "Build complete! Run with: ./$(TARGET)"

$(TARGET): $(OBJECTS)
	@echo "Linking executable..."
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Install to /usr/local/bin
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/file_manager
	@echo "Installation complete! Run with: file_manager"

# Uninstall from /usr/local/bin
uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/file_manager
	@echo "Uninstall complete!"

# Build and run
run: $(TARGET)
	@echo "Starting File Manager..."
	./$(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)
	@echo "Debug build complete!"

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)
	@echo "Release build complete!"

# Create sample test files
test-files:
	@echo "Creating test files..."
	mkdir -p test_directory
	echo "This is a test file." > test.txt
	echo "Hello World!" > hello.txt
	echo "Sample content for testing." > sample.txt
	@echo "Test files created!"

# Remove test files
clean-test:
	@echo "Removing test files..."
	rm -f test.txt hello.txt sample.txt
	rm -rf test_directory
	@echo "Test files removed!"

# Show project structure
structure:
	@echo "Project Structure:"
	@find . -type f \( -name "*.c" -o -name "*.h" -o -name "Makefile" \) | sort

# Help
help:
	@echo "Available targets:"
	@echo "  all         - Build the project (default)"
	@echo "  clean       - Remove build files"
	@echo "  run         - Build and run the program"
	@echo "  debug       - Build with debug flags"
	@echo "  release     - Build optimized release version"
	@echo "  install     - Install to /usr/local/bin"
	@echo "  uninstall   - Uninstall from /usr/local/bin"
	@echo "  test-files  - Create sample files"
	@echo "  clean-test  - Remove sample files"
	@echo "  structure   - Show project file structure"
	@echo "  help        - Show this help message"

-include $(DEPS)
