# Makefile for userspace rbtree library
# Adapted from Linux kernel implementation

CC = gcc
CFLAGS = -g -O2 -Wall -Wextra -fPIC
AR = ar
ARFLAGS = rcs

# Library names
STATIC_LIB = librbtree.a
SHARED_LIB = librbtree.so

# Source and object files
SRCS = rbtree.c
OBJS = $(SRCS:.c=.o)

# Header files to install
HEADERS = rbtree.h rbtree_types.h rbtree_augmented.h

# Test program
TEST_SRCS = rbtree-tst.c
TEST_PROG = rbtree-tst

# Default target
all: $(STATIC_LIB) $(SHARED_LIB) $(TEST_PROG)

# Build static library
$(STATIC_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Build shared library
$(SHARED_LIB): $(OBJS)
	$(CC) -shared -o $@ $^

# Build test program
$(TEST_PROG): $(TEST_SRCS) $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRCS) $(STATIC_LIB)

# Compile source files with -fPIC (works for both static and shared)
%.o: %.c rbtree.h rbtree_types.h rbtree_augmented.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(STATIC_LIB) $(SHARED_LIB) $(TEST_PROG)

# Install library and headers
install: $(STATIC_LIB) $(SHARED_LIB)
	install -d $(DESTDIR)/usr/local/lib
	install -d $(DESTDIR)/usr/local/include
	install -m 644 $(STATIC_LIB) $(DESTDIR)/usr/local/lib/
	install -m 755 $(SHARED_LIB) $(DESTDIR)/usr/local/lib/
	install -m 644 $(HEADERS) $(DESTDIR)/usr/local/include/

.PHONY: all clean install
