CC ?= cc
CFLAGS ?= -Wall -Wextra -Wpedantic -Wshadow -Wdouble-promotion -Wconversion -Wno-unused-parameter -fpic
PREFIX ?= /usr/bin

OBJS=src/main.o src/lookup.o src/delimiter.o src/libarg/libarg.o

.PHONY: all clean

all: $(OBJS) findex

clean:
	$(RM) $(OBJS)
	$(RM) findex

findex: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o findex

install: $(OBJS) findex
	mv findex $(PREFIX)/findex
