#include <stddef.h>
#include <stdio.h>

// Maximum size of comparison
#define CMP_B_SIZE 512

typedef enum {
	NEEDLE,
	HAYSTACK
} n_or_h;

typedef struct {
	char *result;
	int score;
	void *next;
} fuzzylist;

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
	char *dst_offset = &dst[startat];
	//int endat = startat+size-1;
	for(size_t i=0; i<size-1; i++) {
		if(src[i] == delim || src[i] == '\0') {
			dst_offset[i] = '\0';
			break;
		}
		dst_offset[i] = src[i];
	}
	return;
}

static void to_lower(char *str) {
	int len = dstrlen(str, '\n');
	for(int i=0; i<len; i++) {
		if(str[i] >= 'A' && str[i] <= 'Z') str[i] ^= 0b00100000;
	}
	return;
}

int fuzzycmp(char *str1, char *str2) {
	int score = 0;
	int len1 = dstrlen(str1, '\n');
	int len2 = dstrlen(str2, '\n');
	if(len1 == 0 || len2 == 0) return 0;
	char *longer = str1;
	int llen = len1;
	char *shorter = str2;
	int slen = len2;
	if(len1 <= len2) {
		longer = str2;
		shorter = str1;
		llen = len2;
		slen = len1;
	}
	int iter = llen;
	for(int i=0; i<iter; i++) {
		int scoreswitch = 0;
		for(int j=0; j<slen; j++) {
			if(!(shorter[j] ^ longer[(i+j)%llen])) {
				if(scoreswitch > 0) {
					if(scoreswitch == 1) {
						score += 2;
						scoreswitch = 2;
					} else score++;
				} else scoreswitch = 1;
			} else scoreswitch = 0;
		}
	}
	printf("Unweighted score: %d\n", score);			//debug
	score = (float)score * (float)slen / (float)llen / (float)llen * 100;
	return score;
}

char *fuzzymatch(char *needle, char *haystack, int threshold) {
	/*
	int nlen = dstrlen(needle, '\0');
	int hlen = dstrlen(haystack, '\n');
	if(!nlen || !hlen) return NULL;
	int iter = nlen;
	n_or_h bigger = HAYSTACK;
	size_t nbufsize = 0;
	size_t hbufsize = 0;
	char nbuf[CMP_B_SIZE*2] = { '\0' };
	char hbuf[CMP_B_SIZE*2] = { '\0' };
	char *copy_once = hbuf;
	char *copy_once_src = haystack;
	char *copy_more = nbuf;
	char *copy_more_src = needle;
	if(nlen >= hlen) {
		iter = hlen;
		bigger = NEEDLE;
		char *copy_once = nbuf;
		char *copy_once_src = needle;
		char *copy_more = hbuf;
		char *copy_more_src = haystack;
	}
	int copy_times;
	if(bigger == NEEDLE) {
		copy_times = nlen / hlen;
	} else copy_times = hlen / nlen;
	*/

	return NULL;
}

