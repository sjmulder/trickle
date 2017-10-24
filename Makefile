CFLAGS += -ansi -Wall
# required for BSD, glibc, musl; not macOS
LDLIBS_trtty = $(LDFLAGS) -lutil

all: trickle trtty

clean:
	rm -f trickle trtty

trickle: trickle.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

trtty: trtty.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS_trtty)

.PHONY: all clean
