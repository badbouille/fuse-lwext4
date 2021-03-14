#ifndef BLOCKDEV_H
#define BLOCKDEV_H

#include "lwext4.h"

int blockdev_get(const char *fname, struct ext4_blockdev **pbdev);
void blockdev_put(struct ext4_blockdev *bdev);

#endif
