// SPDX-License-Identifier: GPL-2.0

#include <stdio.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef __u64 u64;

#include "famfs_ioctl.h"

void
print_usage(int   argc,
	    char *argv[])
{
	printf("usage\n");
}

int verbose_flag;

struct option global_options[] = {
	/* These options set a flag. */
	{"filename",    required_argument,             0,  'f'},
	/* These options don't set a flag.
	 * We distinguish them by their indices.
	 */
	/*{"dryrun",       no_argument,       0, 'n'}, */
	{0, 0, 0, 0}
};

int
main(int argc, char *argv[])
{
	int c, rc, fd;
	char *filename = NULL;

	int arg_ct = 0;
	struct stat statbuf;
	char *buf;

	/* Process global options, if any */
	/* Note: the "+" at the beginning of the arg string tells getopt_long
	 * to return -1 when it sees something that is not recognized option
	 * (e.g. the command that will mux us off to the command handlers
	 */
	while ((c = getopt_long(argc, argv, "+f:h?",
				global_options, &optind)) != EOF) {
		/* printf("optind:argv = %d:%s\n", optind, argv[optind]); */

		/* Detect the end of the options. */
		if (c == -1)
			break;

		arg_ct++;
		switch (c) {
		case 'f': {
			filename = optarg;
			printf("filename: %s\n", filename);
			/* TODO: make sure filename is in a famfs file system */
			break;
		}
		case 'h':
		case '?':
			print_usage(argc, argv);
			return 0;

		default:
			printf("default (%c)\n", c);
			return -1;
		}
	}

	if (filename == NULL) {
		fprintf(stderr, "Must supply filename\n");
		exit(-1);
	}
	rc = stat(filename, &statbuf);
	if (rc) {
		fprintf(stderr, "unable to stat file %s\n", filename);
		exit(-1);
	}

	printf("file (%s) size %ld\n", filename, statbuf.st_size);

	fd = open(filename, O_RDWR, 0);
	if (fd < 0) {
		fprintf(stderr, "open failed; rc %d errno %d\n", rc, errno);
		exit(-1);
	}

	buf = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (buf == MAP_FAILED)
		fprintf(stderr, "mmap failed\n");

	sleep(2);
	strcpy(buf, "Hello, world\n");

	printf("buf contents: %s\n", buf);

	close(rc);
}
