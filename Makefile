
LIB_NAME = lumber
TEST_NAME = test

SRC_DIR = src
SRC_FILES = \
	binary_search_tree.c \
	red_black_tree.c \
	stack.c
FULL_SRC_FILES = $(patsubst %.c,$(SRC_DIR)/%.c, $(SRC_FILES))

OBJECT_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

TEST_DIR = spec
TEST_FILES = \
	$(TEST_DIR)/main.c \
	$(TEST_DIR)/spec_stack.c \
	$(TEST_DIR)/spec_binary_search_tree.c \
	$(TEST_DIR)/spec_red_black_tree.c \
	$(TEST_DIR)/spec_common.c
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

test: $(FULL_SRC_FILES) $(TEST_FILES)
	$(CC) \
		-D$(TEST_SEED_MACRO_NAME)=$(TEST_SEED) \
		$(FULL_SRC_FILES) \
		$(TEST_FILES) \
		$(COMPILER_FLAGS) \
		$(CFLAGS) \
		$(LIBS) \
		$(LIB_FORGE) \
		$(MANUAL_LIBS) \
		-o $(TEST_NAME)

debug: $(FULL_SRC_FILES) $(TEST_FILES)
	$(CC) \
		-D$(TEST_SEED_MACRO_NAME)=$(TEST_SEED) \
		--verbose \
		-g \
		$(FULL_SRC_FILES) \
		$(TEST_FILES) \
		$(COMPILER_FLAGS) \
		$(CFLAGS) \
		$(LIBS) \
		$(LIB_FORGE) \
		$(MANUAL_LIBS) \
		-o $(TEST_NAME)

clean:
	$(RM) *.o *.so $(TEST_NAME) core*

.PHONY: all release test debug clean 
