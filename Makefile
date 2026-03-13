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

# Unit tests (Catch2)
TEST_CATCH2    := tests/unit/catch2/catch_amalgamated.cpp
TEST_SRCS      := $(wildcard tests/unit/test_*.cpp)
TEST_GAME_OBJS := build/combat.o build/item.o build/level.o
TEST_RUNNER    := build/test-runner

.PHONY: all web clean test-unit test-e2e

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

test-unit: all
	$(CXX) -std=c++17 $(shell pkg-config --cflags raylib) -Iinclude -Itests/unit/catch2 \
		$(TEST_CATCH2) $(TEST_SRCS) \
		$(TEST_GAME_OBJS) \
		-o $(TEST_RUNNER) $(LDFLAGS)
	./$(TEST_RUNNER)

test-e2e:
	@command -v node >/dev/null 2>&1 || { echo "Error: Node.js is required for E2E tests. Install from https://nodejs.org"; exit 1; }
	@command -v npm  >/dev/null 2>&1 || { echo "Error: Node.js is required for E2E tests. Install from https://nodejs.org"; exit 1; }
	cd tests/e2e && npm install && ./node_modules/.bin/playwright install chromium
	python3 -m http.server 8080 --directory build/web & \
	  SERVER_PID=$$!; sleep 2; \
	  cd tests/e2e && ./node_modules/.bin/playwright test; TEST_EXIT=$$?; \
	  kill $$SERVER_PID 2>/dev/null; exit $$TEST_EXIT

clean:
	rm -rf $(BUILD_DIR)
