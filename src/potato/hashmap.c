/**
 * A hashmap implementation based on Pete Warden's C implementation.
 *
 * @date                    25 08 2017
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
#include <potato/hashmap.h>
#include <potato/util.h>

// System dependencies.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// END Includes. /////////////////////////////////////////////////////

bool hashmap_exists(const map_t * map, const char * key) {
    struct hashmap_element * element;
    bool found = false;
    int index;

    index = hashmap_hash(map, key);
    element = &map->bin[index];
    if(strcmp(element->key, key) == 0) {
        found = true;
    } else {
        element = element->next_element;
        while(element) {
            if(strcmp(element->key, key) == 0) {
                found = true;
                break;
            }
        }
    }

    return found;
}

double hashmap_efficiency(const map_t * hashmap) {
    double efficiency;
    size_t num_entries;
    size_t num_bins;

    // TODO Implement.

    return efficiency;
}

int hashmap_get(const map_t * map, const char * key, void ** buffer) {
    struct hashmap_element * element;
    int status;
    int index;

    index = hashmap_hash(map, key);
    element = &map->bin[index];
    if(element->key) {
        if(strcmp(element->key, key) == 0) {
            status = HASHMAP_STATUS_OK;
            *buffer = element->data;
        } else {
            status = HASHMAP_STATUS_NOT_FOUND;
            element = element->next_element;
            while(element) {
                if(strcmp(element->key, key) == 0) {
                    status = HASHMAP_STATUS_OK;
                    *buffer = element->data;
                    break;
                }
            }
        }
    } else {
        status = HASHMAP_STATUS_NOT_FOUND;
        *buffer = NULL;
    }

    return status;
}

int hashmap_hash(const map_t * map, const char * key) {
    uint32_t hash;

    hash = jenkins_one_at_a_time(key);
    //key += (key << 12);
    //key ^= (key >> 22);
    //key += (key << 4);
    //key ^= (key >> 9);
    //key += (key << 10);
    //key ^= (key >> 2);
    //key += (key << 7);
    //key ^= (key >> 12);
    //key = (key >> 3) * 2654435761;

    return hash % map->num_bins;
}

int hashmap_put(map_t * map, const char * key, void * value) {
    struct hashmap_element * element;
    int status;
    int index;

    status = HASHMAP_STATUS_OK;
    index = hashmap_hash(map, key);
    element = &map->bin[index];
    // Check if the current bin is in use.
    if(element->key) {
        // Append the entry to the end of the list.
        while(element->next_element)
            element = element->next_element;
        element->next_element = (struct hashmap_element *) malloc(sizeof(struct hashmap_element));
        // Check if the requested memory could be allocated.
        if(!element->next_element)
            status = HASHMAP_STATUS_MEMORY;
        else
            element = element->next_element;
    }
    if(status == HASHMAP_STATUS_OK) {
        element->key = copy_string(key);
        element->data = value;
        element->next_element = NULL;
        ++map->num_entries;
    }

    return status;
}

int hashmap_remove(map_t * map, const char * key) {
    struct hashmap_element * previous_element;
    struct hashmap_element * element;
    int status;
    int index;

    status = HASHMAP_STATUS_NOT_FOUND;
    index = hashmap_hash(map, key);
    element = &map->bin[index];
    if(element->key) {
        if(strcmp(element->key, key) == 0) {
            // Check if the element has a next one.
            if(element->next_element) {
                memcpy(element->next_element, element, sizeof(struct hashmap_element));
                hashmap_free_element(element->next_element);
            } else {
                free(element->key);
                memset(element, 0, sizeof(struct hashmap_element));
            }
            status = HASHMAP_STATUS_OK;
            --map->num_entries;
        } else {
            previous_element = element;
            element = element->next_element;
            while(element) {
                if(strcmp(element->key, key) == 0) {
                    previous_element->next_element = element->next_element;
                    hashmap_free_element(element);
                    status = HASHMAP_STATUS_OK;
                    --map->num_entries;
                    break;
                }
                previous_element = element;
                element = element->next_element;
            }
        }
    }

    return status;
}

size_t hashmap_size(const map_t * map) {
    return map->num_entries;
}

size_t hashmap_num_free_bins(const map_t * map) {
    size_t num_bins = map->num_bins;
    size_t free_bins = 0;

    for(size_t i = 0; i < num_bins; ++i)
        if(!map->bin[i].key)
            ++free_bins;

    return free_bins;
}

size_t hashmap_num_occupied_bins(const map_t * map) {
    size_t num_bins = map->num_bins;
    size_t occupied_bins;

    for(size_t i = 0; i < num_bins; ++i)
        if(map->bin[i].key)
            ++occupied_bins;

    return occupied_bins;
}

map_t * hashmap_new(void) {
    return hashmap_new_shape(HASHMAP_DEFAULT_SIZE, HASHMAP_DEFAULT_MAX_BIN_ENTRIES);
}

map_t * hashmap_new_shape(const size_t initial_size, const size_t max_bin_entries) {
    map_t * hashmap;

    hashmap = (map_t *) malloc(sizeof(map_t));
    if(hashmap) {
        memset(hashmap, 0, sizeof(map_t));
        hashmap->bin = (struct hashmap_element *) calloc(initial_size, sizeof(struct hashmap_element));
        if(!hashmap->bin) {
            free(hashmap);
            hashmap = NULL;
        } else {
            // Clear the bin entries.
            memset(hashmap->bin, 0, sizeof(struct hashmap_element) * initial_size);
            hashmap->max_bin_entries = max_bin_entries;
            hashmap->num_bins = initial_size;
        }
    }

    return hashmap;
}

uint32_t jenkins_one_at_a_time(const char * key) {
    uint32_t hash = 0;
    const char * input = key;

    while(*input) {
        hash += *input;
        hash += (hash << 10);
        hash ^= (hash >> 6);
        ++input;
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

void hashmap_free(map_t * map) {
    struct hashmap_element * element;
    struct hashmap_element * next_element;

    // Checking the precondition.
    assert(map != NULL);

    // Free the hashmap elements.
    for(size_t i = 0; i < map->num_bins; ++i) {
        // Check if the current element is in use.
        if(map->bin[i].key) {
            free(map->bin[i].key);
            // Check if there are multiple hash-collisions.
            element = map->bin[i].next_element;
            while(element) {
                next_element = element->next_element;
                hashmap_free_element(element);
                element = next_element;
            }
        }
    }
    free(map->bin);
    free(map);
}

void hashmap_free_element(struct hashmap_element * element) {
    free(element->key);
    free(element);
}

void hashmap_increase_max_bin_entries(map_t * map, const size_t max_bin_entries) {
    // Checking the precondition.
    assert(map != NULL && map->max_bin_entries <= max_bin_entries);

    map->max_bin_entries = max_bin_entries;
}

void hashmap_map_keys(map_t * map, void (*f)(map_t *, const char *)) {
    struct hashmap_element * bin = map->bin;
    struct hashmap_element * element;
    size_t num_bins = map->num_bins;

    for(size_t i = 0; i < num_bins; ++i) {
        if(bin[i].key) {
            element = &bin[i];
            while(element) {
                if(element->key)
                    f(map, element->key);
                element = element->next_element;
            }
        }
    }
}
