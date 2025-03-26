# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Folders
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET = $(BUILD_DIR)/clue-pro

# Default
all: $(TARGET)

$(TARGET): $(OBJECTS) main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(BUILD_DIR)/*.o *.o $(TARGET)
