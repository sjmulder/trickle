/* trickle.h - Copyright (c) 2017, Sijmen J. Mulder (see LICENSE.md) */

struct opts {
	struct timespec delay;
	char **argv;
};

static void
parseopts(int argc, char **argv, struct opts *opts)
{
	long rate = 600;
	int c;
	double d;
	char *rest;

	while ((c = getopt(argc, argv, "b:")) != -1) {
		switch (c) {
		case 'b':
			d = strtod(optarg, &rest);
			if (!*rest)
				;
			else if (!strcmp(rest, "k"))
				d *= 1000;
			else {
				fputs("bad -b format\n", stderr);
				exit(1);
			}
			rate = (long)d;
			break;
		default:
			fputs(USAGE "\n", stderr);
			exit(1);
		}
	}

	if (rate < 50 || rate > 57600) {
		fputs("bad -b, must be 50-57600\n", stderr);
		exit(1);
	}

	opts->delay.tv_sec = 0;
	opts->delay.tv_nsec = 8e9 / rate;
	opts->argv = argv + optind;
}
