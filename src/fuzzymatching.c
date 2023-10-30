#include "fuzzymatching.h"

// Function for measuring the length of a string at a specified delimiter
// rather than a null byte only, but still stops on a null byte
static int dstrlen(char *str, char delim) {
	int c = 0;
	while(str[c] != delim && str[c] != '\0') {
		c++;
	};
	return c;
}

char *fuzzymatch(char *needle, char *haystack, int threshold) {
	//char nbuf[256] = { '\0' };
	//char hbuf[256] = { '\0' };
	int nlen = dstrlen(needle, '\0');

	return NULL;
}

