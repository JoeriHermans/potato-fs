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

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/network.h>

// System dependencies.
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

int network_allocate_tcp_port(const uint16_t port) {
    static int yes = 1;
    struct addrinfo hints;
    struct addrinfo * server_info;
    char string_port[6];
    int result;
    int rc;
    int fd;

    // Checking the precondition.
    assert(port > 0);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family    = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM;
    hints.ai_flags     = AI_PASSIVE;
    sprintf(string_port, "%u", port);
    rc = getaddrinfo(NULL, string_port, &hints, &server_info);
    if(rc >= 0) {
        fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
        if(fd >= 0) {
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
            if(bind(fd, server_info->ai_addr, server_info->ai_addrlen) == 0 && listen(fd, 64) == 0) {
                result = fd;
            } else {
                close(fd);
                result = -1;
            }
        } else {
            result = -1;
        }
    } else {
        result = -1;
    }

    return result;
}

int network_allocate_udp_port(const uint16_t port) {
    static int yes = 1;
    struct sockaddr_in in;
    int fd = -1;

    // Checking the precondition.
    assert(port > 0);

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(fd >= 0) {
        in.sin_family = AF_INET;
        in.sin_port = htons(port);
        in.sin_addr.s_addr = htonl(INADDR_ANY);
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
        if(bind(fd, (struct sockaddr *) &in, sizeof(in)) == 1) {
            close(fd);
            fd = -1;
        }
    }

    return fd;
}

int network_allocate_udp_socket(void) {
    int fd = -1;

    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    return fd;
}

bool network_cork_disable(const int fd) {
    static const int optval = 1;
    bool success;

    success = (setsockopt(fd, SOL_SOCKET, TCP_CORK, &optval, sizeof(optval)) >= 0);

    return success;
}

bool network_cork_enable(const int fd) {
    static const int optval = 0;
    bool success;

    success = (setsockopt(fd, SOL_SOCKET, TCP_CORK, &optval, sizeof(optval)) >= 0);

    return success;
}

bool network_keep_alive_disable(const int fd) {
    static const int optval = 0;
    bool success;

    success = (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) >= 0);

    return success;
}

bool network_keep_alive_enable(const int fd) {
    static const int optval = 1;
    bool success;

    success = (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) >= 0);

    return success;
}

bool network_nable_disable(const int fd) {
    static const int optval = 0;
    bool success;

    success = (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) >= 0);

    return success;
}

bool network_nable_enable(const int fd) {
    static const int optval = 1;
    bool success;

    success = (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) >= 0);

    return success;
}

int network_connect(const char * address, const uint16_t port) {
    struct addrinfo hints;
    struct addrinfo * results;
    char string_port[6];
    int result;
    int rc;
    int fd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family    = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM;
    hints.ai_flags     = AI_PASSIVE;
    sprintf(string_port, "%u", port);
    rc = getaddrinfo(address, string_port, &hints, &results);
    if(results != NULL && rc >= 0) {
        fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
        if(fd >= 0) {
            if(connect(fd, results->ai_addr, results->ai_addrlen) == 0) {
                result = fd;
            } else {
                close(fd);
                result = -1;
            }
        } else {
            result = -1;
        }
    } else {
        result = -1;
    }

    return result;
}

void network_timeout_receive(const int fd, const struct timeval tv) {
    // Checking the precondition.
    assert(fd >= 0);

    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

void network_timeout_send(const int fd, const struct timeval tv) {
    // Checking the precondition.
    assert(fd >= 0);

    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
}
