/**
 * Defines all constants.
 *
 * @date                    25 08 2017
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

#ifndef _H_CONSTANTS_
#define _H_CONSTANTS_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <stdlib.h>
#include <stddef.h>

// END Includes. /////////////////////////////////////////////////////

extern const char k_usage[];

extern const char k_equal;
extern const char k_false[];
extern const char k_newline;
extern const char k_program_name[];
extern const char k_trim_characters[];
extern const char k_true[];
extern const size_t k_trim_num_characters;
extern const char k_dev_urandom[];

extern const char k_arg_config[];
extern const char k_arg_config_s[];
extern const char k_arg_daemon[];
extern const char k_arg_daemon_s[];

extern const char k_log_close_config[];
extern const char k_log_config_parsed[];
extern const char k_log_daemonizing[];
extern const char k_log_initializing[];
extern const char k_log_mount_success[];
extern const char k_log_mountpoint_not_exists[];
extern const char k_log_no_root[];
extern const char k_log_open_config[];
extern const char k_log_open_config_error[];
extern const char k_log_parse_config[];
extern const char k_log_potato_initializing[];
extern const char k_log_stop_cause_error[];
extern const char k_log_stopping_potato[];

extern const char k_config_blank_characters[];
extern const char k_config_block_sharing[];
extern const char k_config_comment;
extern const char k_data_node[];
extern const char k_config_daemonize[];
extern const char k_config_data_directory[];
extern const char k_config_default_block_replication[];
extern const char k_config_default_block_size[];
extern const char k_config_default_value[];
extern const char k_config_default_value_int[];
extern const char k_config_illegal_value[];
extern const char k_config_max_peers[];
extern const char k_config_mountpoint[];
extern const char k_config_not_found[];
extern const char k_config_setting_to[];
extern const size_t k_config_num_blank_characters;

#endif
