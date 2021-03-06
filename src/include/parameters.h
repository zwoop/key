/** @file

    Defines the various parameter types, used for later run-time
    evaluation.

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
#ifndef KEY_PARAMETERS_H
#define KEY_PARAMETERS_H

#include "include/arena.h"

#include "include/platform.h"

#if HAVE_STDINT_H
#include <stdint.h>
#endif

struct _key_common;
typedef size_t(key_evaluator_t)(struct _key_common *param, const char *value, size_t value_len, char *buf, size_t start,
                                size_t buf_size);

typedef enum {
    KEY_PARAM_DIV,
    KEY_PARAM_PARTITION,
    KEY_PARAM_MATCH,
    KEY_PARAM_SUBSTR,
    KEY_PARAM_PARAM,
} key_param_types_t;

typedef struct _key_common {
    key_param_types_t type;
    key_evaluator_t *evaluator;
    const char *header;
    size_t header_len;
    const char *debug_name;
    key_arena_t *arena; /* Slightly wasteful, but ce la vie */
    struct _key_common *next;
} key_common_t;

typedef struct {
    key_common_t c;
    uint64_t divider;
} key_param_div_t;

typedef struct {
    key_common_t c;
    uint64_t partitions[HTTP_KEY_MAX_PARTITIONS]; /* ToDo: Eeep */
    size_t num_partitions;
} key_param_partition_t;

typedef struct {
    key_common_t c;
    const char *match;
    size_t match_len;
} key_param_match_t;

typedef struct {
    key_common_t c;
    const char *substr;
    size_t substr_len;
} key_param_substr_t;

typedef struct {
    key_common_t c;
    const char *param;
    size_t param_len;
} key_param_param_t;

#endif /* KEY_PARAMETERS_H */

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
