#ifndef VFS_H
#define VFS_H

#include <klibc/vec.h>
#include <locks/spinlock.h>
#include <sched/syscall.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <types.h>

struct fs;
struct fs_node;
struct file;

typedef vec_t(struct fs_node *) fs_node_vec_t;
typedef vec_t(struct fs *) fs_vec_t;
typedef vec_t(struct file *) file_vec_t;

struct fs_node {
	char *name;
	char *target;
	struct fs *fs;
	file_vec_t files;
	struct fs_node *parent;
	fs_node_vec_t nodes;
};

struct fs {
	char *name;
	struct file *(*open)(struct fs_node *, char *);
	uint32_t (*create)(struct fs_node *, char *);
	uint32_t (*delete)(struct fs_node *, char *);
	uint32_t (*mkdir)(struct fs_node *, char *);
};

struct file {
	char *name;
	lock_t lock;
	struct stat fstat;
	size_t allocated_size;
	ssize_t (*read)(struct file *, void *, off_t, size_t);
	ssize_t (*write)(struct file *, const void *, off_t, size_t);
	struct fs_node *(*readdir)(struct file *);
	struct stat *(*stat)(struct file *);
	uint8_t *data;
};

void vfs_install_fs(struct fs *fs);
struct fs_node *vfs_node_create(struct fs_node *parent, char *name);
bool vfs_node_mount(struct fs_node *node, char *target, char *fs);
void vfs_dump_fs_tree(struct fs_node *node);
struct fs_node *vfs_path_to_node(char *patha);
struct file *vfs_find_file_in_node(struct fs_node *node, char *path,
								   uint8_t fdn);

#define vfs_open_file(A, B) vfs_find_file_in_node(vfs_path_to_node(A), A, B);

void syscall_open(struct syscall_arguments *args);
void syscall_read(struct syscall_arguments *args);
void syscall_write(struct syscall_arguments *args);

#endif
