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
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <syslog.h>
#include <unistd.h>

// END Includes. /////////////////////////////////////////////////////

static struct fuse_operations potato_operations = {
    .getattr	  = potato_getattr,
    .init         = potato_init,
    .open         = potato_open,
    .read         = potato_read,
    .readdir      = potato_readdir
};

int potato_main(const int argc, const char ** argv, const map_t * settings) {
    static char * program_argument[1];
    program_argument[0] = "potato";
    static struct fuse_args fuse_arguments = FUSE_ARGS_INIT(1, program_argument);
    struct potatofs potatofs;
    struct fuse_session * fuse_session;
    struct fuse * fuse;
    int rc;

    // Checking the preconditions.
    assert(settings != NULL);

    potato_init_struct(&potatofs, settings);
    // Allocate a FUSE session.
    syslog(LOG_NOTICE, k_log_potato_initializing);
    fuse = fuse_new(&fuse_arguments, &potato_operations, sizeof(struct fuse_operations), NULL);
    if(fuse == NULL)
        return POTATO_STATUS_FUSE_ALLOCATION;
    // Fetch the FUSE session and set the FUSE signal handlers.
    fuse_session = fuse_get_session(fuse);
    rc = fuse_set_signal_handlers(fuse_session);
    if(rc != 0) {
        fuse_destroy(fuse);

        return POTATO_STATUS_FUSE_SIGNAL_HANDLERS;
    }
    // Check if the mountpoint is accessible.
    if(potatofs.mountpoint && access(potatofs.mountpoint, F_OK) == 0) {
        // Mount the FUSE filesystem.
        rc = fuse_mount(fuse, potatofs.mountpoint);
        if(rc != 0) {
            fuse_destroy(fuse);

            return POTATO_STATUS_FUSE_MOUNT_FAILURE;
        }
    } else {
        syslog(LOG_ERR, k_log_mountpoint_not_exists, potatofs.mountpoint);

        return POTATO_STATUS_MOUNTPOINT_NOT_EXISTS;
    }
    syslog(LOG_NOTICE, k_log_mount_success, potatofs.mountpoint);
    fcntl(fuse_session_fd(fuse_session), F_SETFD, FD_CLOEXEC);
    // Daemonize the FUSE session if required.
    if(potatofs.daemonize) {
        syslog(LOG_NOTICE, k_log_daemonizing);
        rc = fuse_daemonize(potatofs.daemonize);
        if(rc != 0) {
            fuse_unmount(fuse);
            fuse_destroy(fuse);

            return POTATO_STATUS_FUSE_DAEMONIZATION_FAILED;
        }
    }
    // Run FUSE in a multithreaded loop.
    rc = fuse_loop_mt(fuse, 0);
    if(rc != 0) {
        fuse_unmount(fuse);
        fuse_destroy(fuse);

        return POTATO_STATUS_FUSE_MT_FAILURE;
    }
    // FUSE loop done, cleanup.
    fuse_remove_signal_handlers(fuse_session);
    fuse_unmount(fuse);
    fuse_destroy(fuse);
    syslog(LOG_NOTICE, k_log_stopping_potato);

    return POTATO_STATUS_OK;
}

void potato_init_struct(struct potatofs * potatofs, const map_t * settings) {
    // Checking the preconditions.
    assert(potatofs != NULL && settings != NULL);

    // Clear the structure.
    memset(potatofs, 0, sizeof(struct potatofs));
    // Fetch the parameters from the hashmap.
    potato_init_struct_mountpoint(potatofs, settings);
    potato_init_struct_default_block_replication(potatofs, settings);
    potato_init_struct_daemonize(potatofs, settings);
    potato_init_struct_data_directory(potatofs, settings);
    potato_init_struct_default_block_size(potatofs, settings);
}

void potato_init_struct_mountpoint(struct potatofs * potatofs, const map_t * settings) {
    int rc;

    // Checking the precondition.
    assert(potatofs != NULL && settings != NULL);

    rc = hashmap_get(settings, k_config_mountpoint, (void **) &potatofs->mountpoint);
    if(rc == HASHMAP_STATUS_NOT_FOUND) {
        syslog(LOG_ERR, k_config_not_found, k_config_mountpoint);
    } else {
        syslog(LOG_NOTICE, k_config_setting_to, k_config_mountpoint, potatofs->mountpoint);
    }
}

void potato_init_struct_default_block_replication(struct potatofs * potatofs, const map_t * settings) {
    char * value;
    int rc;

    // Checking the precondition.
    assert(potatofs != NULL && settings != NULL);

    rc = hashmap_get(settings, k_config_default_block_replication, (void **) &value);
    if(rc == HASHMAP_STATUS_NOT_FOUND) {
        syslog(LOG_ERR, k_config_not_found, k_config_default_block_replication);
        syslog(LOG_NOTICE, k_config_default_value_int, k_config_default_block_replication, POTATO_SETTING_DEFAULT_BLOCK_REPLICATION);
        potatofs->default_block_replication = POTATO_SETTING_DEFAULT_BLOCK_REPLICATION;
    } else {
        // Check if the specified value is an unsigned integer.
        if(is_unsigned_integer(value)) {
            potatofs->default_block_replication = (size_t) strtoul(value, NULL, 0);
            syslog(LOG_NOTICE, k_config_setting_to, k_config_default_block_replication, value);
        } else {
            syslog(LOG_ERR, k_config_illegal_value, k_config_default_block_replication, value);
            syslog(LOG_NOTICE, k_config_default_value_int, k_config_default_block_replication, POTATO_SETTING_DEFAULT_BLOCK_REPLICATION);
            potatofs->default_block_replication = POTATO_SETTING_DEFAULT_BLOCK_REPLICATION;
        }
    }
}

void potato_init_struct_daemonize(struct potatofs * potatofs, const map_t * settings) {
    const char * flag_value;
    char * value;
    bool flag;
    int rc;

    // Checking the preconditions.
    assert(potatofs != NULL && settings != NULL);

    rc = hashmap_get(settings, k_config_daemonize, (void **) &value);
    if(rc == HASHMAP_STATUS_OK) {
        // Check if the specified value is valid.
        if(strcmp(value, k_true) == 0) {
            flag = true;
            flag_value = k_true;
        } else {
            flag = false;
            flag_value = k_false;
        }
        syslog(LOG_NOTICE, k_config_setting_to, k_config_daemonize, flag_value);
        potatofs->daemonize = flag;
    } else {
        syslog(LOG_NOTICE, k_config_default_value, k_config_daemonize, POTATO_SETTING_DEFAULT_DAEMONIZE);
        potatofs->daemonize = POTATO_SETTING_DEFAULT_DAEMONIZE;
    }
}

void potato_init_struct_data_directory(struct potatofs * potatofs, const map_t * settings) {
    int rc;

    // Checking the precondition.
    assert(potatofs != NULL && settings != NULL);

    rc = hashmap_get(settings, k_config_data_directory, (void **) &potatofs->data_directory);
    if(rc == HASHMAP_STATUS_OK)
        syslog(LOG_NOTICE, k_config_setting_to, k_config_data_directory, potatofs->data_directory);
    else
        syslog(LOG_ERR, k_config_not_found, k_config_data_directory);
}

void potato_init_struct_default_block_size(struct potatofs * potatofs, const map_t * settings) {
    char * value;
    int rc;

    // Checking the precondition.
    assert(potatofs != NULL && settings != NULL);

    rc = hashmap_get(settings, k_config_default_block_size, (void **) &value);
    if(rc == HASHMAP_STATUS_NOT_FOUND) {
        syslog(LOG_ERR, k_config_not_found, k_config_default_block_size);
        syslog(LOG_NOTICE, k_config_default_value_int, k_config_default_block_size, POTATO_SETTING_DEFAULT_BLOCK_SIZE);
        potatofs->default_block_size = POTATO_SETTING_DEFAULT_BLOCK_SIZE;
    } else {
        // Check if the specified value is an unsigned integer.
        if(is_unsigned_integer(value)) {
            potatofs->default_block_size = (size_t) strtoul(value, NULL, 0);
            syslog(LOG_NOTICE, k_config_setting_to, k_config_default_block_size, value);
        } else {
            syslog(LOG_ERR, k_config_illegal_value, k_config_default_block_size, value);
            syslog(LOG_NOTICE, k_config_default_value_int, k_config_default_block_size, POTATO_SETTING_DEFAULT_BLOCK_SIZE);
            potatofs->default_block_size = POTATO_SETTING_DEFAULT_BLOCK_SIZE;
        }
    }
}

void * potato_init(struct fuse_conn_info * conn, struct fuse_config * config) {
    (void) conn;
    config->kernel_cache = true;

    return NULL;
}

int potato_open(const char * path, struct fuse_file_info * fi) {
    	if (strcmp(path+1, "test") != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	return 0;
}

int potato_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi) {
    	size_t len;
	(void) fi;
	if(strcmp(path+1, "test") != 0)
		return -ENOENT;

	len = strlen("MEEEEEPPPPPPPPPPPPP");
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, "MEEEEEPPPPPPPPPPPPP" + offset, size);
	} else
		size = 0;

	return size;
}

int potato_readdir(const char * path, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi, enum fuse_readdir_flags flags) {
	(void) offset;
	(void) fi;
	(void) flags;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);
	filler(buf, "test", NULL, 0, 0);

	return 0;
}

int potato_getattr(const char * path, struct stat * stbuf, struct fuse_file_info * fi) {
	(void) fi;
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path+1, "test") == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen("MEEEEEPPPPPPPPPPPPP");
	} else
		res = -ENOENT;

	return res;
}
