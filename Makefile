# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(INCLUDE_DIR)

# Folders
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET = $(BUILD_DIR)/clue-pro

# Default
all: $(BUILD_DIR) $(TARGET)

# Δημιουργία του εκτελέσιμου
$(TARGET): $(OBJECTS) $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Μεταγλώττιση πηγαίων cpp σε αντικείμενα
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# main.o ειδικά
$(BUILD_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Αν δεν υπάρχει φάκελος build, να τον δημιουργεί
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Καθαρισμός
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) *.o

