DESTDIR   ?=
PREFIX    ?= /usr/local
MANPREFIX ?= $(PREFIX)/man

CFLAGS += -ansi -g
# for SIGWINCH and cfmakeraw()
CFLAGS += -D_BSD_SOURCE
# for nanosleep() on GNU
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Wall -Wextra -pedantic

# required for BSD, glibc, musl; not macOS
LDLIBS_tritty = $(LDLIBS) -lutil

all: trickle tritty

clean:
	rm -f trickle tritty

install: trickle tritty
	install -d $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(MANPREFIX)/man1
	install trickle   tritty   $(DESTDIR)$(PREFIX)/bin/
	install trickle.1 tritty.1 $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/trickle
	rm -f $(DESTDIR)$(PREFIX)/bin/tritty
	rm -f $(DESTDIR)$(MANPREFIX)/man1/trickle.1
	rm -f $(DESTDIR)$(MANPREFIX)/man1/tritty.1

trickle: trickle.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

tritty: tritty.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS_tritty)

.PHONY: all clean install uninstall
