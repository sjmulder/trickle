trickle & tritty
================

Slow pipe and terminal.

 * Website: https://github.com/sjmulder/trickle
 * Video: https://asciinema.org/a/143994

**macOS note**: Homebrew users can easily install trickle and tritty with the
formula at https://github.com/nickolasburr/homebrew-pfa

trickle
-------

**trickle** [**-b** *bitrate*]

**trickle** is used as part of a shell pipeline and has very low throughput,
hence the name: data trickles through the pipe. By default, it passes on 600
bits per second, but this can be configured:

**-b** *bitrate*

(For "baud", bits per second.) Must be a number in range 50-57600. A decimal
separator and decimal 'k' suffix can be used.

tritty
------

**tritty** [**-b** *bitrate*] [*command* ...]

**tritty** ("trickle tty") spawns an interactive subterminal with very low
throughput. It simulates the experience of using a terminal over a slow
connection.

By default, it runs at 600 bits per second (full duplex) but this can be
configured:

**-b** *bitrate*

(For "baud", bits per second.) Must be a number in range 50-57600. A decimal
separator and decimal 'k' suffix can be used.

*command*

Will be executed inside the virtual terminal. It defaults to the *SHELL*
environment variable, or */bin/sh* if unset.

Note that by only limiting throughput the serial device experience is not
accurately simulated. In particular, the baud rate attribute on the virtual
terminal is not set. Software may use this attribute to adjust their output.

Use stty(1) from within a tritty session to adjust the baud rate attribute of
the terminal as shown in the examples below. The original settings will be
restored on exit.

Examples
--------

Print *README.rm* to the terminal at 600 baud:

    $ trickle <README.rm

List files at 2600 baud:

    $ ls | trickle -b2600

List files at 57600 baud:

    $ ls | trickle -b57.6k

Browse the web with a 14400 baud terminal:

    $ tritty -b14.4 elinks news.ycombinator.com

Run the default shell at 2600 baud:

    $ tritty -b2600   # now in the tritty session
    $ man man
      ...
    $ exit
    exit
    $                 # back to regular terminal

Run the default shell at 300 baud, also setting the terminal baud rate
attribute:

    $ tritty -b300
    $ sty 300
       ...
    $ exit

Building
--------

Should work on most Unix-like systems, perhaps with some tweaks. Tested on
FreeBSD, macOS, Linux and Linux with musl.

Edit the Makefile if desired. Then:

    make

There are *install* and *uninstall* targets, too. *PREFIX* is set to
*/usr/local* by default.

Author
------

By Sijmen J. Mulder (<ik@sjmulder.nl>)
