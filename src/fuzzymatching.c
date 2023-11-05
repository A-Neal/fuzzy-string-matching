#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// Size of fuzzymatch() comparison buffers
#define CMP_B_SIZE 1024

// Starting size of fuzzymatch() return string
#define OUTSIZE 32

typedef enum {
	NEEDLE,
	HAYSTACK
} n_or_h;

typedef struct fuzzylist {
	char *result;
	int score;
	struct fuzzylist *next;
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

// Function to clear a null terminated string
static void delstring(char *str) {
	int c = dstrlen(str, '\0');
	for(int i = 0; i < c; i++) {
		str[i] = '\0';
	}
	return;
}

// Function to concatenate a null terminated string with the contents of any
// char array, stopping at delim, null, or size.
// Ignores '\r' carriage return characters
static void dstrncat(char *dst, char *src, size_t size, char delim) {
	int startat = dstrlen(dst, '\0');
	char *dst_offset = &dst[startat];
	//int endat = startat+size-1;
	for(size_t i=0; i<size-1; i++) {
		if(src[i] == delim || src[i] == '\0') {
			dst_offset[i] = '\0';
			return;
		}
		if(src[i] == '\r') {
			dst_offset -= 1;
			continue;
		}
		dst_offset[i] = src[i];
	}
	dst_offset[size] = '\0';
	return;
}

// Adds a fuzzylist node at the given pointer
static void addnode(fuzzylist **dst_ptr, char *result, int score) {
	int rlen = dstrlen(result, '\n');
	fuzzylist *out = calloc(1, sizeof(fuzzylist));
	out->next = NULL;
	out->score = score;
	out->result = calloc(rlen+1, sizeof(char));
	dstrncat(out->result, result, rlen+1, '\n');
	
	if(*dst_ptr) out->next = *dst_ptr;
	*dst_ptr = out;

	return;
}

// Function to traverse and free everything in a linked fuzzylist
static void freelist(fuzzylist *list) {
	fuzzylist *current = list;
	fuzzylist *previous = NULL;
	while(1) {
		if(!current) break;
		free(current->result);
		previous = current;
		current = current->next;
		free(previous);
	}
	return;
}

// Function to turn all capital letters in a string lowercase, stopping at
// '\n' in addition to '\0'
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
	int iter = llen-slen+1;
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
	//printf("fuzzycmp():\n  unweighted score: %d\n  slen: %d\n  llen: %d\n",
			//score, slen, llen);
	score = (float)score * (float)slen / (float)llen / (float)llen * 100;
	return score;
}

char *fuzzymatch(char *needle, char *haystack, int threshold) {
	fuzzylist *results = NULL;
	int score = 0;
	char nbuf[CMP_B_SIZE] = {'\0'};
	char hbuf[CMP_B_SIZE] = {'\0'};
	dstrncat(nbuf, needle, CMP_B_SIZE-1, '\n');
	to_lower(nbuf);

	while(1) {
		delstring(hbuf);
		dstrncat(hbuf, haystack, CMP_B_SIZE-1, '\n');
		to_lower(hbuf);
		score = fuzzycmp(nbuf, hbuf);

		if(score >= threshold) {
			fuzzylist **current = &results;
			fuzzylist **node_ptr = NULL;
			while(1) {
				if(!*current) {
					node_ptr = current;
					break;
				}
				if((*current)->score < score) {
					node_ptr = &(*current);
					break;
				}
				if(!(*current)->next) {
					node_ptr = &(*current)->next;
					break;
				}
				current = &(*current)->next;
			}
			addnode(node_ptr, haystack, score);
		}

		if(haystack[dstrlen(haystack, '\n')] == '\0') break;
		haystack = &haystack[dstrlen(haystack, '\n') + 1];
	}

	if(!results) return NULL;

	size_t outsize = OUTSIZE;
	size_t outlen = 0;
	int rlen;
	char *out = calloc(outsize, sizeof(char));
	char *errck;
	fuzzylist *cur = results;
	while(cur) {
		//printf("%s          %d\n", cur->result, cur->score);	//debug

		rlen = dstrlen(cur->result, '\0');
		if(outsize <= outlen+rlen+2) {
			do {
				outsize += OUTSIZE;
			} while(outsize <= outlen+rlen+2);
			errck = realloc(out, outsize);
			if(!errck) {
				fprintf(stderr, "realloc() failed!\n");
				exit(1);
			}
			out = errck;
			// Initialize realloc()'d memory
			for(int i = outlen; i<outsize; i++) {
				out[i] = '\0';
			}
		}
		dstrncat(out, cur->result, rlen+1, '\0');
		//out[outlen+rlen] = '\n';
		dstrncat(out, "\n", 2, '\0');
		outlen += rlen+2;

		cur = cur->next;
	}

	freelist(results);
	return out;
}

