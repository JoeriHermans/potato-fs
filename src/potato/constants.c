/**
 * Explicit definitions of the constants defined in constants.h.
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

// BEGIN Includes. ///////////////////////////////////////////////////

// Application dependencies.
#include <potato/constants.h>

// END Includes. /////////////////////////////////////////////////////

const char k_usage[] = "PotatoFS 0.0.1\n"
                       "Daeaon:\n"
                       "\t--daemon, -d\t Run as daemon, required argument.\n"
                       "\t--config, -c\t Specify configuration file, required argument.\n"
                       "\n"
                       "User:\n";

const char k_newline = '\n';

const char k_arg_config[] = "--config";
const char k_arg_config_s[] = "-c";
const char k_arg_daemon[] = "--daemon";
const char k_arg_daemon_s[] = "-d";

const char k_log_close_config[] = "Closing configuration file.";
const char k_log_initializing[] = "Initializing PotatoFS daemon.";
const char k_log_open_config[] = "Opening configuration file at %s.";
const char k_log_open_config_error[] = "Configuration file '%s' does not exist, or does not have the required read permissions.";
const char k_log_parse_config[] = "Parsing configuration file.";
const char k_log_stop_cause_error[] = "Stopping PatotoFS due to error, please fix.";

const char k_config_comment = '#';