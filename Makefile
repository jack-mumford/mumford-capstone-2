CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra $(shell pkg-config --cflags raylib)
LDFLAGS  := $(shell pkg-config --libs raylib) -framework IOKit -framework Cocoa -framework OpenGL

SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/4d-pixel-game

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Web build
EMCC         := emcc
RAYLIB_WEB   := raylib-web
WEB_DIR      := build/web
WEB_TARGET   := $(WEB_DIR)/index.html
WEB_FLAGS    := -std=c++17 -DPLATFORM_WEB \
                -I include -I $(RAYLIB_WEB) \
                -s USE_GLFW=3 \
                -s ASYNCIFY \
                -s TOTAL_MEMORY=67108864 \
                -s FORCE_FILESYSTEM=1 \
                -O2

.PHONY: all web clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

web: $(WEB_DIR)
	$(EMCC) $(SRCS) -o $(WEB_TARGET) \
		$(WEB_FLAGS) \
		$(RAYLIB_WEB)/libraylib.a \
		--shell-file $(RAYLIB_WEB)/minshell.html

$(WEB_DIR):
	mkdir -p $(WEB_DIR)

clean:
	rm -rf $(BUILD_DIR)
