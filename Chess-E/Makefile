# Chess-E Makefile

# Build directory
BUILD_DIR := build

# Default target
all: build

# Create build directory and build the project
build:
	@echo "Building Chess-E..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && cmake --build .
	@echo "Build complete."

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

# Run the main chess engine
run:
	@echo "Running Chess-E engine..."
	@cd $(BUILD_DIR) && ./chess_engine

# Run the test suite
test:
	@echo "Running basic test suite..."
	@cd $(BUILD_DIR) && ./test_engine

# Run evaluation tests
test-eval:
	@echo "Running evaluation tests..."
	@cd $(BUILD_DIR) && ./test_evaluation

# Run search tests
test-search:
	@echo "Running search algorithm tests..."
	@cd $(BUILD_DIR) && ./test_search

# Run all tests
test-all: test test-eval test-search

# Rebuild from scratch
rebuild: clean build

# Help menu
help:
	@echo "Chess-E Makefile commands:"
	@echo "  make           - Build the chess engine"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make run       - Run the chess engine"
	@echo "  make test      - Run the basic test suite"
	@echo "  make test-eval - Run evaluation tests"
	@echo "  make test-search - Run search algorithm tests"
	@echo "  make test-all  - Run all tests"
	@echo "  make rebuild   - Rebuild from scratch"
	@echo "  make help      - Show this help message"

.PHONY: all build clean run test test-eval test-search test-all rebuild help 