/**
 * Dynamic threadpool for queueing and processing tasks.
 *
 * @date                    31 08 2017
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
#include <potato/threadpool.h>
#include <potato/util.h>

// System dependencies.
#include <assert.h>
#include <string.h>
#include <pthread.h>

// END Includes. /////////////////////////////////////////////////////

threadpool_t * threadpool_new(const size_t max_tasks, const size_t num_threads) {
    threadpool_t * threadpool;

    // Checking the precondition.
    assert(max_tasks > 0 && num_threads > 0);

    threadpool = (threadpool_t *) malloc(sizeof(threadpool_t));
    memset(threadpool, 0, sizeof(threadpool_t));
    threadpool->max_tasks = max_tasks;
    threadpool->num_threads = num_threads;
    ring_buffer_initialize(&threadpool->task_buffer, max_tasks, threadpool_task_t);
    pthread_mutex_init(&threadpool->mutex_task_buffer, NULL);

    return threadpool;
}

threadpool_t * threadpool_new_default(void) {
    return threadpool_new(THREADPOOL_DEFAULT_MAX_TASKS, THREADPOOL_DEFAULT_NUM_THREADS);
}

void threadpool_free(threadpool_t * threadpool) {
    ring_buffer_threadpool_task_t * ring_buffer;

    // Checking the precondition.
    assert(threadpool != NULL);

    pthread_mutex_destroy(&threadpool->mutex_task_buffer);
    ring_buffer = &threadpool->task_buffer;
    ring_buffer_destroy(ring_buffer);
    free(threadpool);
}
