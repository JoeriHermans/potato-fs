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

#ifndef _H_THREADPOOL_
#define _H_THREADPOOL_

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/ring_buffer.h>

// System dependencies.
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

#define THREADPOOL_DEFAULT_MAX_TASKS 32768
#define THREADPOOL_DEFAULT_NUM_THREADS 10
#define THREADPOOL_STATUS_OK 0
#define THREADPOOL_STATUS_QUEUE_FULL -1

typedef struct _threadpool_task {
    void * argument;
    void * (* method)(void *);
    void * result;
    bool ready;
} threadpool_task_t;

// Ringbuffer definitions for our threadpool.
ring_buffer_define(threadpool_task_t *, ring_buffer_threadpool_task_t);

typedef struct _threadpool {
    bool running;
    bool * active_threads;
    pthread_mutex_t mutex_task_buffer;
    pthread_mutex_t mutex_threads;
    pthread_mutex_t * mutex_cond_threads;
    pthread_cond_t * condition_threads;
    pthread_t * threads;
    ring_buffer_threadpool_task_t task_buffer;
    size_t max_tasks;
    size_t num_active_threads;
    size_t num_inactive_threads;
    size_t num_threads;
} threadpool_t;

struct _threadpool_thread_main_args {
    threadpool_t * threadpool;
    unsigned int thread_index;
};

bool threadpool_running(const threadpool_t * threadpool);

bool threadpool_wakeup_possible(threadpool_t * threadpool);

bool threadpool_queue_full(threadpool_t * threadpool);

bool threadpool_queue_empty(threadpool_t * threadpool);

bool threadpool_has_inactive_threads(const threadpool_t * threadpool);

bool threadpool_task_ready(const threadpool_task_t * task);

int threadpool_enqueue(threadpool_t * threadpool, threadpool_task_t * task);

threadpool_t * threadpool_new(const size_t max_tasks, const size_t num_threads);

threadpool_t * threadpool_new_default(void);

threadpool_task_t * threadpool_dequeue(threadpool_t * threadpool);

void threadpool_free(threadpool_t * threadpool);

void threadpool_stop(threadpool_t * threadpool);

void * threadpool_thread_main(void * argument);

void threadpool_thread_wakeup(threadpool_t * threadpool, const unsigned int thread_index);

void threadpool_wakeup(threadpool_t * threadpool);

void threadpool_wakeup_all(threadpool_t * threadpool);

void threadpool_increase_active_threads(threadpool_t * threadpool, const unsigned int thread_index);

void threadpool_decrease_active_threads(threadpool_t * threadpool, const unsigned int thread_index);

void threadpool_join(threadpool_t * threadpool);

#endif
