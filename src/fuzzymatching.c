#include <stddef.h>

// Maximum size of comparison
#define CMP_B_SIZE 512

typedef enum {
	NEEDLE,
	HAYSTACK
} n_or_h;

// Function for measuring the length of a string at a specified delimiter
// rather than a null byte only, but still stops on a null byte
static int dstrlen(char *str, char delim) {
	int c = 0;
	while(str[c] != delim && str[c] != '\0') {
		c++;
	};
	return c;
}

// Function to concatenate a null terminated string with the contents of any
// char array, stopping at delim, null, or size.
static void dstrncat(char *dst, char *src, size_t size, char delim) {
	int startat = dstrlen(dst, '\0');
	int endat = startat+size-1;
	for(size_t i=0; i<size-1; i++) {
		if(src[i] != delim && src[i] != '\0') {
			dst[i+startat] = src[i];
		}
	}
	return;
}

char *fuzzymatch(char *needle, char *haystack, int threshold) {
	int nlen = dstrlen(needle, '\0');
	int hlen = dstrlen(haystack, '\n');
	if(!nlen || !hlen) return NULL;
	int iter = nlen;
	n_or_h bigger = HAYSTACK;
	size_t nbufsize = 0;
	size_t hbufsize = 0;
	char nbuf[CMP_B_SIZE*2] = { '\0' };
	char hbuf[CMP_B_SIZE*2] = { '\0' };
	char *copy_once = &hbuf;
	char *copy_once_src = haystack;
	char *copy_more = &nbuf;
	char *copy_more_src = needle;
	if(nlen >= hlen) {
		iter = hlen;
		bigger = NEEDLE;
		char *copy_once = &nbuf;
		char *copy_once_src = needle;
		char *copy_more = &hbuf;
		char *copy_more_src = haystack;
	}
	int copy_times;
	if(bigger == NEEDLE) {
		copy_times = nlen / hlen;
	} else copy_times = hlen / nlen;


	return NULL;
}

