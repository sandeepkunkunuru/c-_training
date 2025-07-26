# Makefile for Data Processing Pipeline
# Demonstrates build system integration

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
TARGET = data_processor
SOURCES = main.cpp data_processor.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = data_processor.hpp

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) *.csv

# Debug build
debug: CXXFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build with optimizations
release: CXXFLAGS += -DNDEBUG -O3 -march=native
release: clean $(TARGET)

# Check for memory leaks (requires valgrind)
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Static analysis (requires cppcheck)
static-analysis:
	cppcheck --enable=all --std=c++17 $(SOURCES) $(HEADERS)

# Format code (requires clang-format)
format:
	clang-format -i $(SOURCES) $(HEADERS)

# Generate documentation (requires doxygen)
docs:
	doxygen Doxyfile

# Install dependencies (example for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install build-essential valgrind cppcheck clang-format doxygen

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the program (default)"
	@echo "  run          - Build and run the program"
	@echo "  clean        - Remove build artifacts"
	@echo "  debug        - Build with debug information"
	@echo "  release      - Build optimized release version"
	@echo "  memcheck     - Run with valgrind memory checker"
	@echo "  static-analysis - Run static code analysis"
	@echo "  format       - Format source code"
	@echo "  docs         - Generate documentation"
	@echo "  install-deps - Install development dependencies"
	@echo "  help         - Show this help message"

.PHONY: all run clean debug release memcheck static-analysis format docs install-deps help
