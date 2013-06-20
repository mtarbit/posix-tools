POSIX Tools
===========

This is a probably misguided attempt to re-create a few of the simpler
standard shell tools and utilities from scratch, mainly as a practice exercise
in C and *nix programming. I'm aiming for compliance with the shell & utilities
section of [POSIX.1-2008](http://pubs.opengroup.org/onlinepubs/9699919799/).

If it's not immediately obvious I don't really know what I'm doing, so
don't go relying on these unless you want your machine to go up in a puff
of blue smoke.

### Complete implementations

 * [basename](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/basename.html)
 * [cat](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/cat.html)
 * [dirname](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/dirname.html)
 * [echo](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/echo.html)
 * [env](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/env.html)
 * [false](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/false.html)
 * [pwd](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/pwd.html)
 * [rm](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/rm.html)
 * [true](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/true.html)
 * [wc](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/wc.html)

### Partial implementations

 * [cal](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/cal.html) - Only handles Gregorian calendar dates correctly (14th Sep 1752 onwards)
 * [ls](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/ls.html) - Does basic listings, but most of the formatting and options are still to be done
 * [tail](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/tail.html) - Only supports tailing by characters, -n and -f still to be done

### Prior art

Some other implementations you might be familiar with:

 * https://gist.github.com/dchest/1091803
 * http://www.opensource.apple.com/source/shell_cmds/shell_cmds-170/
 * http://git.savannah.gnu.org/cgit/coreutils.git/tree/src
 * http://plan9.bell-labs.com/sources/plan9/sys/src/cmd/
