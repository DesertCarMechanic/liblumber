
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

HEADER_FILES = \
	memtools.h \
	array.h \
	stack.h \
	binary_search_tree.h \
	red_black_tree.h

PACKAGES ?=
PKGCONFIG = pkg-config
COMPILER_FLAGS = -std=c99 -Wall
CFLAGS = $(if $(PACKAGES), $(shell $(PKGCONFIG) --cflags $(PACKAGES)),)
LIBS = $(if $(PACKAGES), $(shell $(PKGCONFIG) --libs $(PACKAGES)),)

INSTALL_DIR = /usr/lib
INCLUDE_DIR = /usr/include/$(LIB_NAME)

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
	$(CC) -shared $(OBJECT_FILES) -o $(LIB_NAME).so

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

install: $(LIB_NAME).so $(patsubst %.h, $(SRC_DIR)/%.h, $(HEADER_FILES))
	chmod 755 $(LIB_NAME).so
	cp $(LIB_NAME).so $(INSTALL_DIR)
	mkdir -p $(INCLUDE_DIR)
	for header in $(HEADER_FILES); do \
		cp $(SRC_DIR)/$$header $(INCLUDE_DIR); \
	done
	ldconfig -n $(INSTALL_DIR)

uninstall:
	$(RM) $(INSTALL_DIR)/$(LIB_NAME).so
	for header in $(HEADER_FILES); do \
		$(RM) $(INCLUDE_DIR)/$$header; \
	done
	rmdir $(INCLUDE_DIR)

clean:
	$(RM) *.o *.so $(TEST_NAME) core* valgrind_report

.PHONY: all release test debug clean 
