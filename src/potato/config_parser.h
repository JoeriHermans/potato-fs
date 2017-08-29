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

#ifndef _H_CONFIG_PARSER_
#define _H_CONFIG_PARSER_

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/constants.h>
#include <potato/hashmap.h>
#include <potato/util.h>
#include <potato/ring_buffer.h>

// System dependencies.
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

#define CONFIG_PARSER_STATUS_OK 0
#define CONFIG_PARSER_STATUS_COULDNT_OPEN -1
#define CONFIG_PARSER_KEY_COLLISION -2
#define CONFIG_PARSER_EOF_REACHED -3
#define CONFIG_PARSER_READ_ERROR -4
#define CONFIG_PARSER_BUFFER_OVERFLOW -5

bool config_parser_is_blank_line(const char * line_buffer);

int config_parser_read(map_t * properties, const char * path);

int config_parser_read_line(const int fd, ring_buffer_char_t * ring_buffer,
                            char * line_buffer, const size_t line_buffer_size);

void config_parser_filter_line(char * line_buffer);

void config_parser_process_file(const int fd, ring_buffer_char_t * ring_buffer, map_t * properties);

#endif
