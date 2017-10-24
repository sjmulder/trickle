CFLAGS += -ansi -Wall
# required for BSD, glibc, musl; not macOS
LDLIBS_tritty = $(LDFLAGS) -lutil

all: trickle tritty

clean:
	rm -f trickle tritty

trickle: trickle.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

tritty: tritty.c trickle.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS_tritty)

.PHONY: all clean
