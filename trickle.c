#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <libutil.h>
#include <poll.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#define USAGE "usage: trickle [-b BITRATE] [COMMAND ...]"

int
main(int argc, char **argv)
{
	long rate = 600;
	int i, fdchild = -1;
	char c, *rest;
	double d;
	struct timespec delay;
	struct pollfd pollfds[2];
	struct winsize winsize;
	struct termios termios;

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

	pollfds[0].fd = STDIN_FILENO;
	pollfds[0].events = POLLIN;

	delay.tv_sec = 0;
	delay.tv_nsec = 8e9 / rate;

	if (argv[optind]) {
		if (!isatty(STDIN_FILENO) || !isatty(STDIN_FILENO)) {
			fputs("not a tty\n", stderr);
			return 1;
		}

		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) == -1) {
			perror("TIOCGWINSZ ioctl");
			return 1;
		}

		switch (forkpty(&fdchild, NULL, NULL, &winsize)) {
		case -1:
			perror("forkpty");
			return 1;
		case 0:
			execvp(argv[optind], argv+optind);
			perror("execvp");
			return 1;
		}

		if (tcgetattr(STDIN_FILENO, &termios) == -1) {
			perror("tcgetattr");
			return -1;
		}

		cfmakeraw(&termios);

		if (tcsetattr(STDIN_FILENO, TCSANOW, &termios) == -1) {
			perror("tcsetattr");
			return -1;
		}

		pollfds[1].fd = fdchild;
		pollfds[1].events = POLLIN;

		while (1) {
			poll(pollfds, 2, 0);

			if (pollfds[0].revents) {
				if (read(STDIN_FILENO, &c, 1) != 1)
					break;
				if (write(fdchild, &c, 1) != 1)
					break;
			}

			if (pollfds[1].revents) {
				if (read(fdchild, &c, 1) != 1)
					break;
				if (write(STDOUT_FILENO, &c, 1) != 1)
					break;
			}

			nanosleep(&delay, NULL);
		}
	} else if (isatty(STDIN_FILENO)) {
		fputs(USAGE "\n", stderr);
		return 1;
	} else {
		while (1) {
			poll(pollfds, 1, 0);

			if (pollfds[0].revents) {
				if (read(STDIN_FILENO, &c, 1) != 1)
					break;
				if (write(STDOUT_FILENO, &c, 1) != 1)
					break;
			}

			nanosleep(&delay, NULL);
		}
	}

	if (errno) {
		perror("I/O error");
		return 1;
	}

	if (fdchild != -1)
		close(fdchild);

	return 0;
}
