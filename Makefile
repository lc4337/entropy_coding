# Compiler
CXX = g++

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

# Files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)%.o,$(TEST_SRCS))

# Flags
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -I$(INC_DIR)

# Targets
all: compress

compress: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_OBJS) $(filter-out $(BUILD_DIR)/main.o,$(OBJS))
	$(CXX) $(CXXFLAGS) $^ -o $@
	./test

.PHONY: clean

clean:
	rm -f $(BUILD_DIR)/* test