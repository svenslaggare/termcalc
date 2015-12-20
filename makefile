CC=clang++
CFLAGS=-std=c++11 -Wall

SRC_DIR=src
OBJ_DIR=obj
EXECUTABLE=termcalc

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS=$(wildcard $(SRC_DIR)/*.h)

_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_OBJECTS))

MAIN_OBJ=$(OBJ_DIR)/$(EXECUTABLE).o
TEST_OBJECTS=$(filter-out $(MAIN_OBJ), $(OBJECTS))

TESTS_DIR=tests

ifeq ($(OS),Windows_NT)
TEST_RUNNERS_DIR=$(TESTS_DIR)\\runners
else
TEST_RUNNERS_DIR=$(TESTS_DIR)/runners
endif

TESTS=$(wildcard $(TESTS_DIR)/*.h)
TEST_EXECUTABLES=$(patsubst $(TESTS_DIR)/%.h,$(TEST_RUNNERS_DIR)/%, $(TESTS))

all: $(OBJ_DIR) $(SOURCES) $(EXECUTABLE)

release-flags:
	$(eval CFLAGS += -O2)

release: release-flags clean all

ifeq ($(OS),Windows_NT)
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(TEST_RUNNERS_DIR):
	mkdir $(TEST_RUNNERS_DIR)
else
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TEST_RUNNERS_DIR):
	mkdir -p $(TEST_RUNNERS_DIR)
endif

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

test: $(TEST_RUNNERS_DIR) $(TEST_EXECUTABLES)

$(TEST_RUNNERS_DIR)/%: $(TESTS_DIR)/%.h $(OBJ_DIR) $(TEST_OBJECTS)
	cxxtestgen --error-printer -o $@-runner.cpp $<
	$(CC) $(CFLAGS) -o $@ -I $(CXXTEST) $(TEST_OBJECTS) $@-runner.cpp
	./$@

run: $(OBJ_DIR) $(SOURCES) $(EXECUTABLE)
	./$(EXECUTABLE)

ifeq ($(OS),Windows_NT)
clean:
	del /S /Q $(OBJ_DIR)
	del /S /Q $(TEST_RUNNERS_DIR)
	del $(EXECUTABLE)
	del $(TEST_EXECUTABLE)
else
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TEST_RUNNERS_DIR)
	rm -f $(EXECUTABLE)
	rm -f $(TEST_EXECUTABLE)
endif
