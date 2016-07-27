/** @file

    We create a template instance of each of the possible parameter evaluation
    struct's. This allows us for quick initialization with a simple memcpy().
    This is a factory "class", used to create new instances of these structs.

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
#include "include/parser.h"

#if HAVE_STRING_H
#include <string.h>
#endif

static const key_param_div_t g_div = {
    .c.type = KEY_PARAM_DIV,
    .c.evaluator = &key_eval_div,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "DIV",
    .divider = 0,
};

static const key_param_partition_t g_partition = {
    .c.type = KEY_PARAM_PARTITION,
    .c.evaluator = &key_eval_partition,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "PARTITION",
    .partitions = {0},
    .num_partitions = 0,
};

static const key_param_match_t g_match = {
    .c.type = KEY_PARAM_MATCH,
    .c.evaluator = &key_eval_match,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "MATCH",
    .match = NULL,
    .match_len = 0,
};

static const key_param_substr_t g_substr = {
    .c.type = KEY_PARAM_SUBSTR,
    .c.evaluator = &key_eval_substr,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "SUBSTR",
    .substr = NULL,
    .substr_len = 0,
};

static const key_param_param_t g_param = {
    .c.type = KEY_PARAM_PARAM,
    .c.evaluator = &key_eval_param,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "param",
    .param = NULL,
    .param_len = 0,
};

/* This is the main factory for creating new objects. */
void *
key_factory(key_arena_t *arena, key_param_types_t type, const char *header, size_t header_len)
{
    switch (type) {
        case KEY_PARAM_DIV: {
            key_param_div_t *p = (key_param_div_t *)key_arena_allocate(arena, sizeof(key_param_div_t));

            if (p) {
                memcpy(p, &g_div, sizeof(g_div));
                if ((p->c.header = (const char*)key_arena_memdup(arena, (void*)header, header_len))) {
                    p->c.header_len = header_len;
                    return (void *)p;
                }
            }
        } break;

        case KEY_PARAM_PARTITION: {
            key_param_partition_t *p = (key_param_partition_t*)key_arena_allocate(arena, sizeof(key_param_partition_t));

            if (p) {
                memcpy(p, &g_partition, sizeof(g_partition));
                if ((p->c.header = (const char*)key_arena_memdup(arena, (void*)header, header_len))) {
                    p->c.header_len = header_len;
                    return (void *)p;
                }
            }
        } break;

        case KEY_PARAM_MATCH: {
            key_param_match_t *p = (key_param_match_t *)key_arena_allocate(arena, sizeof(key_param_match_t));

            if (p) {
                memcpy(p, &g_match, sizeof(g_match));
                if ((p->c.header = (const char*)key_arena_memdup(arena, (void*)header, header_len))) {
                    p->c.header_len = header_len;
                    return (void *)p;
                }
            }
        } break;

        case KEY_PARAM_SUBSTR: {
            key_param_substr_t *p = (key_param_substr_t *)key_arena_allocate(arena, sizeof(key_param_substr_t));

            if (p) {
                memcpy(p, &g_substr, sizeof(g_substr));
                if ((p->c.header = (const char*)key_arena_memdup(arena, (void*)header, header_len))) {
                    p->c.header_len = header_len;
                    return (void *)p;
                }
            }
        } break;

        case KEY_PARAM_PARAM: {
            key_param_param_t *p = (key_param_param_t *)key_arena_allocate(arena, sizeof(key_param_param_t));

            if (p) {
                memcpy(p, &g_param, sizeof(g_param));
                if ((p->c.header = (const char*)key_arena_memdup(arena, (void*)header, header_len))) {
                    p->c.header_len = header_len;
                    return (void *)p;
                }
            }
        } break;
    }

    return NULL; /* Likely memory allocation (arena) failed, deal with it. */
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
