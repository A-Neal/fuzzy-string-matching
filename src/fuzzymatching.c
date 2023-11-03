#include <stddef.h>
#include <stdio.h>

// Size of fuzzymatch() comparison buffers
#define CMP_B_SIZE 1024

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
	score = (float)score * (float)slen / (float)llen / (float)llen * 100;
	return score;
}

char *fuzzymatch(char *needle, char *haystack, int threshold) {
	int score = 0;
	char nbuf[CMP_B_SIZE] = {'\0'};
	char hbuf[CMP_B_SIZE] = {'\0'};
	dstrncat(nbuf, needle, CMP_B_SIZE-1, '\n');
	to_lower(nbuf);

	int counter = 0;
	while(1) {
		dstrncat(hbuf, haystack, CMP_B_SIZE-1, '\n');
		to_lower(hbuf);
		score = fuzzycmp(nbuf, hbuf);
		printf("fuzzymatch():\n  needle: %s\n  haystack: %s\n  score: %d\n",
				&nbuf[0], &hbuf[0], score);
		if(haystack[dstrlen(haystack, '\n')] == '\0') break;
		haystack = &haystack[dstrlen(haystack, '\n') + 1];
		if(counter++ >= 100) break;
	}

	return NULL;
}

