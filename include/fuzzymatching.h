
#ifndef FUZZYMATCHING_H
#define FUZZYMATCHING_H

// Takes pointers to two strings to be compared, and returns an integer "score"
// representing how similar the two strings are. The score will be normalized
// to 0-100. 0 is no similarity, and 100 is high similarity. A score of 100
// can only be returned if the lengths of the input strings are equal, AND the
// score is equal to that length. It isn't guaranteed that a score of 100
// constitutes an exact match, but it should be the case most of the time.
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
//
// The clamp paramter is a later addition. It is the number of results to
// output before stopping. Set to 0 to disable. Hard limit at 1000.
char *fuzzymatch(char *needle, char *haystack, int threshold, int clamp);

#endif

