#ifndef OPS_H
#define OPS_H

#include <fuse.h>
#include <stdlib.h>
#include "blockdev.h"
#include "lwext4/lwext4/ext4.h"

static inline struct ext4_blockdev *get_current_blockdev(void)
{
    return (struct ext4_blockdev *)fuse_get_context()->private_data;
}

static inline struct ext4_file *get_fi_file(struct fuse_file_info *fi)
{
    return (struct ext4_file *)fi->fh;
}

static inline void set_fi_file(struct fuse_file_info *fi, struct ext4_file *file)
{
    fi->fh = (uint64_t)file;
}

static inline ext4_dir *get_fi_dir(struct fuse_file_info *fi)
{
    return (ext4_dir *)fi->fh;
}

static inline void set_fi_dir(struct fuse_file_info *fi, ext4_dir *dir)
{
    fi->fh = (uint64_t)dir;
}

static inline ext4_file *alloc_ext4_file(void)
{
    return calloc(1, sizeof(ext4_file));
}

static inline void free_ext4_file(ext4_file *f)
{
    free(f);
}

static inline ext4_dir *alloc_ext4_dir(void)
{
    return calloc(1, sizeof(ext4_dir));
}

static inline void free_ext4_dir(ext4_dir *d)
{
    free(d);
}

void *op_init(struct fuse_conn_info *info);
void op_destroy(void *);
int op_readlink(const char *path, char *buf, size_t bufsize);
int op_read(const char *path, char *buf, size_t size, off_t offset
                             , struct fuse_file_info *fi);
int op_write(const char *path, const char *buf, size_t size, off_t offset
                             , struct fuse_file_info *fi);
int op_truncate (const char *path, off_t length);
int op_ftruncate (const char *path, off_t length, struct fuse_file_info *fi);
int op_readdir(const char *path, void *buf, fuse_fill_dir_t filler
                               , off_t offset, struct fuse_file_info *fi);
int op_getattr(const char *path, struct stat *stbuf);
int op_open(const char *path, struct fuse_file_info *fi);
int op_release(const char *path, struct fuse_file_info *fi);

int op_opendir(const char *path, struct fuse_file_info *fi);
int op_releasedir(const char *path, struct fuse_file_info *fi);
int op_getattr(const char *path, struct stat *stbuf);

int op_mkdir(const char *path, mode_t mode);
int op_rmdir(const char *path);
int op_unlink(const char *path);

#endif