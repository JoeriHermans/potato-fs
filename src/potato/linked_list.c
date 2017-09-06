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

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/linked_list.h>

// System dependencies.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// END Includes. /////////////////////////////////////////////////////

linked_list_element_t * linked_list_element_at(const linked_list_t * list, const int index) {
    linked_list_element_t * current_element;
    linked_list_element_t * element;
    int current_index = 0;

    // Checking the precondition.
    assert(index < list->length);

    if(index <= (list->length / 2)) {
        current_element = list->head;
        for(current_index = 0; current_index < index; ++current_index) {
            if(current_index == index) {
                element = current_element;
                break;
            }
            current_element = current_element->next;
        }
    } else {
        current_element = list->tail;
        for(current_index = list->length - 1; current_index >= 0; --current_index) {
            if(current_index == index) {
                element = current_element;
                break;
            }
            current_element = current_element->previous;
        }
    }

    return element;
}

linked_list_t * linked_list_create(void) {
    linked_list_t * list;

    list = (linked_list_t *) malloc(sizeof(linked_list_t));
    memset(list, 0, sizeof(linked_list_t));

    return list;
}

void * linked_list_at(const linked_list_t * list, const int index) {
    void * argument;

    argument = linked_list_element_at(list, index)->value;

    return argument;
}

void * linked_list_append(linked_list_t * list, void * argument) {
    linked_list_element_t * element;

    element = (linked_list_element_t *) malloc(sizeof(linked_list_element_t));
    element->value = argument;
    if(list->tail) {
        list->tail->next = element;
        element->previous = list->tail;
        element->next = NULL;
        list->tail = element;
    } else {
        list->tail = element;
        list->head = element;
        element->previous = NULL;
        element->next = NULL;
    }
    ++list->length;

    return element;
}

void linked_list_destroy(linked_list_t * list) {
    linked_list_element_t * element;
    linked_list_element_t * temp;

    // Free elements in the list.
    element = list->head;
    while(element) {
        temp = element->next;
        free(element);
        element = temp;
    }
    // Free allocated list.
    free(list);
}

void linked_list_initialize(linked_list_t * list) {
    memset(list, 0, sizeof(linked_list_t));
}

void linked_list_remove_at(linked_list_t * list, const int index) {
    linked_list_element_t * element;
    int current_index;

    // Checking the precondition.
    assert(index >= 0);

    if((index / 2) < list->length) {
        // Iterate from list head.
        current_index = 0;
        element = list->head;
        while(current_index < index && element) {
            element = element->next;
            ++current_index;
        }
    } else {
        // Iterate from list tail.
        current_index = (int) list->length - 1;
        element = list->tail;
        while(current_index > index && element) {
            element = element->previous;
            --current_index;
        }
    }
    // Check if the current index is valid.
    if(current_index == index && element)
        linked_list_remove_element(list, element);
}

void linked_list_remove_element(linked_list_t * list, linked_list_element_t * element) {
    if(element->previous == NULL) {
        // Element is head.
        list->head = element->next;
        list->head->previous = NULL;
    } else if(element->next == NULL) {
        // Element is tail.
        list->tail = element->previous;
        list->tail->next = NULL;
    } else {
        // Element is in the middle of the list.
        element->previous->next = element->next;
        element->next->previous = element->previous;
    }
    --list->length;
    free(element);
}
