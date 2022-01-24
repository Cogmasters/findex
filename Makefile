CC = cc
CFLAGS = 
PREFIX = /usr/bin
OBJS_COMMON = src/lookup.o src/delimiter.o src/libarg/libarg.o
OBJS_FINDEX = src/main.o
OBJS_FIDXGEN = src/generate.o


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
	mv findex $(PREFIX)/findex
