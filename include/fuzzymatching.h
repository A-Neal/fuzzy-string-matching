
#ifndef FUZZYMATCHING_H
#define FUZZYMATCHING_H

// Takes pointers to two strings to be compared, and returns an integer "score"
// representing how similar the two strings are. The score will be normalized
// to a nominal 0-100. 0 is no similarity, and 100+ is high similarity. A Full
// match is never truly determined, and scores greater than 100 can be returned
// from regularly repeating strings. This will be left as such, as it would add
// complexity and CPU time to each comparison, and it matters very little in
// the real world where repeating strings are uncommon. Not to mention that
// return values > 100 are still interpreted as high similarity. Not perfect
// but good enough.
float fuzzycmp(char *str1, char *str2);

// Takes a pointer to a string to search for, the string to search for it in,
// a score threshold to throttle unlikely matches, and returns a pointer to an
// allocated, '\n' delimited string, sorted with the higher scoring matches on
// top.
//
// The haystack string is to be a string of '\n' delimited strings. The needle
// string will be matched against these strings in their entirety. A fuzzy
// match (high enough score) will take the whole line as a match.
//
// This function is case insensitive.
char *fuzzymatch(char *needle, char *haystack, int threshold);

#endif

