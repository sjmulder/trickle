PREFIX ?= /usr/local
CFLAGS += -ansi -Wall
# required for BSD, glibc, musl; not macOS
LDLIBS_tritty = $(LDFLAGS) -lutil

all: trickle tritty

clean:
	rm -f trickle tritty

install: trickle tritty
	install -d $(PREFIX)/bin \
	           $(PREFIX)/share/man/man1 \
		   $(PREFIX)/share/doc/trickle
	install trickle tritty $(PREFIX)/bin/
	install trickle.1 tritty.1 $(PREFIX)/share/man/man1/
	install README.md LICENSE.md $(PREFIX)/share/doc/trickle/

uninstall:
	rm -f $(PREFIX)/bin/trickle \
	      $(PREFIX)/bin/trittty \
	      $(PREFIX)/share/man/man1/trickle.1 \
	      $(PREFIX)/share/man/man1/tritty.1 \
	      $(PREFIX)/share/doc/trickle/README.md \
	      $(PREFIX)/share/doc/trickle/LICENSE.md
	rmdir -p $(PREFIX)/share/doc/trickle/

trickle: trickle.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

tritty: tritty.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS_tritty)

.PHONY: all clean install uninstall
