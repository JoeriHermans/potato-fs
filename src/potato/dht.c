/**
 * Kademlia-based resilliant distributed hash table.
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
#include <potato/dht.h>
#include <potato/util.h>

// System dependencies.
#include <assert.h>
#include <string.h>
#include <time.h>

// END Includes. /////////////////////////////////////////////////////

bool dht_key_equal(const struct dht_key * a, const struct dht_key * b) {
    for(int i = 0; i < DHT_KEY_SPACE_SIZE_BYTES; ++i)
        if(a->key[i] != b->key[i])
            return false;

    return true;
}

bool dht_key_smaller(const struct dht_key * a, const struct dht_key * b) {
    for(int i = 0; i < DHT_KEY_SPACE_SIZE_BYTES; ++i)
        if(a->key[i] > b->key[i])
            return false;

    return true;
}

bool dht_key_larger(const struct dht_key * a, const struct dht_key * b) {
    for(int i = 0; i < DHT_KEY_SPACE_SIZE_BYTES; ++i)
        if(a->key[i] < b->key[i])
            return false;

    return true;
}

dht_t * dht_create(void) {
    dht_t * dht;

    dht = (dht_t *) malloc(sizeof(dht_t));
    memset(dht, 0, sizeof(dht_t));
    // TODO Implement.

    return dht;
}

void dht_destroy(dht_t * dht) {
    // Checking the precondition.
    assert(dht != NULL);

    free(dht);
}

void dht_generate_id(uint8_t * id) {
    // Checking the precondition.
    assert(id != NULL);

    generate_random_bytes((unsigned char *) id, DHT_KEY_SPACE_SIZE_BYTES);
}

void dht_message_initialize(struct dht_message * message) {
    memset(message, 0, sizeof(struct dht_message));
    message->version = DHT_VERSION;
    generate_random_bytes((unsigned char *) &message->identifier, 2);
}

void dht_contact_set_id(struct dht_contact * contact, const uint8_t * id) {
    memcpy(contact->id.key, id, DHT_KEY_SPACE_SIZE_BYTES);
}
