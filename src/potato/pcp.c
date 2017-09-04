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

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/pcp.h>

// System dependencies.
#include <assert.h>

// END Includes. /////////////////////////////////////////////////////

bool pcp_is_request(const struct pcp_header * header) {
    return !pcp_is_response(header);
}

bool pcp_is_response(const struct pcp_header * header) {
    return ((header->opcode >> 8) & 1);
}

void pcp_clear_opcode(struct pcp_header * header) {
    header->opcode &= 0x80;
}

void pcp_set_response(struct pcp_header * header) {
    header->opcode |= 1 << 8;
}

void pcp_set_request(struct pcp_header * header) {
    header->opcode |= 0 << 8;
}
