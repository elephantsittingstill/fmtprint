CC = gcc
AR = ar
CFLAGS = -Iinclude -Wall -Wextra -std=c89 -g #-fsanitize=address
OBJDIR = build
SRCDIR = src
TEST_DIR = test
LIBNAME = libfmtprint.a

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
TARGET = $(TEST_DIR)/test
TEST_SRC = $(TEST_DIR)/test.c

.PHONY: all clean test lib


all: $(TARGET)

lib: $(OBJ)
	$(AR) rcs $(LIBNAME) $^

$(TARGET): $(OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

test:
	./test/run_tests.sh
