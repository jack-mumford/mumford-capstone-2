CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra $(shell pkg-config --cflags raylib)
LDFLAGS  := $(shell pkg-config --libs raylib) -framework IOKit -framework Cocoa -framework OpenGL

SRC_DIR  := src
BUILD_DIR := build
TARGET   := $(BUILD_DIR)/4d-pixel-game

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
