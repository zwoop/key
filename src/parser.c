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
#include <assert.h>
#include <ctype.h>

#include "include/parser.h"
#include "include/evaluators.h"

#if HAVE__BOOL
#include <stdbool.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STRINGS_H
#include <strings.h>
#endif

/* These are the template structures for each of the supported parameter evaluator types */
static const key_param_div_t g_div = {
    .c.type = KEY_PARAM_DIV,
    .c.evaluator = &key_eval_div,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "DIV",
    .c.arena = NULL,
    .c.next = NULL,
    .divider = 0,
};

static const key_param_partition_t g_partition = {
    .c.type = KEY_PARAM_PARTITION,
    .c.evaluator = &key_eval_partition,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "PARTITION",
    .c.arena = NULL,
    .c.next = NULL,
    .partitions = {0},
    .num_partitions = 0,
};

static const key_param_match_t g_match = {
    .c.type = KEY_PARAM_MATCH,
    .c.evaluator = &key_eval_match,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "MATCH",
    .c.arena = NULL,
    .c.next = NULL,
    .match = NULL,
    .match_len = 0,
};

static const key_param_substr_t g_substr = {
    .c.type = KEY_PARAM_SUBSTR,
    .c.evaluator = &key_eval_substr,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "SUBSTR",
    .c.arena = NULL,
    .c.next = NULL,
    .substr = NULL,
    .substr_len = 0,
};

static const key_param_param_t g_param = {
    .c.type = KEY_PARAM_PARAM,
    .c.evaluator = &key_eval_param,
    .c.header = NULL,
    .c.header_len = 0,
    .c.debug_name = "param",
    .c.arena = NULL,
    .c.next = NULL,
    .param = NULL,
    .param_len = 0,
};

/* This is an specialized implementation of strsep(), obviously not compatible, but useful
   for us since it does the following:

   1) Separates tokens on the character
   2) Does not modify the input string
   3) Does not need to be a NULL terminated string (hence the wonky passing of value/value_len)
   4) Strips leading and trailing spaces

   ToDo: This must deal with quotations ("") !!
p*/
size_t
key_strsep(const char* value, size_t value_len, const char** start, const char** next, const char separator)
{
    const char *token_end = NULL; /* Used for calculating the size of the token */

    /* Strip any leading whitespaces */
    while (*start && ((value_len - (*start - value)) > 0) && isspace(**start)) {
        ++*start;
    }
    /* Not exactly necessary, but avoids some calls in the case of trailing spaces. */
    if (*start >= (value + value_len)) {
        return 0;
    }

    /* Look for a separator character */
    if ((*next = key_memchr(*start, separator, value_len - (*start - value)))) {
        token_end = *next - 1;
    } else {
        /* No separators left in the string, end of token is end of value string */
        token_end = *next = (value + value_len - 1);
    }

    /* Strip trailing whitespaces */
    while ((token_end > *start) && isspace(*token_end)) {
        --token_end;
    }

    /* Skip past the separator for the next token */
    ++*next;

    return (token_end - *start + 1);
}

/* This is the main factory for creating new objects. */
key_common_t *
key_factory(key_arena_t *arena, const char *param_str, size_t param_len, const char *header, size_t header_len)
{
    key_common_t *param = NULL;
    const char* delim = memchr(param_str, '=', param_len);
    size_t type_len, arg_len;
    void *arg;

    if (!delim) {
        return NULL;
    }

    /* Setup the parameter argument, which must be copied unto the arena */
    type_len = (delim - param_str);
    arg_len = (param_str + param_len - delim - 1);
    if (!(arg = key_arena_allocate(arena, arg_len))) {
        return NULL;
    }
    memcpy(arg, delim + 1, arg_len);

    if (0 == header_len) {
        header_len = strlen(header);
    }

    switch (type_len) {
    case 3: /* DIV */
        if (!key_strncasecmp(param_str, "div", 3)) {
            key_param_div_t *p = (key_param_div_t *)key_arena_allocate(arena, sizeof(key_param_div_t));

            if (p) {
                memcpy(p, &g_div, sizeof(g_div)); /* Copy the template */
                param = &p->c;
            }
        }
        break;
    case 9: /* PARTITION */
        if (!key_strncasecmp(param_str, "partition", 9)) {
            key_param_partition_t *p = (key_param_partition_t*)key_arena_allocate(arena, sizeof(key_param_partition_t));

            if (p) {
                memcpy(p, &g_partition, sizeof(g_partition)); /* Copy the template */
                param = &p->c;
            }
        }
        break;
    case 5: /* MATCH and PARAM */
        switch (*param_str) {
        case 'm':
        case 'M':
            if (!key_strncasecmp(param_str, "match", 5)) {
                key_param_match_t *p = (key_param_match_t *)key_arena_allocate(arena, sizeof(key_param_match_t));

                if (p) {
                    memcpy(p, &g_match, sizeof(g_match)); /* Copy the template */
                    param = &p->c;
                }
            }
            break;
        case 'p':
        case 'P':
            if (!key_strncasecmp(param_str, "param", 5)) {
                key_param_param_t *p = (key_param_param_t *)key_arena_allocate(arena, sizeof(key_param_param_t));

                if (p) {
                    memcpy(p, &g_param, sizeof(g_param)); /* Copy the template */
                    param = &p->c;
                }
            }
            break;
        }
        break;
    case 6: /* SUBSTR */
        if (!key_strncasecmp(param_str, "substr", 6)) {
            key_param_substr_t *p = (key_param_substr_t *)key_arena_allocate(arena, sizeof(key_param_substr_t));

            if (p) {
                memcpy(p, &g_substr, sizeof(g_substr)); /* Copy the template */
                p->substr = arg;
                p->substr_len = arg_len;
                param = &p->c;
            }
        }
        break;
    default: /* Unknown */
        break;
    }

    /* Dup the header string unto the arena */
    if (param) {
        void *hdr = key_arena_allocate(arena, header_len + 1); /* We do NULL terminate this header string */

        param->arena = arena;
        if (hdr) {
            memcpy(hdr, header, header_len);
            *((char*)hdr + header_len) = '\0';
            param->header = (const char*)hdr;
            param->header_len = header_len;

            return param; /* We can always cast this back to the right type, based on the parameter type */
        }
    }

    return NULL; /* Could be memory allocation issue, *or* a bad string, we don't really care. */
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
