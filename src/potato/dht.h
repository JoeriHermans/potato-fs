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

#ifndef _H_DHT_
#define _H_DHT_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// END Includes. /////////////////////////////////////////////////////

#define DHT_KEY_SPACE_SIZE 256
#define DHT_KEY_SPACE_SIZE_BYTES 32
#define DHT_INTERNET_ADDRESS_SIZE 16
#define DHT_HASH_ITERATIONS 100
#define DHT_VERSION 1

#define DHT_OPCODE_NOOP 0
#define DHT_OPCODE_DELETE 1
#define DHT_OPCODE_GET 2
#define DHT_OPCODE_PING 3
#define DHT_OPCODE_PONG 4
#define DHT_OPCODE_PUT 5

typedef struct dht {
    // TODO Implement.
} dht_t;

struct dht_contact {
    uint8_t id[DHT_KEY_SPACE_SIZE_BYTES];
    uint8_t address[DHT_INTERNET_ADDRESS_SIZE];
    uint16_t port;
} __attribute__((packed));

struct dht_message {
    uint8_t version;
    uint8_t opcode;
    uint16_t identifier;
    union {
        struct dht_contact contact;
    } payload;
} __attribute__((packed));

dht_t * dht_create(void);

void dht_destroy(dht_t * dht);

void dht_generate_id(unsigned char * id);

void dht_message_initialize(struct dht_message * message);

void dht_contact_set_id(struct dht_contact * contact, const uint8_t * id);

#endif
