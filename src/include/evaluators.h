/** @file

    Include file for all the various parameter evaluators.

    @section license License

    Licensed to the Apache Software Foundation (ASF) under one
    or more contributor license agreements.  See the NOTICE file
    distributed with this work for additional information
    regarding copyright ownership.  The ASF licenses this file
    to you under the Apache License, Version 2.0 (the
    "License"); you may not use this file except in compliance
    with the License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#ifndef EVALUATORS_H
#define EVALUATORS_H

#include "include/parameters.h"

size_t key_eval_div(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size);
size_t key_eval_partition(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size);
size_t key_eval_match(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size);
size_t key_eval_substr(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size);
size_t key_eval_param(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size);

#endif /* EVALUATORS_H */
