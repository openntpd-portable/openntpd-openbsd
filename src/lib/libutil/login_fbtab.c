/*	$OpenBSD: login_fbtab.c,v 1.8 2002/02/16 21:27:29 millert Exp $	*/

/************************************************************************
* Copyright 1995 by Wietse Venema.  All rights reserved.  Some individual
* files may be covered by other copyrights.
*
* This material was originally written and compiled by Wietse Venema at
* Eindhoven University of Technology, The Netherlands, in 1990, 1991,
* 1992, 1993, 1994 and 1995.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that this entire copyright notice
* is duplicated in all such copies.
*
* This software is provided "as is" and without any expressed or implied
* warranties, including, without limitation, the implied warranties of
* merchantibility and fitness for any particular purpose.
************************************************************************/
/*
    SYNOPSIS
	void login_fbtab(tty, uid, gid)
	char *tty;
	uid_t uid;
	gid_t gid;

    DESCRIPTION
	This module implements device security as described in the
	SunOS 4.1.x fbtab(5) and SunOS 5.x logindevperm(4) manual
	pages. The program first looks for /etc/fbtab. If that file
	cannot be opened it attempts to process /etc/logindevperm.
	We expect entries with the folowing format:

	    Comments start with a # and extend to the end of the line.

	    Blank lines or lines with only a comment are ignored.

	    All other lines consist of three fields delimited by
	    whitespace: a login device (/dev/console), an octal
	    permission number (0600), and a ":"-delimited list of
	    devices (/dev/kbd:/dev/mouse). All device names are
	    absolute paths. A path that ends in "*" refers to all
	    directory entries except "." and "..".

	    If the tty argument (relative path) matches a login device
	    name (absolute path), the permissions of the devices in the
	    ":"-delimited list are set as specified in the second
	    field, and their ownership is changed to that of the uid
	    and gid arguments.

    DIAGNOSTICS
	Problems are reported via the syslog daemon with severity
	LOG_ERR.

    BUGS
	This module uses strtok(3), which may cause conflicts with other
	uses of that same routine.

    AUTHOR
	Wietse Venema (wietse@wzv.win.tue.nl)
	Eindhoven University of Technology
	The Netherlands
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <paths.h>

#include "util.h"

#define _PATH_FBTAB	"/etc/fbtab"

static void login_protect(char *, char *, int, uid_t, gid_t);

#define	WSPACE		" \t\n"

/*
 * login_fbtab - apply protections specified in /etc/fbtab or logindevperm
 */
void
login_fbtab(tty, uid, gid)
	char	*tty;
	uid_t	uid;
	gid_t	gid;
{
	FILE	*fp;
	char	buf[BUFSIZ], *devname, *cp, *table;
	int	prot;

	if ((fp = fopen(table = _PATH_FBTAB, "r")) == NULL)
		return;

	while (fgets(buf, sizeof(buf), fp)) {
		if ((cp = strchr(buf, '#')))
			*cp = 0;	/* strip comment */
		if ((cp = devname = strtok(buf, WSPACE)) == 0)
			continue;	/* empty or comment */
		if (strncmp(devname, _PATH_DEV, sizeof(_PATH_DEV) - 1) != 0 ||
		    (cp = strtok((char *) 0, WSPACE)) == 0 ||
		    *cp != '0' ||
		    sscanf(cp, "%o", &prot) == 0 ||
		    prot == 0 ||
		    (prot & 0777) != prot ||
		    (cp = strtok((char *) 0, WSPACE)) == 0) {
			syslog(LOG_ERR, "%s: bad entry: %s", table,
			    cp ? cp : "(null)");
			continue;
		}
		if (strcmp(devname + sizeof(_PATH_DEV) - 1, tty) == 0)
			for (cp = strtok(cp, ":"); cp; cp = strtok(NULL, ":"))
				login_protect(table, cp, prot, uid, gid);
	}
	fclose(fp);
}

/*
 * login_protect - protect one device entry
 */
static void
login_protect(table, path, mask, uid, gid)
	char	*table;
	char	*path;
	int	mask;
	uid_t	uid;
	gid_t	gid;
{
	char	buf[BUFSIZ];
	int	pathlen = strlen(path);
	struct	dirent *ent;
	DIR	*dir;

	if (strcmp("/*", path + pathlen - 2) != 0) {
		if (chmod(path, mask) && errno != ENOENT)
			syslog(LOG_ERR, "%s: chmod(%s): %m", table, path);
		if (chown(path, uid, gid) && errno != ENOENT)
			syslog(LOG_ERR, "%s: chown(%s): %m", table, path);
	} else {
		strncpy(buf, path, sizeof buf);
		buf[pathlen - 1] = 0;
		if ((dir = opendir(buf)) == 0) {
			syslog(LOG_ERR, "%s: opendir(%s): %m", table, path);
		} else {
			while ((ent = readdir(dir))) {
				if (strcmp(ent->d_name, ".") &&
				    strcmp(ent->d_name, "..")) {
					strncpy(buf + pathlen - 1, ent->d_name,
					    sizeof(buf) - pathlen - 1);
					login_protect(table, buf, mask,
					    uid, gid);
				}
			}
			closedir(dir);
		}
	}
}
