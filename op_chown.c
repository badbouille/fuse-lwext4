/*
 * Copyright (c) 2015, Kaho Ng, ngkaho1234@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See README and COPYING for
 * more details.
 */
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <inttypes.h>

#include "logging.h"
#include "ops.h"
#include "lwext4.h"

int op_chown(const char *path, fuse_uid_t uid, fuse_gid_t gid)
{
	return LWEXT4_CALL(ext4_owner_set, path, uid, gid);
}

