# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -Iinclude -I/usr/include


# Define the target executable
TARGET = CuBirdsGame

# Define the directories for source files and object files
SRC_DIR = src
OBJ_DIR = obj

# Define the source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Rule to compile the project
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Ensure object directory exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the compiled files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)