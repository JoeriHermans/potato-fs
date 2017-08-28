/**
 * Defines the properties and actions of a configuration parser.
 *
 * @date                    28 08 2017
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
#include <potato/ring_buffer.h>

// System dependencies.
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// END Includes. /////////////////////////////////////////////////////

int config_parser_read(map_t * properties, const char * path) {
    ring_buffer_char_t buffer;
    int fd;

    // Checking the precondition.
    assert(path != NULL && strlen(path) > 0);

    // Check if the file exists or has appropriate read permissions.
    if(access(path, F_OK) == -1)
        return CONFIG_PARSER_STATUS_COULDNT_OPEN;
    fd = open(path, O_RDONLY);
    if(fd < 0)
        return CONFIG_PARSER_STATUS_COULDNT_OPEN;
    ring_buffer_initialize(&buffer, RING_BUFFER_DEFAULT_SIZE, char);
    config_parser_process_file(fd, &buffer, properties);
    ring_buffer_free_internals(&buffer);
    close(fd);

    return CONFIG_PARSER_STATUS_OK;
}

void config_parser_process_file(const int fd, ring_buffer_char_t * buffer, map_t * properties) {
    // Checking the precondition.
    assert(fd >= 0 && buffer != NULL && properties != NULL);

    // TODO Implement.
}
