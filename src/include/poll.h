/*	$OpenBSD$ */

/*
 * Typical poll() implimentations expect poll.h to be in /usr/include. 
 * However this is not a convenient place for the real definitions.
 */
#include <sys/poll.h>
