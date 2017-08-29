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

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/ring_buffer.h>

// END Includes. /////////////////////////////////////////////////////

int ring_buffer_char_index_of(const ring_buffer_char_t * buffer, const char element) {
    int index = buffer->index_head;
    int tail_index = buffer->index_tail;
    while(index != tail_index) {
        if(buffer->buffer[index] == element)
            return index;
        index = ring_buffer_next_index(buffer, index);
    }

    return -1;
}

int ring_buffer_double_index_of(const ring_buffer_double_t * buffer, const double element) {
    int index = buffer->index_head;
    int tail_index = buffer->index_tail;
    while(index != tail_index) {
        if(buffer->buffer[index] == element)
            return index;
        index = ring_buffer_next_index(buffer, index);
    }

    return -1;
}

int ring_buffer_float_index_of(const ring_buffer_float_t * buffer, const float element) {
    int index = buffer->index_head;
    int tail_index = buffer->index_tail;
    while(index != tail_index) {
        if(buffer->buffer[index] == element)
            return index;
        index = ring_buffer_next_index(buffer, index);
    }

    return -1;
}

int ring_buffer_int_index_of(const ring_buffer_int_t * buffer, const int element) {
    int index = buffer->index_head;
    int tail_index = buffer->index_tail;
    while(index != tail_index) {
        if(buffer->buffer[index] == element)
            return index;
        index = ring_buffer_next_index(buffer, index);
    }

    return -1;
}

int ring_buffer_long_index_of(const ring_buffer_long_t * buffer, const long element) {
    int index = buffer->index_head;
    int tail_index = buffer->index_tail;
    while(index != tail_index) {
        if(buffer->buffer[index] == element)
            return index;
        index = ring_buffer_next_index(buffer, index);
    }

    return -1;
}

int ring_buffer_dindex_head(const int index_head, const int size, const int index) {
    int dindex;

    if(index_head < size && index >= index_head)
        dindex = index - index_head;
    else
        dindex = (size - index_head) + index;

    return dindex;
}
