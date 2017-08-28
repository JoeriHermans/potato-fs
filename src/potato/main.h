/**
 * Utility methods to assist with the startup of the filesystem.
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

#ifndef _H_MAIN_
#define _H_MAIN_

// BEGIN Includes. ///////////////////////////////////////////////////

// System dependencies.
#include <linux/limits.h>
#include <stdbool.h>

// END Includes. /////////////////////////////////////////////////////

/**
 * Checks if the user specified the program to run in Daemon mode.
 *
 * @param argc
 *        The number of arguments in the argv array.
 * @param argv
 *        Array of arguments.
 * @return True, if '-d' or '--daemon' is specified in the arguments array.
 *         False in any other case.
 */
bool daemon_mode_specified(const int argc, const char ** argv);

/**
 * Checks if the arguments contains the specified key.
 *
 * @param key
 *        The string that needs to be searched for.
 * @param argc
 *        The number of arguments in the arguments array.
 * @param argv
 *        Array of arguments.
 * @return True, if the specified key is present in the arguments array.
 *         False in any other case.
 */
bool has_argument(const char * key, const int argc, const char ** argv);

/**
 * Checks if the specified arguments are valid depending on the program mode
 * that has been specified by the user.
 */
bool valid_arguments(const int argc, const char ** argv);

/**
 * Returns the index of the argument at which the specified key has been found.
 * Returns -1 if the key isn't found.
 *
 * @param key
 *        The key to search for.
 * @param argc
 *        The number of arguments.
 * @param argv
 *        Array of arguments.
 * @returns The index ([0, argc - 1]) if the specified key is found, -1 otherwise.
 */
int get_argument(const char * key, const int argc, const char ** argv);

int main(const int argc, const char ** argv);

/**
 * Runs the daemon mode of the program.
 */
int run_mode_daemon(const int argc, const char ** argv);

/**
 * Runs the user mode of the program, functionality depends on the
 * program arguments.
 */
int run_mode_user(const int argc, const char ** argv);

/**
 * Retrieves the configuration path specified by the program arguments, and places
 * the contents in the char buffer.
 */
void fetch_configuration_path(char * buffer, const int argc, const char ** argv);

/**
 * Prints the usage of the program to stdout.
 */
void usage(void);

#endif
