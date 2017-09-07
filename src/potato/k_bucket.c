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

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/k_bucket.h>

// System dependencies.
#include <assert.h>
#include <string.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

k_bucket_t * k_bucket_create(const size_t k, const size_t key_length) {
    k_bucket_t * k_bucket = NULL;

    // Checking the precondition.
    assert(k > 0 && key_length > 0);

    // TODO Implement.

    return k_bucket;
}
