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
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

bool threadpool_running(const threadpool_t * threadpool) {
    return threadpool->running;
}

bool threadpool_wakeup_possible(threadpool_t * threadpool) {
    bool wakeup_possible;

    pthread_mutex_lock(&threadpool->mutex_threads);
    wakeup_possible = threadpool->num_inactive_threads > 0;
    pthread_mutex_unlock(&threadpool->mutex_threads);

    return wakeup_possible;
}

bool threadpool_queue_full(threadpool_t * threadpool) {
    bool full;

    pthread_mutex_lock(&threadpool->mutex_task_buffer);
    full = ring_buffer_is_full(&threadpool->task_buffer);
    pthread_mutex_unlock(&threadpool->mutex_task_buffer);

    return full;
}

bool threadpool_queue_empty(threadpool_t * threadpool) {
    bool empty;

    pthread_mutex_lock(&threadpool->mutex_task_buffer);
    empty = !ring_buffer_is_full(&threadpool->task_buffer);
    pthread_mutex_unlock(&threadpool->mutex_task_buffer);

    return empty;
}

bool threadpool_has_inactive_threads(const threadpool_t * threadpool) {
    return threadpool->num_inactive_threads > 0;
}

int threadpool_enqueue(threadpool_t * threadpool, threadpool_task_t * task) {
    int result;

    // Checking the precondition.
    assert(task->ready == false);

    pthread_mutex_lock(&threadpool->mutex_task_buffer);
    // Check if the task buffer is full.
    if(!ring_buffer_is_full(&threadpool->task_buffer)) {
        ring_buffer_insert(&threadpool->task_buffer, task);
        result = THREADPOOL_STATUS_OK;
    } else {
        result = THREADPOOL_STATUS_QUEUE_FULL;
    }
    pthread_mutex_unlock(&threadpool->mutex_task_buffer);
    // Check if a thread needs to be woken up.
    if(threadpool_wakeup_possible(threadpool))
        threadpool_wakeup(threadpool);

    return result;
}

threadpool_t * threadpool_new(const size_t max_tasks, const size_t num_threads) {
    threadpool_t * threadpool;

    // Checking the precondition.
    assert(max_tasks > 0 && num_threads > 0);

    threadpool = (threadpool_t *) malloc(sizeof(threadpool_t));
    memset(threadpool, 0, sizeof(threadpool_t));
    threadpool->running = true;
    threadpool->max_tasks = max_tasks;
    threadpool->num_threads = num_threads;
    ring_buffer_initialize(&threadpool->task_buffer, max_tasks, threadpool_task_t *);
    pthread_mutex_init(&threadpool->mutex_task_buffer, NULL);
    pthread_mutex_init(&threadpool->mutex_threads, NULL);
    threadpool->threads = (pthread_t *) calloc(num_threads, sizeof(pthread_t));
    threadpool->active_threads = (bool *) calloc(num_threads, sizeof(bool));
    memset(threadpool->active_threads, 0, num_threads * sizeof(bool));
    threadpool->mutex_cond_threads = (pthread_mutex_t *) calloc(num_threads, sizeof(pthread_mutex_t));
    memset(threadpool->mutex_cond_threads, 0, num_threads * sizeof(pthread_mutex_t));
    threadpool->condition_threads = (pthread_cond_t *) calloc(num_threads, sizeof(pthread_cond_t));
    memset(threadpool->condition_threads, 0, num_threads * sizeof(pthread_cond_t));
    threadpool->num_inactive_threads = num_threads;
    for(size_t i = 0; i < num_threads; ++i) {
        struct _threadpool_thread_main_args * args;

        args = (struct _threadpool_thread_main_args *) malloc(sizeof(struct _threadpool_thread_main_args));
        args->threadpool = threadpool;
        args->thread_index = (unsigned int) i;
        pthread_create(&threadpool->threads[i], NULL, &threadpool_thread_main, (void *) args);
    }

    return threadpool;
}

threadpool_t * threadpool_new_default(void) {
    return threadpool_new(THREADPOOL_DEFAULT_MAX_TASKS, THREADPOOL_DEFAULT_NUM_THREADS);
}

threadpool_task_t * threadpool_dequeue(threadpool_t * threadpool) {
    ring_buffer_threadpool_task_t * buffer;
    threadpool_task_t * task;

    buffer = &threadpool->task_buffer;
    pthread_mutex_lock(&threadpool->mutex_task_buffer);
    if(!ring_buffer_is_empty(buffer)) {
        task = ring_buffer_peek_head(buffer);
        ring_buffer_delete_head(buffer);
    } else {
        task = NULL;
    }
    pthread_mutex_unlock(&threadpool->mutex_task_buffer);

    return task;
}

void threadpool_free(threadpool_t * threadpool) {
    ring_buffer_threadpool_task_t * ring_buffer;

    // Checking the precondition.
    assert(threadpool != NULL && threadpool->running == false);

    free(threadpool->mutex_cond_threads);
    free(threadpool->condition_threads);
    pthread_mutex_destroy(&threadpool->mutex_task_buffer);
    pthread_mutex_destroy(&threadpool->mutex_threads);
    ring_buffer = &threadpool->task_buffer;
    ring_buffer_free_internals(ring_buffer);
    free(threadpool->threads);
    free(threadpool->active_threads);
    free(threadpool);
}

void threadpool_stop(threadpool_t * threadpool) {
    threadpool->running = false;
    threadpool_wakeup_all(threadpool);
}

void * threadpool_thread_main(void * argument) {
    struct _threadpool_thread_main_args * args = (struct _threadpool_thread_main_args *) argument;
    threadpool_t * threadpool = args->threadpool;
    unsigned int thread_index = args->thread_index;
    pthread_mutex_t * lock = &threadpool->mutex_cond_threads[thread_index];
    pthread_cond_t * condition = &threadpool->condition_threads[thread_index];
    threadpool_task_t * task;

    // Allocate the thread mutex and condition variable.
    pthread_mutex_init(lock, NULL);
    pthread_cond_init(condition, NULL);
    // Start the thread task.
    threadpool_increase_active_threads(threadpool, thread_index);
    pthread_mutex_lock(lock);
    // Run until the threadpool has to run.
    while(threadpool_running(threadpool)) {
        // Fetch the next task from the task buffer.
        task = threadpool_dequeue(threadpool);
        if(task) {
            // TODO Implement running task.
            printf("Running task from thread %u.\n", thread_index);
            task->ready = true;
        }
        // Check if there are tasks left to be processed.
        if(threadpool_queue_empty(threadpool)) {
            threadpool_decrease_active_threads(threadpool, thread_index);
            pthread_cond_wait(condition, lock);
            threadpool_increase_active_threads(threadpool, thread_index);
        }
    }
    // Edn the thread task.
    pthread_mutex_unlock(lock);
    threadpool_decrease_active_threads(threadpool, thread_index);
    // Free the allocated mutex and condition variable.
    pthread_cond_destroy(condition);
    pthread_mutex_destroy(lock);
    // Free the allocated arguments structure.
    free(args);

    return NULL;
}

void threadpool_thread_wakeup(threadpool_t * threadpool, const unsigned int thread_index) {
    pthread_mutex_t * lock;
    pthread_cond_t * condition;

    lock = &threadpool->mutex_cond_threads[thread_index];
    condition = &threadpool->condition_threads[thread_index];
    pthread_mutex_lock(lock);
    pthread_cond_signal(condition);
    pthread_mutex_unlock(lock);
}

void threadpool_wakeup_all(threadpool_t * threadpool) {
    size_t num_threads;

    num_threads = threadpool->num_threads;
    pthread_mutex_lock(&threadpool->mutex_threads);
    for(size_t i = 0; i < num_threads; ++i)
        threadpool_thread_wakeup(threadpool, (unsigned int) i);
    pthread_mutex_unlock(&threadpool->mutex_threads);
}

void threadpool_wakeup(threadpool_t * threadpool) {
    size_t num_threads;

    num_threads = threadpool->num_threads;
    pthread_mutex_lock(&threadpool->mutex_threads);
    if(threadpool->num_inactive_threads > 0) {
        for(unsigned int i = 0; i < num_threads; ++i) {
            if(!threadpool->active_threads[i]) {
                threadpool_thread_wakeup(threadpool, i);
                break;
            }
        }
    }
    pthread_mutex_unlock(&threadpool->mutex_threads);
}

void threadpool_increase_active_threads(threadpool_t * threadpool, const unsigned int thread_index) {
    pthread_mutex_lock(&threadpool->mutex_threads);
    threadpool->active_threads[thread_index] = true;
    ++threadpool->num_active_threads;
    --threadpool->num_inactive_threads;
    pthread_mutex_unlock(&threadpool->mutex_threads);
}

void threadpool_decrease_active_threads(threadpool_t * threadpool, const unsigned int thread_index) {
    pthread_mutex_lock(&threadpool->mutex_threads);
    threadpool->active_threads[thread_index] = false;
    --threadpool->num_active_threads;
    ++threadpool->num_inactive_threads;
    pthread_mutex_unlock(&threadpool->mutex_threads);
}

void threadpool_join(threadpool_t * threadpool) {
    size_t num_threads;

    // Checking the precondition.
    assert(threadpool->running == false);

    num_threads = threadpool->num_threads;
    // Join all threads.
    for(size_t i = 0; i < num_threads; ++i)
        pthread_join(threadpool->threads[i], NULL);
    // Reset all threadpool statistics.
    threadpool->num_active_threads = 0;
    threadpool->num_inactive_threads = threadpool->num_threads;
    memset(threadpool->active_threads, 0, threadpool->num_threads * sizeof(bool));
}
