CFLAGS += -ansi -Wall
LDLIBS = -lutil # for BSD, glibc, musl; not macOS

all: trickle
clean: ; rm -f trickle

.PHONY: all clean
