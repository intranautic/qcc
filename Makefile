CC         = gcc
MAIN       = qcc

SRC_DIR    = src
HEADER_DIR = include

HEADERS    = $(wildcard include/$(MAIN)/*.h)
CFLAGS     = -Wall -D_DEFAULT_SOURCE $(DEBUG) -std=c99 -O2 -I include

SRCS       = $(wildcard src/*.c)
OBJS       = $(SRCS:%.c=%.o)

TESTS_DIR  = tests
TESTS_SRCS = $(wildcard tests/*.c)
TESTS_BINS = $(patsubst %.c,%,$(TESTS_SRCS))

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(MAIN)

debug:
	$(MAKE) all DEBUG=-DDEBUG

clean:
	rm -rf $(MAIN) $(OBJS)
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;

format:
	clang-format -i $(SRCS) $(HEADERS)

.PHONY: debug clean format
