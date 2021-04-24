/*
 * Copyright (c) 2015, Kaho Ng, ngkaho1234@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
 */


#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>

#include "ops.h"
#include "lwext4.h"

/* Copied from http://web.mit.edu/~tcoffee/Public/rss/common/timespec.c */
static void timespec_now(struct fuse_timespec *ts)
{
	struct timeval  tv;

	gettimeofday(&tv, NULL);
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec * 1000;
}

static uint32_t timespec_to_second(const struct fuse_timespec *ts)
{
	return ts->tv_sec;
}

#if !defined(__FreeBSD__)
int op_utimens(const char *path, const struct fuse_timespec tv[2])
{
	int rc;
	uint32_t atime, mtime;
#if defined(UTIME_OMIT) && defined(UTIME_NOW)
	if (tv[0].tv_nsec != UTIME_OMIT) {
		if (tv[0].tv_nsec == UTIME_NOW) {
			struct fuse_timespec ts;
			timespec_now(&ts);
			atime = timespec_to_second(&ts);
		} else
#endif /* defined(UTIME_OMIT) && defined(UTIME_NOW) */
			atime = timespec_to_second(tv);

		rc = LWEXT4_CALL(ext4_atime_set, path, atime);
		if (rc)
			return rc;

#if defined(UTIME_OMIT) && defined(UTIME_NOW)
	}
	if (tv[1].tv_nsec != UTIME_OMIT) {
		if (tv[1].tv_nsec == UTIME_NOW) {
			struct fuse_timespec ts;
			timespec_now(&ts);
			mtime = timespec_to_second(&ts);
		} else
#endif /* defined(UTIME_OMIT) && defined(UTIME_NOW) */
			mtime = timespec_to_second(tv);

		rc = LWEXT4_CALL(ext4_mtime_set, path, mtime);
		if (rc)
			return rc;

#if defined(UTIME_OMIT) && defined(UTIME_NOW)
	}
#endif /* defined(UTIME_OMIT) && defined(UTIME_NOW) */
	return 0;

}
#endif

int op_utimes(const char *path, struct fuse_utimbuf *utimbuf)
{
	int rc;
	time_t atime, mtime, ctime;
	struct fuse_timespec ts;
	timespec_now(&ts);
	if (!utimbuf) {
		atime = mtime = timespec_to_second(&ts);
	} else {
		atime = utimbuf->actime;
		mtime = utimbuf->modtime;
	}
	ctime = timespec_to_second(&ts);
	rc = LWEXT4_CALL(ext4_atime_set, path, atime);
	if (rc)
		return rc;

	rc = LWEXT4_CALL(ext4_mtime_set, path, mtime);
	if (rc)
		return rc;

	rc = LWEXT4_CALL(ext4_ctime_set, path, ctime);
	return rc;
}
