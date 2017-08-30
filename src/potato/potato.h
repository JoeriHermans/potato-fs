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

// Application dependencies.
#include <potato/hashmap.h>

// System dependencies.
#include <fuse.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// END Includes. /////////////////////////////////////////////////////

#define POTATO_STATUS_OK 0
#define POTATO_STATUS_FAILURE -1

int potato_initialize(const int argc, const char ** argv, const map_t * settings);

int potato_open(const char * path, struct fuse_file_info * fi);

int potato_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi);

int potato_readdir(const char *path, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi, enum fuse_readdir_flags flags);

#endif
