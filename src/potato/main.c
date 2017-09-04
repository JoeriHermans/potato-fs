/**
 * Main entry point of the filesystem.
 *
 * @date                    25 08 2017
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
#include <potato/config_parser.h>
#include <potato/constants.h>
#include <potato/hashmap.h>
#include <potato/main.h>
#include <potato/potato.h>
#include <potato/ring_buffer.h>
#include <potato/threadpool.h>

// System dependencies.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

int main(const int argc, const char ** argv) {
    int exit_code = EXIT_SUCCESS;

    // Check if PotatoFS is being run as root.
    if((getuid() == 0) || (geteuid() == 0)) {
        syslog(LOG_ERR, k_log_no_root);

        return EXIT_FAILURE;
    }
    // Check if the initial program arguments are valid.
    if(valid_arguments(argc, argv)) {
        // Check if the program is running in Daemon or user mode.
        if(daemon_mode_specified(argc, argv))
            exit_code = run_mode_daemon(argc, argv);
        else
            exit_code = run_mode_user(argc, argv);
    } else {
        usage();
    }

    return exit_code;
}

int run_mode_daemon(const int argc, const char ** argv) {
    char path_configuration[PATH_MAX + 1];
    map_t * properties;
    int rc;

    syslog(LOG_NOTICE, k_log_initializing, FUSE_MAJOR_VERSION, FUSE_MINOR_VERSION);
    fetch_configuration_path(path_configuration, argc, argv);
    syslog(LOG_NOTICE, k_log_open_config, path_configuration);
    properties = hashmap_new();
    rc = config_parser_read(properties, path_configuration);
    if(rc == 0) {
        syslog(LOG_NOTICE, k_log_config_parsed);
    } else {
        syslog(LOG_ERR, k_log_open_config_error, path_configuration);
        syslog(LOG_ERR, k_log_stop_cause_error);
        hashmap_map_keys(properties, &config_clear_properties);
        hashmap_free(properties);

        return EXIT_FAILURE;
    }
    rc = potato_main(argc, argv, properties);
    hashmap_map_keys(properties, &config_clear_properties);
    hashmap_free(properties);

    return rc;
}

int run_mode_user(const int argc, const char ** argv) {
    //puts("Testing");
    //threadpool_t * threadpool = threadpool_new_default();
    //threadpool_task_t task;
    //memset(&task, 0, sizeof(threadpool_task_t));
    //threadpool_enqueue(threadpool, &task);
    //usleep(5000000);
    //puts("Enqueing again.");
    //task.ready = false;
    //threadpool_enqueue(threadpool, &task);
    //usleep(5000000);
    //threadpool_stop(threadpool);
    //puts("Waiting for threads to be done and stuff.");
    //threadpool_join(threadpool);
    //puts("Threads joined.");
    //threadpool_free(threadpool);
    //puts("Done");
    // TODO Implement.

    return EXIT_SUCCESS;
}

bool daemon_mode_specified(const int argc, const char ** argv) {
    bool b = true;

    b &= (has_argument(k_arg_daemon, argc, argv) | has_argument(k_arg_daemon_s, argc, argv));

    return b;
}

bool valid_arguments(const int argc, const char ** argv) {
    bool valid;

    // Check if daemon, or user mode has been sepcified.
    if(daemon_mode_specified(argc, argv)) {
        valid = true;
        valid &= (has_argument(k_arg_config, argc, argv) | has_argument(k_arg_config_s, argc, argv));
    } else {
        // TODO Implement user mode.
        valid = true;
    }

    return valid;
}

void usage(void) {
    puts(k_usage);
}

int get_argument(const char * key, const int argc, const char ** argv) {
    for(int i = 0; i < argc; ++i)
        if(strcmp(key, argv[i]) == 0)
            return i;

    return -1;
}

void fetch_configuration_path(char * buffer, const int argc, const char ** argv) {
    int index;

    // Check if the short flag has been specified.
    index = get_argument(k_arg_config_s, argc, argv);
    if(index < 0)
        // Short flag not found, get index of large argument.
        index = get_argument(k_arg_config, argc, argv);
    // Check if the target index is still within bounds.
    if((index + 1) < argc)
        // Store the path in the buffer.
        strcpy(buffer, argv[index + 1]);
    else
        // Clear the buffer.
        buffer[0] = '\0';
}

bool has_argument(const char * key, const int argc, const char ** argv) {
    return get_argument(key, argc, argv) != -1;
}
