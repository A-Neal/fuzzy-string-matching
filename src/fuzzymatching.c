#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// Size of fuzzymatch() comparison buffers
#define CMP_B_SIZE 1024

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
			break;
		}
		if(src[i] != '\r') dst_offset[i] = src[i];
	}
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
	fuzzylist *results = NULL; //calloc(1, sizeof(fuzzylist));
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
		//printf("fuzzymatch():\n  needle  : %s\n  haystack: %s\n  score   : %d\n",
				//&nbuf[0], &hbuf[0], score);

		/*
		if(!results && score >= threshold) {
			results = malloc(sizeof(fuzzylist));
			results->result = malloc((dstrlen(haystack, '\n')*sizeof(char))+1);
			dstrncat(results->result, haystack, dstrlen(haystack, '\n'), '\n');
			results->score = score;
			results->next = NULL;
			printf("add first node\n");		//debug
		} else if(score >= threshold) {
			fuzzylist *current = results;
			while(current->next && current->next->score > score) {
				current = current->next;
			}
			addnode(current->next, haystack, score);
			printf("addnode\n");			//debug
		}
		*/

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

	fuzzylist *cur = results;
	while(cur) {
		//printf("needle  : %s\nhaystack: %s\nscore   : %d\n\n",
				//&nbuf[0], cur->result, cur->score);
		printf("%s          %d\n", cur->result, cur->score);
		cur = cur->next;
	}

	freelist(results);
	return NULL;
}

