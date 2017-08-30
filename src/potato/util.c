/**
 * A set of utility methods.
 *
 * @date                    28 08 2017
 * @author                  Joeri HERMANS
 * @version                 0.1
 *
 * Copyright 2017 Joeri HERMANS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/constants.h>
#include <potato/util.h>

// END Includes. /////////////////////////////////////////////////////

char * copy_string(const char * str) {
    char * p;

    p = (char *) malloc(strlen(str) + 1);
    if(p) strcpy(p, str);

    return p;
}

bool contains_character(const char * str, const size_t str_length, const char c) {
    for(size_t i = 0; i < str_length; ++i) {
        if(str[i] == c)
            return true;
    }

    return false;
}

bool in_character_set(const char to_check, const char * set, const size_t set_size) {
    for(size_t i = 0; i < set_size; ++i)
        if(to_check == set[i])
            return true;

    return false;
}

void trim_a(char * str, size_t * str_length) {
    // Trim start of the string.
    for(size_t i = 0; i < *str_length; ++i) {
        if(in_character_set(str[i], k_trim_characters, k_trim_num_characters)) {
            string_shift_left(&str[i]);
            --i;
        } else {
            break;
        }
    }
    // Compute the new string length.
    *str_length = strlen(str);
    // Trim end of the string.
    for(size_t i = (*str_length - 1); i >= 0 ; --i) {
        if(in_character_set(str[i], k_trim_characters, k_trim_num_characters))
            string_shift_left(&str[i]);
        else
            break;
    }
    // Compute the final string length after trimming.
    *str_length = strlen(str);
}

void trim(char * str) {
    size_t length;

    length = strlen(str);
    // Trim start of the string.
    for(size_t i = 0; i < length; ++i) {
        if(in_character_set(str[i], k_trim_characters, k_trim_num_characters)) {
            string_shift_left(&str[i]);
            --i;
        } else {
            break;
        }
    }
    // Compute the new string length.
    length = strlen(str);
    // Trim end of the string.
    for(size_t i = (length - 1); i >= 0 ; --i) {
        if(in_character_set(str[i], k_trim_characters, k_trim_num_characters))
            string_shift_left(&str[i]);
        else
            break;
    }
}

void string_shift_left(char * str) {
    while(*str) {
        *str = *(str + 1);
        ++str;
    }
}

int character_index(const char * str, const char c) {
    int index = 0;

    while(str[index]) {
        if(str[index] == c) return index;
        ++index;
    }

    return -1;
}
