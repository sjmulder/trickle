# BSD conventions by default, override to taste
prefix  ?= /usr/local
bindir  ?= $(prefix)/bin
man1dir ?= $(prefix)/man/man1

CFLAGS += -ansi -g
CFLAGS += -D_POSIX_C_SOURCE=199309L
CFLAGS += -Wall -Wextra -pedantic

# required for BSD, glibc, musl; not macOS
LDLIBS_tritty = $(LDLIBS) -lutil

all: trickle tritty

clean:
	rm -f trickle tritty

install: trickle tritty
	install -d $(bindir) $(man1dir)
	install trickle tritty $(bindir)/
	install trickle.1 tritty.1 $(man1dir)/

uninstall:
	rm -f $(bindir)/trickle \
	      $(bindir)/trittty \
	      $(man1dir)/trickle.1 \
	      $(man1dir)/tritty.1

trickle: trickle.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

tritty: tritty.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS_tritty)

.PHONY: all clean install uninstall
