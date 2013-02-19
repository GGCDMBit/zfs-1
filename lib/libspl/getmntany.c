/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Copyright 2006 Ricardo Correia.  All rights reserved.
 * Use is subject to license terms.
 */

/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved	*/

#include <stdio.h>
#include <string.h>
#include <mntent.h>
#include <sys/mnttab.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE (MNT_LINE_MAX + 2)

/*__thread*/ char buf[BUFSIZE];

#define DIFF(xx) ((mrefp->xx != NULL) && \
		  (mgetp->xx == NULL || strcmp(mrefp->xx, mgetp->xx) != 0))

int
getmntany(FILE *fp, struct mnttab *mgetp, struct mnttab *mrefp)
{
        struct statfs *sfsp;
        int nitems;

        nitems = getmntinfo(&sfsp, MNT_WAIT);

        while (nitems-- > 0) {
                if (strcmp(mrefp->mnt_fstype, sfsp->f_fstypename) == 0 &&
                    strcmp(mrefp->mnt_special, sfsp->f_mntfromname) == 0) {
                        mgetp->mnt_special = sfsp->f_mntfromname;
                        mgetp->mnt_mountp = sfsp->f_mntonname;
                        mgetp->mnt_fstype = sfsp->f_fstypename;
                        mgetp->mnt_mntopts = "";
                        return (0);
                }
                ++sfsp;
        }
        return (-1);
}

char *
mntopt(char **p)
{
        char *cp = *p;
        char *retstr;

        while (*cp && isspace(*cp))
                cp++;

        retstr = cp;
        while (*cp && *cp != ',')
                cp++;

        if (*cp) {
                *cp = '\0';
                cp++;
        }

        *p = cp;
        return (retstr);
}

char *
hasmntopt(struct mnttab *mnt, char *opt)
{
        char tmpopts[MNT_LINE_MAX];
        char *f, *opts = tmpopts;

        if (mnt->mnt_mntopts == NULL)
                return (NULL);
        (void) strcpy(opts, mnt->mnt_mntopts);
        f = mntopt(&opts);
        for (; *f; f = mntopt(&opts)) {
                if (strncmp(opt, f, strlen(opt)) == 0)
                        return (f - tmpopts + mnt->mnt_mntopts);
        }
        return (NULL);
}


int
getextmntent(FILE *fp, struct extmnttab *mp, int len)
{
	int ret;
	struct stat64 st;

	ret = _sol_getmntent(fp, (struct mnttab *) mp);
	if (ret == 0) {
		if (stat64(mp->mnt_mountp, &st) != 0) {
			mp->mnt_major = 0;
			mp->mnt_minor = 0;
			return ret;
		}
		mp->mnt_major = major(st.st_dev);
		mp->mnt_minor = minor(st.st_dev);
	}

	return ret;
}
