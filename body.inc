/*
 * This file is intended to be included.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <time.h>

#include "config.h"

int
main(int argc, char *argv[])
{
	int fd;
	struct stat st;
	unsigned char *src, *dst;
	size_t srclen, dstlen;
	clock_t cl;
	double diff;
	int times = CONFIG_TIMES;
	int i;
	
	if (argc < 2) err(1, "usage: %s filename", argv[0]);
	
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) err(fd, "open %s", argv[1]);
	if (fstat(fd, &st) != 0) err(2, "fstat");
	srclen = st.st_size;
	dstlen = (srclen * 1.001) + 12; // so saith zlib
	src = mmap(NULL, srclen, PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (src == MAP_FAILED) err(3, "mmap");
	dst = malloc(dstlen);
	if (dst == NULL) err(4, "malloc");
	
	cl = clock();
	
	for (i = 0; i < times; i++) {
		// Compress.
		if (compress2(dst, &dstlen, src, srclen, CONFIG_LEVEL) != Z_OK)
			err(5, "compress2");
	}
	
	diff = (clock() - cl)/(double)CLOCKS_PER_SEC;
	printf("%s - %s\n", COMP_NAME, argv[1]);
	printf("source size    : %lu\n", srclen);
	printf("compressed size: %lu\n", dstlen);
	printf("%.3f sec, %.3f MB/s\n\n", diff, (srclen*times)/1024.0/1024.0/diff);
	exit(0);
}

