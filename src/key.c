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
key_t *
key_init(key_t *key, key_header_t get_header, key_malloc_t mem_alloc, key_free_t mem_free, size_t arena_size,
         key_cache_store_t cache_store, key_cache_lookup_t cache_lookup, void *cache_data)
{
    key_malloc_t allocator = mem_alloc ? mem_alloc : &malloc;

    assert(get_header);

    if (!key) {
        if (!(key = (key_t *)allocator(sizeof(key_t)))) {
            return NULL;
        }
        key->allocated = 1;
    } else {
        key->allocated = 0;
    }

    key->get_header = get_header;
    key->malloc = allocator;
    key->free = mem_free ? mem_free : &free;
    key->arena_size = arena_size >= KEY_MIN_ARENA ? arena_size : KEY_MIN_ARENA;

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
        p_key_memset(&key->cache, 0, sizeof(key->cache));
    }

    return key;
}

void
key_release(key_t *key)
{
    if (key && key->allocated) {
        key->free(key);
    }
}

void
key_release_params(key_t *key, key_params_t params)
{
    key_common_t *param = (key_common_t *)params;

    assert(key);
    assert(param);

    p_key_arena_destroy(param->arena);
}

/* Main evaluation entry point */
size_t
key_eval(key_t *key, void *header_data, key_params_t params, char *buf, size_t buf_size)
{
    key_common_t *param = (key_common_t *)params;
    size_t pos = 0;

    while (param) {
        size_t val_len;
        const char *value = key->get_header(header_data, param->header, param->header_len, &val_len);

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
