CFLAGS += -ansi -Wall
LDLIBS = -lutil

all: trickle
clean: ; rm -f trickle

.PHONY: all clean
