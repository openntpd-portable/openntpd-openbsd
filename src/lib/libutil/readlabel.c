/*	$OpenBSD: readlabel.c,v 1.7 2002/02/21 16:22:23 deraadt Exp $	*/

/*
 * Copyright (c) 1996, Jason Downs.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <string.h>
#include <unistd.h>
#include <sys/dkio.h>
#define DKTYPENAMES
#include <sys/disklabel.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>

#include "util.h"

/*
 * Try to get a disklabel for the specified device, and return mount_xxx
 * style filesystem type name for the specified partition.
 */

char *readlabelfs(device, verbose)
	char *device;
	int verbose;
{
	char rpath[MAXPATHLEN];
	char part, *type;
	struct stat sbuf;
	struct disklabel dk;
	int fd;

	/* Assuming device is of the form /dev/??p, build a raw partition. */
	if (stat(device, &sbuf) < 0) {
		if (verbose)
			warn("%s", device);
		return(NULL);
	}
	switch(sbuf.st_mode & S_IFMT) {
	case S_IFCHR:
		/* Ok... already a raw device.  Hmm. */
		strncpy(rpath, device, sizeof(rpath));
		rpath[sizeof(rpath) - 1] = '\0';

		/* Change partition name. */
		part = rpath[strlen(rpath) - 1];
		rpath[strlen(rpath) - 1] = 'a' + getrawpartition();
		break;
	case S_IFBLK:
		if (strlen(device) > sizeof(_PATH_DEV) - 1) {
			snprintf(rpath, sizeof(rpath), "%sr%s", _PATH_DEV,
			    &device[sizeof(_PATH_DEV) - 1]);

			/* Change partition name. */
			part = rpath[strlen(rpath) - 1];
			rpath[strlen(rpath) - 1] = 'a' + getrawpartition();
			break;
		}
	default:
		if (verbose)
			warnx("%s: not a device node", device);
		return(NULL);
	}

	/* If rpath doesn't exist, change that partition back. */
	fd = open(rpath, O_RDONLY);
	if (fd < 0) {
		if (errno == ENOENT) {
			rpath[strlen(rpath) - 1] = part;

			fd = open(rpath, O_RDONLY);
			if (fd < 0) {
				if (verbose)
					warn("%s", rpath);
				return(NULL);
			}
		} else {
				if (verbose)
					warn("%s", rpath);
				return(NULL);
		}
	}
	if (ioctl(fd, DIOCGDINFO, &dk) < 0) {
		if (verbose)
			warn("%s: couldn't read disklabel", rpath);
		close(fd);
		return(NULL);
	}
	close(fd);

	if (dk.d_partitions[part - 'a'].p_fstype > FSMAXTYPES) {
		if (verbose)
			warnx("%s: bad filesystem type in label", rpath);
		return(NULL);
	}

	type = fstypesnames[dk.d_partitions[part - 'a'].p_fstype];
	return((type[0] == '\0') ? NULL : type);
}
