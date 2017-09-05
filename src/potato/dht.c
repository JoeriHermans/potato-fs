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

void dht_generate_id(unsigned char * id) {
    // Checking the precondition.
    assert(id != NULL);

    // TODO Implement.
}
