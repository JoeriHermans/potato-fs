/**
 * Implementation of Port Control Protocol (RFC 6887).
 *
 * @date                    04 09 2017
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

#ifndef _H_PCP_
#define _H_PCP_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

#define PCP_MAX_PAYLOAD_SIZE 1100
#define PCP_HEADER_SIZE 192
#define PCP_VERSION 2

struct pcp_response_header {
    uint8_t _reserved;
    uint8_t result_code;
    uint32_t lifetime;
    unsigned char client_address[16];
} __attribute__((packed));

struct pcp_request_header {
    uint16_t _reserved;
    uint32_t lifetime;
    uint32_t epoch_time;
    unsigned char _padding[12];
} __attribute__((packed));

struct pcp_header {
    uint8_t version;
    uint8_t opcode;
    union {
        struct pcp_response_header response_header;
        struct pcp_request_header request_header;
    } header;
} __attribute__((packed));

struct pcp_option {
    uint8_t option_code;
    uint8_t _reserved;
    uint16_t option_length;
} __attribute__((packed));

bool pcp_is_request(const struct pcp_header * header);

bool pcp_is_response(const struct pcp_header * header);

void pcp_clear_opcode(struct pcp_header * header);

void pcp_set_response(struct pcp_header * header);

void pcp_set_request(struct pcp_header * header);

#endif
