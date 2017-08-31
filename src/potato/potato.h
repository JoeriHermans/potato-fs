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

#ifndef _H_POTATO_
#define _H_POTATO_

// BEGIN Includes. ///////////////////////////////////////////////////

// FUSE Definition.
#define FUSE_USE_VERSION 31

// POSIX Fixes.
#ifndef S_IFMT
#define S_IFMT __S_IFMT
#define S_IFDIR __S_IFDIR
#define S_IFREG __S_IFREG
#endif

// Application dependencies.
#include <potato/hashmap.h>

// System dependencies.
#include <sys/stat.h>
#include <fuse.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// END Includes. /////////////////////////////////////////////////////

#define POTATO_SETTING_DEFAULT_BLOCK_REPLICATION 3
#define POTATO_SETTING_DEFAULT_DAEMONIZE true
#define POTATO_SETTING_DEFAULT_BLOCK_SIZE 1000000

#define POTATO_STATUS_OK 0
#define POTATO_STATUS_GENERIC_FAILURE -1
#define POTATO_STATUS_FUSE_ALLOCATION -2
#define POTATO_STATUS_FUSE_SIGNAL_HANDLERS -3
#define POTATO_STATUS_FUSE_MOUNT_FAILURE -4
#define POTATO_STATUS_FUSE_DAEMONIZATION_FAILED -5
#define POTATO_STATUS_FUSE_MT_FAILURE -6
#define POTATO_STATUS_MOUNTPOINT_NOT_EXISTS -7

struct potatofs {
    bool daemonize;
    char * data_directory;
    char * mountpoint;
    size_t default_block_replication;
    size_t default_block_size;
};

int potato_main(const int argc, const char ** argv, const map_t * settings);

void potato_init_struct(struct potatofs * potatofs, const map_t * settings);

void potato_init_struct_mountpoint(struct potatofs * potatofs, const map_t * settings);

void potato_init_struct_default_block_replication(struct potatofs * potatofs, const map_t * settings);

void potato_init_struct_daemonize(struct potatofs * potatofs, const map_t * settings);

void potato_init_struct_data_directory(struct potatofs * potatofs, const map_t * settings);

void potato_init_struct_default_block_size(struct potatofs * potatofs, const map_t * settings);

void  * potato_init(struct fuse_conn_info * conn, struct fuse_config * config);

int potato_open(const char * path, struct fuse_file_info * fi);

int potato_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi);

int potato_readdir(const char *path, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi, enum fuse_readdir_flags flags);

int potato_getattr(const char * path, struct stat * stbuf, struct fuse_file_info * fi);

#endif
