/*	$OpenBSD: curses.h,v 1.2 2003/06/03 01:52:39 millert Exp $	*/

/*
 * Placed in the public domain by Todd C. Miller <Todd.Miller@courtesan.com>
 * on June 17, 2003.
 */

#ifndef _CURSES_H_
#define _CURSES_H_

#ifdef _USE_OLD_CURSES_
#include <ocurses.h>
#else
#include <ncurses.h>
#endif

#endif /* _CURSES_H_ */
