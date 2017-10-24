#define _BSD_SOURCE /* deprecated */
#define _DEFAULT_SOURCE

#define USAGE "usage: trtty [-b BITRATE] [COMMAND [ARG ...]]"

#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#if defined(__APPLE__)
# include <util.h>
#elif defined(BSD)
# include <libutil.h>
#else
# include <pty.h>
#endif

#include "trickle.h"

static int fdchild;
static struct termios termios_orig;

static void
onsigwinch(int sig)
{
	struct winsize winsize;

	if (!fdchild)
		return;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) == -1)
		return;

	ioctl(fdchild, TIOCSWINSZ, &winsize);
}

static void
restoreterm(void)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &termios_orig);
}

int
main(int argc, char **argv)
{
	char c, *shell;
	struct opts opts;
	struct pollfd pollfds[2];
	struct winsize winsize;
	struct termios termios;

	parseopts(argc, argv, &opts);

	if (!isatty(STDIN_FILENO) || !isatty(STDIN_FILENO)) {
		fputs("not a tty\n", stderr);
		return 1;
	}

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize) == -1) {
		perror("TIOCGWINSZ ioctl");
		return 1;
	}

	signal(SIGWINCH, onsigwinch);

	switch (forkpty(&fdchild, NULL, NULL, &winsize)) {
	case -1:
		perror("forkpty");
		return 1;
	case 0:
		if (*opts.argv)
			execvp(*opts.argv, opts.argv);
		else if ((shell = getenv("SHELL")))
			execl(shell, shell, NULL);
		else
			execl("/bin/sh", "/bin/sh", NULL);
		perror("exec");
		return 1;
	}

	if (tcgetattr(STDIN_FILENO, &termios) == -1) {
		perror("tcgetattr");
		return -1;
	}

	termios_orig = termios;
	atexit(restoreterm);
	cfmakeraw(&termios);

	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &termios) == -1) {
		perror("tcsetattr");
		return -1;
	}

	pollfds[0].fd = STDIN_FILENO;
	pollfds[0].events = POLLIN;
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

		nanosleep(&opts.delay, NULL);
	}

	close(fdchild);

	return 0;
}
