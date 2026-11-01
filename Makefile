CXX      ?= c++
STD      ?= -std=c++17
WARN     := -Wall -Wextra -Wpedantic -Werror
CXXFLAGS ?= -O1 -g
BUILD    ?= build
SAN      := -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer

SRCS := $(wildcard tests/*.cpp)
OBJS := $(patsubst tests/%.cpp,$(BUILD)/%.o,$(SRCS))
DEPS := $(wildcard include/dsa/*.hpp) third_party/doctest.h

.PHONY: all test sanitize coverage clean headers

all: $(BUILD)/run_tests

$(BUILD)/%.o: tests/%.cpp $(DEPS) | $(BUILD)
	$(CXX) $(STD) $(WARN) $(CXXFLAGS) $(EXTRA) -Iinclude -Ithird_party -c $< -o $@

$(BUILD)/run_tests: $(OBJS)
	$(CXX) $(STD) $(CXXFLAGS) $(EXTRA) $^ -o $@

$(BUILD):
	mkdir -p $(BUILD)

test: $(BUILD)/run_tests
	./$(BUILD)/run_tests

# Rebuilds into build-san/ with AddressSanitizer and UndefinedBehaviorSanitizer and runs the tests.
sanitize:
	$(MAKE) BUILD=build-san EXTRA="$(SAN)" test

# Line coverage with clang (llvm-cov). Needs llvm-profdata and llvm-cov on PATH or via xcrun.
coverage:
	$(MAKE) BUILD=build-cov CXX=clang++ EXTRA="-fprofile-instr-generate -fcoverage-mapping" CXXFLAGS="-O0 -g" all
	LLVM_PROFILE_FILE=build-cov/run.profraw ./build-cov/run_tests
	$(LLVM_PROFDATA) merge -sparse build-cov/run.profraw -o build-cov/run.profdata
	$(LLVM_COV) report build-cov/run_tests -instr-profile=build-cov/run.profdata -ignore-filename-regex='third_party|tests/' include/dsa

LLVM_PROFDATA ?= llvm-profdata
LLVM_COV ?= llvm-cov

# Compiles every header on its own to check that each one includes what it needs.
headers:
	@for h in include/dsa/*.hpp; do \
		echo "#include \"$$(basename $$h)\"" | $(CXX) $(STD) $(WARN) -Iinclude/dsa -x c++ -fsyntax-only - || exit 1; \
	done

clean:
	rm -rf build build-san build-cov
