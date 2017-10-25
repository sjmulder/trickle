/* trickle.c - Copyright (c) 2017, Sijmen J. Mulder (see LICENSE) */

#define _BSD_SOURCE /* deprecated */
#define _DEFAULT_SOURCE

#define USAGE \
"usage: trickle [-b bitrate] <file\n" \
"   or  ... | trickle [-b bitrate]"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#include "trickle.h"

int
main(int argc, char **argv)
{
	struct opts opts;
	char c;

	parseopts(argc, argv, &opts);

	if (*opts.argv || isatty(STDIN_FILENO)) {
		fputs(USAGE "\n", stderr);
		return 1;
	}

	while (read(STDIN_FILENO, &c, 1) == 1) {
		if (write(STDOUT_FILENO, &c, 1) != 1)
			break;

		nanosleep(&opts.delay, NULL);
	}

	return 0;
}
