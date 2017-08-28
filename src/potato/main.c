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
#include <potato/main.h>
#include <potato/constants.h>
#include <potato/hashmap.h>
#include <potato/ring_buffer.h>

// System dependencies.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <syslog.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

int main(const int argc, const char ** argv) {
    int exit_code = EXIT_SUCCESS;

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

    syslog(LOG_NOTICE, k_log_initializing);
    fetch_configuration_path(path_configuration, argc, argv);
    syslog(LOG_NOTICE, k_log_open_config, path_configuration);
    if(access(path_configuration, F_OK) == -1) {
        syslog(LOG_ERR, k_log_open_config_error, path_configuration);
        syslog(LOG_ERR, k_log_stop_cause_error);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int run_mode_user(const int argc, const char ** argv) {
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
