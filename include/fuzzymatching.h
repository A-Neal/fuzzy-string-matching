
#ifndef FUZZYMATCHING_H
#define FUZZYMATCHING_H

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

