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

#define PCP_STATUS_SUCCESS 0
#define PCP_STATUS_UNSUPP_VERSION 1
#define PCP_STATUS_NOT_AUTHORIZED 2
#define PCP_STATUS_MALFORMED_REQUEST 3
#define PCP_STATUS_UNSUPP_OPCODE 4
#define PCP_STATUS_UNSUPP_OPTION 5
#define PCP_STATUS_MALFORMED_OPTION 6
#define PCP_STATUS_NETWORK_FAILURE 7
#define PCP_STATUS_NO_RESOURCES 8
#define PCP_STATUS_UNSUPP_PROTOCOL 9
#define PCP_STATUS_USER_EX_QUOTA 10
#define PCP_STATUS_CANNOT_PROVIDE_EXTERNAL 11
#define PCP_STATUS_ADDRESS_MISMATCH 12
#define PCP_STATUS_EXCESSIVE_REMOTE_PEERS 13

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

void pcp_option_set_mandatory(struct pcp_option * options);

void pcp_option_set_optional(struct pcp_option * options);

#endif
