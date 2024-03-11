#include <stdlib.h>
#include <stdio.h>

#include "fuzzymatching.h"

#define BUFSIZE 1024

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("You must enter a search term.\n");
		return 1;
	}
	size_t buflen = BUFSIZE;
	char *buf = calloc(buflen, sizeof(char*));
	FILE *words = fopen("demo_song_list.txt", "rb");
	if(!words) {
		fprintf(stderr, "Could not open file\n");
		return 1;
	}
	char ch;
	int count;
	void *errck;
	while((ch = fgetc(words)) != EOF) {
		if(count >= buflen-1) {
			buflen += BUFSIZE;
			errck = realloc(buf, buflen);
			if(!errck) {
				fprintf(stderr, "realloc() failed!\n");
				return 1;
			}
			buf = errck;
		}
		buf[count] = ch;
		count++;
	}
	fclose(words);

	printf("%d bytes loaded from file\n", count);

// ---- Testing fuzzymatching.h functions -------------------------------------
	// Testing fuzzymatch()
	char *results = fuzzymatch(argv[1], buf, 60, 50);

	if(results) {
		printf("RESULTS:\n%s\n", results);
		free(results);
	}

	free(buf);
}

