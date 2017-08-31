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

#ifndef _H_UTIL_
#define _H_UTIL_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// END Includes. /////////////////////////////////////////////////////

char * copy_string(const char * str);

bool contains_character(const char * str, const size_t str_length, const char c);

bool in_character_set(const char to_check, const char * set, const size_t set_size);

void trim_a(char * str, size_t * str_length);

void trim(char * str);

void string_shift_left(char * str);

int character_index(const char * str, const char c);

bool is_unsigned_integer(const char * str);

#endif
