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
#include <potato/constants.h>
#include <potato/config_parser.h>
#include <potato/ring_buffer.h>

// System dependencies.
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// END Includes. /////////////////////////////////////////////////////

bool config_parser_is_blank_line(const char * line_buffer) {
    // Checking the precondition.
    assert(line_buffer != NULL && strlen(line_buffer) > 0);

    // TODO Implement.

    return false;
}

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

void config_parser_process_file(const int fd, ring_buffer_char_t * ring_buffer, map_t * properties) {
    char line_buffer[BUFSIZ + 1];
    bool has_lines = true;

    // Checking the precondition.
    assert(fd >= 0 && ring_buffer != NULL && properties != NULL);

    while(has_lines) {
        int rc = config_parser_read_line(fd, ring_buffer, line_buffer, BUFSIZ);
        if(rc != CONFIG_PARSER_STATUS_OK) {
            has_lines = false;
            continue;
        }
        config_parser_filter_line(line_buffer);
        if(strlen(line_buffer) > 0) {
            // TODO Implement.
        }
    }
}

int config_parser_read_line(const int fd, ring_buffer_char_t * ring_buffer,
                            char * line_buffer, const size_t line_buffer_size) {
    bool has_line = false;
    char buffer[BUFSIZ + 1];
    int rc = 0;

    while(!has_line) {
        // Check if the buffer contains a line.
        int index = ring_buffer_char_index_of(ring_buffer, k_newline);
        if(index >= 0) {
            // Line is present in the ring buffer.
            int size = ring_buffer_dindex_head(ring_buffer->index_head, ring_buffer->size, index);
            if(size >= line_buffer_size)
                return CONFIG_PARSER_BUFFER_OVERFLOW;
            ring_buffer_read_buffer(ring_buffer, line_buffer, size);
            line_buffer[size] = '\0';
            has_line = true;
        } else {
            // Fill the local buffer with bytes from the file, then inserts
            // the bytes into the ring buffer.
            rc = read(fd, buffer, BUFSIZ);
            if(rc <= 0)
                return CONFIG_PARSER_READ_ERROR;
            buffer[rc] = '\0';
            ring_buffer_insert_buffer(ring_buffer, buffer, rc);
        }
    }

    return CONFIG_PARSER_STATUS_OK;
}

void config_parser_filter_line(char * line_buffer) {
    // Checking the precondition.
    assert(line_buffer != NULL && strlen(line_buffer) > 0);

    // TODO Implement.
}
