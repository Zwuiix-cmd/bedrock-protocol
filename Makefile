BUILD_DIR ?= build
CONFIG ?= Release

.PHONY: all configure build test clean

all: build test

configure:
	cmake -S . -B $(BUILD_DIR) -DBEDROCK_PROTOCOL_BUILD_POOL=ON -DBEDROCK_PROTOCOL_BUILD_TESTS=ON

build: configure
	cmake --build $(BUILD_DIR) --config $(CONFIG) -j 8

test: build
	ctest --test-dir $(BUILD_DIR) -C $(CONFIG) --output-on-failure

clean:
	cmake -E remove_directory $(BUILD_DIR)
