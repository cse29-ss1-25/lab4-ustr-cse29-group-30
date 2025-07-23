#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "list.h"

/*
Returns an empty list of initial_capacity
*/
List new_list(int32_t initial_capacity) {
    List list;
    list.size = 0;
    list.capacity = initial_capacity;
    list.data = malloc(initial_capacity * sizeof(struct UStr));
    return list;
}

/*
Initializes an list of length size with elements from array
*/
List new_list_from_array(struct UStr* array, int32_t size) {
    List list = new_list(size);
    for (int32_t i = 0; i < size; i++) {
        list.data[i] = array[i];
    }
    list.size = size;
    return list;
}


/*
Given a list of strings and a separator string, returns a single string 
containing all the strings in list joined by the separator.
*/
UStr join(List* list, UStr separator) {
    if (list->size == 0) {
        return new_ustr("");
    }
    
    if (list->size == 1) {
        return new_ustr(list->data[0].contents);
    }

    int32_t total_length = 0;
    for (int32_t i = 0; i < list->size; i++) {
        total_length += list->data[i].bytes;
        if (i < list->size - 1) {
            total_length += separator.bytes;
        }
    }
    
    char* result = malloc(total_length + 1);
    if (result == NULL) {
        return new_ustr("");
    }
    
    strcpy(result, list->data[0].contents);
    
    int32_t pos = list->data[0].bytes;
    for (int32_t i = 1; i < list->size; i++) {
        strcpy(result + pos, separator.contents);
        pos += separator.bytes;
        strcpy(result + pos, list->data[i].contents);
        pos += list->data[i].bytes;
    }
    
    UStr joined = new_ustr(result);
    free(result);
    return joined;
}

/*
Inserts string s into list at index s, shifting elements to the right.
Expands the list's capacity if necessary (double the capacity, or set to 1 if 0).

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t insert(List* list, UStr s, int32_t index) {
    if (index < 0 || index > list->size) {
        return 0;
    }
    
    if (list->size >= list->capacity) {
        int32_t new_capacity = list->capacity * 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        
        UStr* new_data = realloc(list->data, new_capacity * sizeof(UStr));
        if (new_data == NULL) {
            return 0;
        }
        
        list->data = new_data;
        list->capacity = new_capacity;
    }
    
    for (int32_t i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    
    list->data[index] = s;
    list->size++;
    
    return 1;
}

/*
Removes the element at the given index and shifts all subsequent elements left.

Returns 1 on success, 0 if the index is invalid (out of bounds).
*/
int8_t listRemoveAt(List* list, int32_t index) {
    if (index < 0 || index >= list->size) {
        return 0;
    }

    free_ustr(list->data[index]);

    for (int32_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    // Clear the last element
    list->data[list->size - 1].contents = NULL;
    list->data[list->size - 1].bytes = 0;

    list->size--;
    
    return 1; 
}

/*
Splits the given string s into substrings separated by the given delimiter string.

Returns a List of String objects, each containing a segment between delimiters.

If the delimiter is the empty string, return a list containing the original 
string as a single element.

If the input string ends with the delimiter, include an empty string at the 
end of the result.

Note that the delimiter could be of a length of more than 1 character
*/
List split(UStr s, UStr separator) {
    List result = new_list(1);
    
    if (separator.bytes == 0) {
        UStr copy = new_ustr(s.contents);
        insert(&result, copy, 0);
        return result;
    }
    
    int32_t start = 0;
    int32_t end = 0;
    
    while (end <= s.bytes - separator.bytes) {
        int found = 1;
        for (int32_t i = 0; i < separator.bytes; i++) {
            if (s.contents[end + i] != separator.contents[i]) {
                found = 0;
                break;
            }
        }
        
        if (found) {
            // Extract the substring from start to end
            int32_t length = end - start;
            char* substring = malloc(length + 1);
            if (substring == NULL) {
                return result;
            }
            
            strncpy(substring, s.contents + start, length);
            substring[length] = '\0';
            
            UStr substr = new_ustr(substring);
            insert(&result, substr, result.size);
            free(substring);
            
            start = end + separator.bytes;
            end = start;
        } else {
            end++;
        }
    }
    
    if (start <= s.bytes) {
        int32_t length = s.bytes - start;
        char* substring = malloc(length + 1);
        if (substring == NULL) {
            return result;
        }
        
        strncpy(substring, s.contents + start, length);
        substring[length] = '\0';
        
        UStr substr = new_ustr(substring);
        insert(&result, substr, result.size);
        free(substring);
    }
    
    return result;
}