#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/
UStr substring(UStr s, int32_t start, int32_t end) {
	// TODO: implement this

}

/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
	// TODO: implement this

}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
    //returns original string out of bounds
    if (index < 0 || index >= s.codepoints || !s.is_ascii) {
        return s;
    }
    //make space for new string
    char* new_str = malloc(s.bytes);
    if (new_str == NULL) {
        return s;
    }
    //copies all chars except for the one at index
    for (int i = 0, j = 0; i < s.bytes; i++) {
        if (i != index) {
            new_str[j++] = s.contents[i];
        }
    }

    new_str[s.bytes - 1] = '\0';

    UStr result = new_ustr(new_str);
    free(new_str);
    return result;
}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/
UStr reverse(UStr s) {
    //nothing to reverse
    if (s.codepoints <= 1) return s;

    char* reversed = malloc(s.bytes);
    if (!reversed) return s;
    //copy bytes in reverse
    for (int i = 0; i < s.bytes - 1; i++) {
        reversed[i] = s.contents[s.bytes - 2 - i];
    }
    //null terminates
    reversed[s.bytes - 1] = '\0';

    //creates new ustr
    UStr reversed_str = new_ustr(reversed);
    free(reversed);
    return reversed_str;
}


void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

