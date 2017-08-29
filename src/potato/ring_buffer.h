/**
 * A generic ring buffer.
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

#ifndef _H_RING_BUFFER_
#define _H_RING_BUFFER_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// END Includes. /////////////////////////////////////////////////////

#define RING_BUFFER_DEFAULT_SIZE 32768

#define ring_buffer_define(T, NAME) \
    typedef struct { \
    size_t size; \
    int index_head; \
    int index_tail; \
    T * buffer; \
    } NAME;

#define ring_buffer_initialize(BUFFER, SIZE, T) \
    (BUFFER)->size = SIZE; \
    (BUFFER)->index_head = 0; \
    (BUFFER)->index_tail = 0; \
    (BUFFER)->buffer = (T *) calloc((BUFFER)->size, sizeof(T));

#define ring_buffer_free_internals(BUFFER) free((BUFFER)->buffer)

#define ring_buffer_destroy(BUFFER) \
    ring_buffer_free_internals(BUFFER); \
    free(BUFFER); \
    BUFFER = NULL;

#define ring_buffer_next_head_index(BUFFER) (((BUFFER)->index_head + 1) % ((BUFFER)->size + 1))

#define ring_buffer_next_tail_index(BUFFER) (((BUFFER)->index_tail + 1) % ((BUFFER)->size + 1))

#define ring_buffer_next_index(BUFFER, CURRENT_INDEX) ((CURRENT_INDEX + 1) % ((BUFFER)->size + 1))

#define ring_buffer_is_full(BUFFER) (ring_buffer_next_tail_index(BUFFER) == (BUFFER)->index_head)

#define ring_buffer_is_empty(BUFFER) ((BUFFER)->index_head == (BUFFER)->index_tail)

#define ring_buffer_insert(BUFFER, ELEMENT) \
    { \
    int next_index = ring_buffer_next_tail_index(BUFFER); \
    (BUFFER)->buffer[next_index] = ELEMENT; \
    (BUFFER)->index_tail = next_index; \
    }

#define ring_buffer_delete_head(BUFFER) \
    (BUFFER)->index_head = ring_buffer_next_head_index(BUFFER)

#define ring_buffer_peek_head(BUFFER) (BUFFER)->buffer[BUFFER.index_head]

#define ring_buffer_peek_tail(BUFFER) (BUFFER)->buffer[BUFFER.index_tail]

#define ring_buffer_insert_buffer(BUFFER, ELEMENTS, NUM_ELEMENTS) \
    { \
    int index = (BUFFER)->index_tail; \
    for(size_t i = 0; i < NUM_ELEMENTS; ++i) { \
    (BUFFER)->buffer[index] = ELEMENTS[i]; \
    index = ring_buffer_next_tail_index(BUFFER); \
    (BUFFER)->index_tail = index; \
    } \
    }

#define ring_buffer_read_buffer(BUFFER, ELEMENT_BUFFER, BUFFER_SIZE) \
    { \
    int index = (BUFFER)->index_head; \
    for(size_t i = 0; i < BUFFER_SIZE; ++i) { \
        ELEMENT_BUFFER[i] = (BUFFER)->buffer[index + i]; \
    } \
    (BUFFER)->index_head = ((BUFFER)->index_head + BUFFER_SIZE + 1) % ((BUFFER)->size + 1); \
    }

ring_buffer_define(char, ring_buffer_char_t);
ring_buffer_define(double, ring_buffer_double_t);
ring_buffer_define(float, ring_buffer_float_t);
ring_buffer_define(int, ring_buffer_int_t);
ring_buffer_define(long, ring_buffer_long_t);

int ring_buffer_char_index_of(const ring_buffer_char_t * buffer, const char element);

int ring_buffer_double_index_of(const ring_buffer_double_t * buffer, const double element);

int ring_buffer_float_index_of(const ring_buffer_float_t * buffer, const float element);

int ring_buffer_int_index_of(const ring_buffer_int_t * buffer, const int element);

int ring_buffer_long_index_of(const ring_buffer_long_t * buffer, const long element);

int ring_buffer_dindex_head(const int index_head, const int size, const int index);

#endif
