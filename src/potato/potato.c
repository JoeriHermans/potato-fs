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

static struct fuse_operations potato_operations = {
    .open         = potato_open,
    .read         = potato_read,
    .readdir      = potato_readdir
};

int potato_initialize(const int argc, const char ** argv, const map_t * settings) {
    static char * program_argument[1];
    program_argument[0] = "potato";
    static struct fuse_args fuse_arguments = FUSE_ARGS_INIT(1, program_argument);
    struct potatofs potatofs;
    struct fuse_session * fuse_session;
    struct fuse * fuse;
    int rc;

    // Checking the preconditions.
    assert(settings != NULL);

    potato_initialize_structure(&potatofs, settings);
    syslog(LOG_NOTICE, k_log_potato_initializing);
    fuse = fuse_new(&fuse_arguments, &potato_operations, sizeof(struct fuse_operations), NULL);
    if(fuse == NULL)
        return POTATO_STATUS_FUSE_ALLOCATION;
    fuse_session = fuse_get_session(fuse);
    rc = fuse_set_signal_handlers(fuse_session);
    if(rc != 0) {
        fuse_destroy(fuse);

        return POTATO_STATUS_FUSE_SIGNAL_HANDLERS;
    }
    rc = fuse_mount(fuse, potatofs.mountpoint);
    if(rc != 0) {
        fuse_destroy(fuse);

        return POTATO_STATUS_FUSE_MOUNT_FAILURE;
    }
    fcntl(fuse_session_fd(fuse_session), F_SETFD, FD_CLOEXEC);
    rc = fuse_daemonize(potatofs.daemonize);
    if(rc != 0) {
        fuse_unmount(fuse);
        fuse_destroy(fuse);

        return POTATO_STATUS_FUSE_DAEMONIZATION_FAILED;
    }
    rc = fuse_loop_mt(fuse, 0);
    if(rc != 0) {
        fuse_unmount(fuse);
        fuse_destroy(fuse);

        return POTATO_STATUS_FUSE_MT_FAILURE;
    }
    // TODO Implement.
    fuse_remove_signal_handlers(fuse_session);
    fuse_unmount(fuse);
    fuse_destroy(fuse);

    return POTATO_STATUS_OK;
}

void potato_initialize_structure(struct potatofs * potatofs, const map_t * settings) {
    // Checking the preconditions.
    assert(potatofs != NULL && settings != NULL);

    // Clear the structure.
    memset(potatofs, 0, sizeof(struct potatofs));
}

int potato_open(const char * path, struct fuse_file_info * fi) {
    // TODO Implement.

    return POTATO_STATUS_GENERIC_FAILURE;
}

int potato_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi) {
    // TODO Implement.

    return POTATO_STATUS_GENERIC_FAILURE;
}

int potato_readdir(const char * path, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi, enum fuse_readdir_flags flags) {
    // TODO Implement.

    return POTATO_STATUS_GENERIC_FAILURE;
}
