CC = cc
CFLAGS = 
PREFIX = /usr/local
OBJS_COMMON = lookup.o delimiter.o libarg.o
OBJS_FINDEX = findex.o
OBJS_FIDXGEN = findex_gen.o
OBJS_FIDXPRM = findex_prompt.o


.PHONY: all clean

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS) findex findex_gen findex_prompt

clean:
	rm -f $(OBJS_COMMON) $(OBJS_FINDEX) $(OBJS_FIDXGEN) $(OBJS_FIDXPRM)
	rm -f findex findex_gen findex_prompt

findex: $(OBJS_COMMON) $(OBJS_FINDEX)
	$(CC) $(OBJS_COMMON) $(OBJS_FINDEX) $(CFLAGS) -o findex

findex_gen: $(OBJS_COMMON) $(OBJS_FIDXGEN)
	$(CC) $(OBJS_COMMON) $(OBJS_FIDXGEN) $(CFLAGS) -o findex_gen

findex_prompt: $(OBJS_COMMON) $(OBJS_FIDXPRM)
	$(CC) $(OBJS_COMMON) $(OBJS_FIDXPRM) $(CFLAGS) -o findex_prompt

install: $(OBJS) findex
	cp findex $(PREFIX)/bin/findex
	cp findex_gen $(PREFIX)/bin/findex_gen
	cp findex_prompt $(PREFIX)/bin/findex_prompt
	cp ../doc/findex.1 $(PREFIX)/share/man/man1/findex.1
	cp ../doc/findex_gen.1 $(PREFIX)/share/man/man1/findex_gen.1

uninstall:
	rm -f $(PREFIX)/bin/findex
	rm -f $(PREFIX)/bin/findex_gen
	rm -f $(PREFIX)/bin/findex_prompt
	rm -f $(PREFIX)/share/man/man1/findex.1
	rm -f $(PREFIX)/share/man/man1/findex_gen.1
