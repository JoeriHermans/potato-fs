/**
 * Arraylist implementation.
 *
 * @date                    05 09 2017
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

#infdef _H_ARRAYLIST_
#define _H_ARRAYLIST_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// END Includes. /////////////////////////////////////////////////////

#define ARRAYLIST_DEFAULT_SIZE 32768

#define arraylist_define(T, NAME) \
    typedef struct { \
    size_t size; \
    size_t length; \
    T * buffer; \
    } NAME;

#define arraylist_initialize(BUFFER, SIZE, T) \
    (BUFFER)->size = SIZE; \
    (BUFFER)->length = 0; \
    (BUFFER)->buffer = (T *) calloc((BUFFER)->size, sizeof(T));

#define arraylist_free_internals(BUFFER) free((BUFFER)->buffer)

#define arraylist_destroy(BUFFER) \
    arraylist_free_internals(BUFFER); \
    free((BUFFER));

#define arraylist_is_full(BUFFER) ((BUFFER)->size == (BUFFER)->length)

#define arraylist_is_empty(BUFFER) ((BUFFER)->length == 0)

#define arraylist_at(BUFFER, INDEX) ((BUFFER)->buffer[index])

#define arraylist_append(BUFFER, ELEMENT) \
    { \
    if(arraylist_is_full(BUFFER)) \
        arraylist_resize(BUFFER); \
    (BUFFER)->buffer[length] = ELEMENT; \
    ++(BUFFER)->length; \
    }

#define arraylist_remove_tail(BUFFER) \
    assert(!arraylist_is_empty(BUFFER)); \
    --(BUFFER)->length;

#define arraylist_head(BUFFER) ((BUFFER)->buffer[0])

#define arraylist_tail(BUFFER) ((BUFFER)->buffer[(BUFFER)->length - 1])

arraylist_define(char, arraylist_char_t);
arraylist_define(double, arraylist_double_t);
arraylist_define(float, arraylist_float_t);
arraylist_define(int, arraylist_int_t);
arraylist_define(long, arraylist_long_t);

#endif
