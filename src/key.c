/** @file

    Main entry points for the Key APIs; this implements the public
    interfaces as exposed via a

        #include <http/key.h>

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
#include <stdio.h>

#include "http/key.h"
#include "include/parameters.h"
#include "include/platform.h"

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

/* Initialize a Key object. This must be called before usage. */
http_key_t *
http_key_init(http_key_t *key, http_key_header_t get_header, http_key_malloc_t mem_alloc, http_key_free_t mem_free,
              size_t arena_size, http_key_cache_store_t cache_store, http_key_cache_lookup_t cache_lookup, void *cache_data)
{
    assert(get_header);
    assert(key);

    /* Setup the mandatory fields */
    key->get_header = get_header;
    key->malloc = mem_alloc ? mem_alloc : &malloc;
    key->free = mem_free ? mem_free : &free;
    key->arena_size = arena_size >= HTTP_KEY_MIN_ARENA ? arena_size : HTTP_KEY_MIN_ARENA;

    /* These can all be NULL, i.e. the Key parameter cache is optional */
    if (cache_store || cache_lookup || cache_data) {
        /* Must have all three, or none */
        assert(cache_store);
        assert(cache_lookup);
        assert(cache_data);

        key->cache.store = cache_store;
        key->cache.lookup = cache_lookup;
        key->cache.data = cache_data;
    } else {
        memset(&key->cache, 0, sizeof(key->cache));
    }

    return key;
}

void
http_key_release(http_key_params_t params)
{
    key_common_t *param = (key_common_t *)params;

    if (param && param->arena && param->arena->key) {
        key_arena_destroy(param->arena);
    }
}

/* Main evaluation entry point */
size_t
http_key_eval(http_key_t *key, void *header_data, http_key_params_t params, char *buf, size_t buf_size)
{
    key_common_t *param = (key_common_t *)params;
    size_t pos = 0;
    const char *last_header = NULL;
    size_t last_header_len = 0;
    const char *value = NULL;
    size_t val_len = 0;

    while (param) {

        if ((last_header_len != param->header_len) || (last_header != param->header)) {
            value = key->get_header(header_data, param->header, param->header_len, &val_len);
            last_header = param->header;
            last_header_len = param->header_len;
        }

        if (value && (val_len > 0)) {
            size_t len;

            /* In this case, the header can not be NULL, and we'll assure that there's room for at
               least one result character in the buffer. Neither of those conditions needs to be
               checked for in the individual evaluators. */

            if ((pos < buf_size) && ((len = param->evaluator(param, value, val_len, buf, pos, buf_size)) > 0)) {
                pos += len;
            } else {
                return 0; /* Error. We choose to abort the entire evaluation, as per the RFC. */
            }
        } else {
            /* This deals with step 1 in all evaluators; header is not present. */
            if ((buf_size - pos) >= 4) {
                memcpy(buf + pos, "none", 4);
                pos += 4;
            } else {
                return 0; /* Error. We choose to abort the entire evaluation, as per the RFC. */
            }
        }
        param = param->next;
    }

    return pos;
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
