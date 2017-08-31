/**
 * Networking utility methods.
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

#ifndef _H_NETWORK_
#define _H_NETWORK_

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/util.h>

// System dependencies.
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// END Includes. /////////////////////////////////////////////////////

int network_allocate_tcp_port(const uint16_t port);

bool network_cork_disable(const int fd);

bool network_cork_enable(const int fd);

bool network_keep_alive_disable(const int fd);

bool network_keep_alive_enable(const int fd);

bool network_nagle_disable(const int fd);

bool network_nagle_enable(const int fd);

int network_connect(const char * address, const uint16_t port);

void network_timeout_receive(const int fd, const struct timeval tv);

void network_timeout_send(const int fd, const struct timeval tv);

#endif
