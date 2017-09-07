/**
 * k-bucket implementation according to the Kademlia paper.
 *
 * @date                    07 09 2017
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

#ifndef _H_K_BUCKET_
#define _H_K_BUCKET_

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/dht.h>
#include <potato/ring_buffer.h>

// System dependencies.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// END Includes. /////////////////////////////////////////////////////

// Define the ring buffer with the DHT Contact pointer type.
ring_buffer_define(struct dht_contact *, ring_buffer_dht_contact_t);

typedef struct _k_bucket_bucket {
    size_t num_entries;
    ring_buffer_dht_contact_t * bucket;
} k_bucket_bucket_t;

typedef struct _k_bucket {
    size_t k;
    size_t key_length;
    k_bucket_bucket_t * buckets;
} k_bucket_t;

k_bucket_t * k_bucket_create(const size_t k, const size_t key_length);

void k_bucket_destroy(k_bucket_t * k_bucket);

#endif
