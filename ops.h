#ifndef OPS_H
#define OPS_H

#include <stdint.h>

#include <fuse.h>
#include <stdlib.h>
#include "blockdev.h"

/* Custom redefinitions for Winfsp usage */
/* Imported from Winfsp Cygwin configuration */
#ifdef __linux__
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <utime.h>

#define fuse_uid_t                      uid_t
#define fuse_gid_t                      gid_t
#define fuse_pid_t                      pid_t

#define fuse_dev_t                      dev_t
#define fuse_ino_t                      ino_t
#define fuse_mode_t                     mode_t
#define fuse_nlink_t                    nlink_t
#define fuse_off_t                      off_t

#define fuse_fsblkcnt_t                 fsblkcnt_t
#define fuse_fsfilcnt_t                 fsfilcnt_t
#define fuse_blksize_t                  blksize_t
#define fuse_blkcnt_t                   blkcnt_t

#define fuse_utimbuf                    utimbuf
#define fuse_timespec                   timespec

#define fuse_stat                       stat

#define fuse_statvfs                    statvfs
#define fuse_flock                      flock
#endif

/* fuse-lwext4 options. */
struct fuse_lwext4_options {
	char *disk;
	char *logfile;
	int debug;
	int journal;
	int cache;
};

static inline struct ext4_blockdev *get_current_blockdev(void)
{
	return (struct ext4_blockdev *)fuse_get_context()->private_data;
}

static inline struct ext4_file *get_fi_file(struct fuse_file_info *fi)
{
	return (struct ext4_file *)(uintptr_t)fi->fh;
}

static inline void set_fi_file(struct fuse_file_info *fi, struct ext4_file *file)
{
	fi->fh = (uint64_t)(uintptr_t)file;
}

static inline ext4_dir *get_fi_dir(struct fuse_file_info *fi)
{
	return (ext4_dir *)(uintptr_t)fi->fh;
}

static inline void set_fi_dir(struct fuse_file_info *fi, ext4_dir *dir)
{
	fi->fh = (uint64_t)(uintptr_t)dir;
}

static inline ext4_file *alloc_ext4_file(void)
{
	return (ext4_file *)calloc(1, sizeof(ext4_file));
}

static inline void free_ext4_file(ext4_file *f)
{
	free(f);
}

static inline ext4_dir *alloc_ext4_dir(void)
{
	return (ext4_dir *)calloc(1, sizeof(ext4_dir));
}

static inline void free_ext4_dir(ext4_dir *d)
{
	free(d);
}

void *op_init(struct fuse_conn_info *info);
void op_destroy(void *);
int op_readlink(const char *path, char *buf, size_t bufsize);
int op_read(const char *path, char *buf, size_t size, fuse_off_t offset,
	    struct fuse_file_info *fi);
int op_write(const char *path, const char *buf, size_t size, fuse_off_t offset,
		struct fuse_file_info *fi);
int op_truncate (const char *path, fuse_off_t length);
int op_ftruncate (const char *path, fuse_off_t length, struct fuse_file_info *fi);
int op_readdir(const char *path, void *buf, fuse_fill_dir_t filler, fuse_off_t offset,
		struct fuse_file_info *fi);
int op_getattr(const char *path, struct fuse_stat *stbuf);
int op_open(const char *path, struct fuse_file_info *fi);
int op_create(const char *path, fuse_mode_t mode, struct fuse_file_info *fi);
int op_release(const char *path, struct fuse_file_info *fi);

int op_opendir(const char *path, struct fuse_file_info *fi);
int op_releasedir(const char *path, struct fuse_file_info *fi);
int op_getattr(const char *path, struct fuse_stat *stbuf);

int op_mkdir(const char *path, fuse_mode_t mode);
int op_rmdir(const char *path);
int op_link(const char *path, const char *hardlink_path);
int op_unlink(const char *path);
int op_symlink(const char *target, const char *path);
int op_readlink(const char *path, char *buf, size_t bufsiz);

int op_rename(const char *path, const char *new_path);

int op_chmod(const char *path, fuse_mode_t mode);
int op_chown(const char *path, fuse_uid_t uid, fuse_gid_t gid);

int op_statvfs(const char *path, struct fuse_statvfs *statvfs);

int op_setxattr(const char *path, const char *name,
		const char *value, size_t size, int flags);

#if defined(__APPLE__)
int op_getxattr(const char *path, const char *name,
		char *value, size_t size, uint32_t position);
#else
int op_getxattr(const char *path, const char *name,
		char *value, size_t size);
#endif

int op_listxattr(const char *path, char *list, size_t size);
int op_removexattr(const char *path, const char *name);

#if !defined(__FreeBSD__)
int op_utimens(const char *path, const struct fuse_timespec tv[2]);
#endif

int op_utimes(const char *path, struct fuse_utimbuf *utimbuf);

#endif
