CC=clang++
CFLAGS=-c -std=c++11 -Wall
LDFLAGS=-std=c++11 -Wall

SRCDIR=src
OBJDIR=obj
EXECUTABLE=termcalc

SOURCES=$(wildcard $(SRCDIR)/*.cpp)
HEADERS=$(wildcard $(SRCDIR)/*.h)

_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(_OBJECTS))

MAIN_OBJ=$(OBJDIR)/$(EXECUTABLE).o
TEST_OBJECTS=$(filter-out $(MAIN_OBJ), $(OBJECTS))

TESTS_DIR=tests

ifeq ($(OS),Windows_NT)
TEST_RUNNERS_DIR=$(TESTS_DIR)\\runners
else
TEST_RUNNERS_DIR=$(TESTS_DIR)/runners
endif

TEST_EXECUTABLE=test

all: $(OBJDIR) $(SOURCES) $(EXECUTABLE)

release-flags: 
	$(eval CFLAGS += -O2)

release: release-flags clean all

ifeq ($(OS),Windows_NT)
$(OBJDIR):
	mkdir $(OBJDIR)
else
$(OBJDIR):
	mkdir -p $(OBJDIR)
endif

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@	

test: $(TESTS_DIR)/test.h $(OBJDIR) $(TEST_OBJECTS)
	mkdir -p $(TEST_RUNNERS_DIR)
	cxxtestgen --error-printer -o $(TEST_RUNNERS_DIR)/test-runner.cpp $(TESTS_DIR)/test.h
	$(CC) $(LDFLAGS) -o $(TEST_EXECUTABLE) -I $(CXXTEST) $(TEST_OBJECTS) $(TEST_RUNNERS_DIR)/test-runner.cpp
	./$(TEST_EXECUTABLE)

run: $(OBJDIR) $(SOURCES) $(EXECUTABLE)
	./$(EXECUTABLE)

ifeq ($(OS),Windows_NT)
clean:
	del /S /Q $(OBJDIR)
	del /S /Q $(TEST_RUNNERS_DIR)
	del $(EXECUTABLE)
	del $(TEST_EXECUTABLE)
else
clean:
	rm -rf $(OBJDIR)
	rm -rf $(TEST_RUNNERS_DIR)
	rm -f $(EXECUTABLE)
	rm -f $(TEST_EXECUTABLE)
endif	
