CFLAGS += -ansi -Wall

# for BSD, glibc, musl; not macOS
trtty: LDLIBS += -lutil

all: trickle trtty
clean: ; rm -f trickle trtty

.PHONY: all clean
