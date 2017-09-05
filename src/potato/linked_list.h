/**
 * Generic Linked List implementation for pointer types.
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

#ifndef _H_LINKED_LIST_
#define _H_LINKED_LIST_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// END Includes. /////////////////////////////////////////////////////

typedef struct _linked_list_element {
    struct _linked_list_element * previous;
    struct _linked_list_element * next;
    void * value;
} linked_list_element_t;

typedef struct _linked_list {
    size_t length;
    linked_list_element_t * head;
    linked_list_element_t * tail;
} linked_list_t;

linked_list_element_t * linked_list_element_at(const linked_list_t * list, const int index);

linked_list_t * linked_list_create(void);

void * linked_list_at(const linked_list_t * list, const int index);

void * linked_list_append(linked_list_t * list, void * argument);

void * linked_list_insert_at(linked_list_t * list, void * argument, const int index);

void * linked_list_prepend(linked_list_t * list, void * argument);

void linked_list_destroy(linked_list_t * list);

void linked_list_initialize(linked_list_t * list);

void linked_list_remove_at(linked_list_t * list, const int index);

void linked_list_remove_element(linked_list_t * list, linked_list_element_t * element);

#endif
