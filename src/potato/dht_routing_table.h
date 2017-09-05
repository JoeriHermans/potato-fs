/**
 * Routing table specifically designed for a distributed hash table
 * implementing the Kameldia scheme.
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

#findef _H_DHT_ROUTING_TABLE_
#define _H_DHT_ROUTING_TABLE_

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/dht.h>

// System dependencies.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// END Includes. /////////////////////////////////////////////////////

#define DHT_ROUTING_TABLE_MAX_ENTRIES 8

struct dht_routing_table {
    size_t allocated_entries;
    size_t entries;
    struct dht_routing_table_entry * entries;
};

struct dht_routing_table_entry {
    uint8_t mask[DHT_KEY_SPACE_SIZE_BYTES];
    uint8_t destination[DHT_KEY_SPACE_SIZE_BYTES];
    uint8_t node_id[DHT_KEY_SPACE_SIZE_BYTES];
};

#endif
