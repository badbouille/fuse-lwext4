#ifndef ACL_H
#define ACL_H

#include <stdint.h>
#include <stdlib.h>

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/acl.h>
#else
/*=== Constants ===*/

/* 23.2.5 acl_tag_t values */

#define ACL_UNDEFINED_TAG	(0x00)
#define ACL_USER_OBJ		(0x01)
#define ACL_USER		(0x02)
#define ACL_GROUP_OBJ		(0x04)
#define ACL_GROUP		(0x08)
#define ACL_MASK		(0x10)
#define ACL_OTHER		(0x20)

#endif

/*
 * Copied from acl_ea.h in libacl source; ACLs have to be sent to and from fuse
 * in this format... at least on Linux.
 */
#define ACL_EA_ACCESS		"system.posix_acl_access"
#define ACL_EA_DEFAULT		"system.posix_acl_default"

#define ACL_EA_VERSION		0x0002

typedef struct {
	uint16_t	e_tag;
	uint16_t	e_perm;
	uint32_t	e_id;
} acl_ea_entry;

typedef struct {
	int32_t	a_version;
	acl_ea_entry	a_entries[0];
} acl_ea_header;

/*
 * ext4 ACL structures, copied from fs/ext4/acl.h.
 */
#define EXT4_ACL_VERSION	0x0001

typedef struct {
	uint16_t	e_tag;
	uint16_t	e_perm;
	uint32_t	e_id;
} ext4_acl_entry;

typedef struct {
	uint16_t	e_tag;
	uint16_t	e_perm;
} ext4_acl_entry_short;

typedef struct {
	uint32_t	a_version;
} ext4_acl_header;


int fuse_to_ext4_acl(const acl_ea_header *facl, size_t facl_sz,
		     ext4_acl_header **eacl, size_t *eacl_sz);
int ext4_to_fuse_acl(acl_ea_header **facl, size_t *facl_sz,
		     const ext4_acl_header *eacl, size_t eacl_sz);

#endif
