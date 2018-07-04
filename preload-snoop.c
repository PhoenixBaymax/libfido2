/*
 * public domain
 * cc -fPIC -D_GNU_SOURCE -shared -o preload.so preload-snoop.c
 * export LD_PRELOAD=$(realpath preload.so)
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <dlfcn.h>
#include <fcntl.h>
#include <fido.h>
#include <hidapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int infd = -1;
static int outfd = -1;
static int (*hid_read_f)(hid_device *, unsigned char *, size_t, int);
static int (*hid_write_f)(hid_device *, const unsigned char *, size_t);

int
hid_write(hid_device *dev, const unsigned char *data, size_t len)
{
	int r;

	if (hid_write_f == NULL)
		hid_write_f = dlsym(RTLD_NEXT, "hid_write");
	if (outfd < 0)
		outfd = open("/tmp/hid_out", O_CREAT | O_WRONLY, 0644);

	if (hid_write_f == NULL || outfd < 0) {
		fprintf(stderr, "failed to snoop on hid_write\n");
		exit(1);
	}

	r = hid_write_f(dev, data, len);
	if (r > 0) {
		(void)write(outfd, data, r);
		(void)fsync(outfd);
	}

	return (r);
}

int
hid_read_timeout(hid_device *dev, unsigned char *data, size_t len, int ms)
{
	int r;

	if (hid_read_f == NULL)
		hid_read_f = dlsym(RTLD_NEXT, "hid_read_timeout");
	if (infd < 0)
		infd = open("/tmp/hid_in", O_CREAT | O_WRONLY, 0644);

	if (hid_read_f == NULL || infd < 0) {
		fprintf(stderr, "failed to snoop on hid_read\n");
		exit(1);
	}

	r = hid_read_f(dev, data, len, ms);
	if (r > 0) {
		(void)write(infd, data, r);
		(void)fsync(infd);
	}

	return (r);
}
