
LIB_NAME = lumber

SRC_DIR = src
SRC_FILES = \
	memtools.c \
	array.c \
	stack.c \
	binary_search_tree.c \
	red_black_tree.c
FULL_SRC_FILES = $(patsubst %.c,$(SRC_DIR)/%.c, $(SRC_FILES))

OBJECT_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

TEST_NAME = compiled_test_suite
TEST_DIR = tests
TEST_FILE_PREFIX = test
MAIN_TEST_FILE = $(TEST_DIR)/main.c
TEST_FILES = \
	common.c \
	memtools.c \
	array.c \
	stack.c \
	binary_search_tree.c \
	red_black_tree.c
FULL_TEST_FILES = $(patsubst %.c, $(TEST_DIR)/$(TEST_FILE_PREFIX)_%.c, $(TEST_FILES))
TEST_SEED = $(shell date +"%s")
TEST_SEED_MACRO_NAME = TEST_SEED

PACKAGES ?=
PKGCONFIG = pkg-config
COMPILER_FLAGS = -std=c99 -Wall
CFLAGS = $(if $(PACKAGES), $(shell $(PKGCONFIG) --cflags $(PACKAGES)),)
LIBS = $(if $(PACKAGES), $(shell $(PKGCONFIG) --libs $(PACKAGES)),)

LIB_FORGE = \
	-I/usr/include/forge \
	-lforge \
	-lm # math lib
MANUAL_LIBS = \
	-I/usr/include/sane \
	-lsane

all: compile library

compile: $(FULL_SRC_FILES)
	$(CC) -fpic -c $(FULL_SRC_FILES) $(COMPILER_FLAGS) $(CFLAGS) $(LIBS) $(MANUAL_LIBS) $(FULL_SRC_FILES)
library: $(OBJECT_FILES)
	$(CC) -shared $(OBJECT_FILES) -o lib$(LIB_NAME).so

test: $(FULL_SRC_FILES) $(MAIN_TEST_FILE) $(FULL_TEST_FILES)
	$(CC) \
		-D$(TEST_SEED_MACRO_NAME)=$(TEST_SEED) \
		$(FULL_SRC_FILES) \
		$(MAIN_TEST_FILE) \
		$(FULL_TEST_FILES) \
		$(COMPILER_FLAGS) \
		$(CFLAGS) \
		$(LIBS) \
		$(LIB_FORGE) \
		$(MANUAL_LIBS) \
		-o $(TEST_NAME)

debug: $(FULL_SRC_FILES) $(MAIN_TEST_FILE) $(FULL_TEST_FILES)
	$(CC) \
		-D$(TEST_SEED_MACRO_NAME)=$(TEST_SEED) \
		--verbose \
		-g \
		$(FULL_SRC_FILES) \
		$(MAIN_TEST_FILE) \
		$(FULL_TEST_FILES) \
		$(COMPILER_FLAGS) \
		$(CFLAGS) \
		$(LIBS) \
		$(LIB_FORGE) \
		$(MANUAL_LIBS) \
		-o $(TEST_NAME)

clean:
	$(RM) *.o *.so $(TEST_NAME) core* valgrind_report

.PHONY: all release test debug clean 
