BUILD_DIR ?= build
CONFIG ?= Release

.PHONY: all configure build test cross-test clean

all: build test

configure:
	@cmake -S . -B $(BUILD_DIR) -DBEDROCK_PROTOCOL_BUILD_POOL=ON -DBEDROCK_PROTOCOL_BUILD_TESTS=ON > nul

build: configure
	@cmake --build $(BUILD_DIR) --config $(CONFIG) -j 8 -- /nologo /verbosity:minimal

test: build
	@ctest --test-dir $(BUILD_DIR) -C $(CONFIG) --output-on-failure
	@$(MAKE) --no-print-directory cross-test

cross-test:
	@python tests/cross_verify/report.py $(BUILD_DIR)/$(CONFIG)/bedrock-protocol-cross-verify.exe

clean:
	cmake -E remove_directory $(BUILD_DIR)
