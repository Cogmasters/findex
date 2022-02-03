CC = cc
CFLAGS = 
PREFIX = /usr/local
OBJS_COMMON = src/lookup.o src/delimiter.o src/libarg/libarg.o
OBJS_FINDEX = src/findex.o
OBJS_FIDXGEN = src/findex_gen.o


.PHONY: all clean

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS) findex findex_gen

clean:
	rm -f $(OBJS_COMMON) $(OBJS_FINDEX) $(OBJS_FIDXGEN)
	rm -f findex findex_gen

findex: $(OBJS_COMMON) $(OBJS_FINDEX)
	$(CC) $(OBJS_COMMON) $(OBJS_FINDEX) $(CFLAGS) -o findex

findex_gen: $(OBJS_COMMON) $(OBJS_FIDXGEN)
	$(CC) $(OBJS_COMMON) $(OBJS_FIDXGEN) $(CFLAGS) -o findex_gen

install: $(OBJS) findex
	cp findex $(PREFIX)/bin/findex
	cp findex_gen $(PREFIX)/bin/findex_gen
	cp doc/findex.1 $(PREFIX)/share/man/man1/findex.1
	cp doc/findex_gen.1 $(PREFIX)/share/man/man1/findex_gen.1

uninstall:
	rm -f $(PREFIX)/bin/findex
	rm -f $(PREFIX)/bin/findex_gen
	rm -f $(PREFIX)/share/man/man1/findex.1
	rm -f $(PREFIX)/share/man/man1/findex_gen.1
