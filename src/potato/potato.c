/**
 * Potato Filesystem, fully P2P filesystem for big datasets and analytics.
 * Probably crappy, hence the name.
 *
 *
 * @date                    30 08 2017
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
#include <potato/hashmap.h>
#include <potato/potato.h>
#include <potato/util.h>

// System dependencies.
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <syslog.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

struct fuse_operations potato_operations = {
    .open         = potato_open,
    .read         = potato_read,
    .readdir      = potato_readdir
};

int potato_initialize(const int argc, const char ** argv, const map_t * settings) {
    int rc;

    // Checking the preconditions.
    assert(settings != NULL);

    syslog(LOG_NOTICE, k_log_potato_initializing);
    rc = fuse_main(argc, (char **) argv, &potato_operations, NULL);

    return rc;
}

int potato_open(const char * path, struct fuse_file_info * fi) {
    // TODO Implement.

    return POTATO_STATUS_FAILURE;
}

int potato_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi) {
    // TODO Implement.

    return POTATO_STATUS_FAILURE;
}

int potato_readdir(const char * path, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi) {
    // TODO Implement.

    return POTATO_STATUS_FAILURE;
}
