/** @file

    Implementation for the parameter evaluators.

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
#include <assert.h>

#include "include/parameters.h"
#include "include/evaluators.h"

int
key_eval_div(void *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_div_t *div = (key_param_div_t *)param;

    assert(div->c.type == KEY_PARAM_DIV);

    return 0;
}

int
key_eval_partition(void *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_partition_t *partition = (key_param_partition_t *)param;

    assert(partition->c.type == KEY_PARAM_PARTITION);

    return 0;
}

int
key_eval_match(void *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_match_t *match = (key_param_match_t *)param;

    assert(match->c.type == KEY_PARAM_MATCH);

    return 0;
}

int
key_eval_substr(void *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_substr_t *substr = (key_param_substr_t *)param;

    assert(substr->c.type == KEY_PARAM_SUBSTR);

    return 0;
}

int
key_eval_param(void *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_param_t *substr = (key_param_param_t *)param;

    assert(substr->c.type == KEY_PARAM_PARAM);

    return 0;
}

/*
  local variables:
  mode: C
  indent-tabs-mode: nil
  c-basic-offset: 4
  c-file-offsets: ((statement-block-intro . +)
  (label . 0)
  (statement-cont . +))
  end:
*/
