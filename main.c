#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static const char *fpath = "/test";
static const char *fcont = "This is a test file by Shravan A Y\n";

static int fs_getattr(const char *path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		return 0;
	} else if (strcmp(path, fpath) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(fcont);
		return 0;
	}
	return -ENOENT;
}

static int fs_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, fpath) != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int fs_read(const char *path, char *buf, size_t size, off_t offset,
		   struct fuse_file_info *fi)
{
	(void)fi;

	if (strcmp(path, fpath) != 0)
		return -ENOENT;

	size_t len = strlen(fcont);
	if ((size_t)offset >= len)
		return 0;

	if (offset + size > len)
		size = len - offset;

	memcpy(buf, fcont + offset, size);
	return size;
}

static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		      off_t offset, struct fuse_file_info *fi)
{
	(void)offset;
	(void)fi;

	if (strcmp(path, "/") != 0)
		return -ENOENT;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);

	filler(buf, "test", NULL, 0);
	return 0;
}

static struct fuse_operations fs_oper = {
	.getattr = fs_getattr,
	.open = fs_open,
	.read = fs_read,
	.readdir = fs_readdir,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &fs_oper, NULL);
}
