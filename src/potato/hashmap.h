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

#ifndef _H_HASHMAP_
#define _H_HASHMAP_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// END Includes. /////////////////////////////////////////////////////

#define HASHMAP_DEFAULT_SIZE 8096
#define HASHMAP_DEFAULT_MAX_BIN_ENTRIES 10
#define HASHMAP_STATUS_EXISTS -1
#define HASHMAP_STATUS_FULL -2
#define HASHMAP_STATUS_MEMORY -3
#define HASHMAP_STATUS_NOT_FOUND -4
#define HASHMAP_STATUS_OK 0

struct hashmap_element{
    void * data;
    char * key;
    struct hashmap_element * next_element;
};

typedef struct _hashmap {
    size_t num_bins;
    size_t num_entries;
    size_t max_bin_entries;
    struct hashmap_element * bin;
} map_t;

bool hashmap_exists(const map_t * map, const char * key);

double hashmap_efficiency(const map_t * map);

int hashmap_get(const map_t * map, const char * key, void ** buffer);

int hashmap_hash(const map_t * map, const char * key);

int hashmap_put(map_t * map, const char * key, void * data);

int hashmap_remove(map_t * map, const char * key);

size_t hashmap_size(const map_t * map);

size_t hashmap_num_free_bins(const map_t * map);

size_t hashmap_num_occupied_bins(const map_t * map);

map_t * hashmap_new(void);

map_t * hashmap_new_shape(const size_t initialize_size, const size_t max_bin_entries);

uint32_t jenkins_one_at_a_time(const char * key);

void hashmap_free(map_t * map);

void hashmap_free_element(struct hashmap_element * element);

void hashmap_increase_max_bin_entries(map_t * map, const size_t max_bin_entries);

void hashmap_map_keys(map_t * map, void (*f)(map_t *, const char *));

#endif
