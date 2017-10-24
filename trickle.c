#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#define USAGE "usage: trickle [-b BITRATE]"

int
main(int argc, char **argv)
{
	long rate = 600;
	int i;
	char c, *rest;
	double d;
	struct timespec delay;

	while ((i = getopt(argc, argv, "b:")) != -1) {
		switch (i) {
		case 'b':
			d = strtod(optarg, &rest);
			if (!*rest)
				;
			else if (!strcmp(rest, "k"))
				d *= 1000;
			else {
				fputs("bad -b format\n", stderr);
				return 1;
			}
			rate = (long)d;	
			break;
		default:
			fputs(USAGE "\n", stderr);
			return 1;
		}
	}

	if (rate < 50 || rate > 57600) {
		fputs("bad -b, must be 50-57600\n", stderr);
		return 1;
	}

	if (argv[optind] || isatty(STDIN_FILENO)) {
		fputs(USAGE "\n", stderr);
		return 1;
	}

	delay.tv_sec = 0;
	delay.tv_nsec = 8e9 / rate;

	while (read(STDIN_FILENO, &c, 1) == 1) {
		if (write(STDOUT_FILENO, &c, 1) != 1)
			break;

		nanosleep(&delay, NULL);
	}

	return 0;
}
