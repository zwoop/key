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

#include "include/arena.h"
#include "include/parser.h"
#include "include/parameters.h" /* ToDo: Should probably be removed */

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
        if (!(key = (key_t*)allocator(sizeof(key_t)))) {
            return NULL;
        }
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
        key_memset(&key->cache, 0, sizeof(key->cache));
    }

    return key;
}

void
key_release(key_t *key)
{
    if (key) {
        key->free(key);
    }
}

void
key_release_params(key_t *key, key_params_t params)
{
    assert(key);
    assert(params);

    /* The first Key parameter object is the first value in the arena, right after the arena object itself,
       so the start of the arena can be calculated from this. Yes, slightly ugly, but avoid wasting 8 bytes
       in every parameter object, or another struct object to manage the two. */
    key_arena_destroy(key, (key_arena_t *)((unsigned char*)params - KEY_ARENA_ALIGN(sizeof(key_arena_t))));
}


/* Main Key parser entry point. */
key_parse_status
key_parse(key_t *key, const char *key_string, size_t header_len, key_params_t *params, size_t *num_params)
{
    key_arena_t *arena;

    assert(key);
    if ((arena = key_arena_create(key))) {
        key_common_t* param;

        /* ToDo: This is obviously just made up, but testing the factory now */
        param = key_factory(arena, "substr", 6, "accept-encoding", 0);
        if (param) {
            key_param_substr_t *substr = (key_param_substr_t *)param;

            substr->substr = "gzip";
            substr->substr_len = 4;
            *params = (key_params_t)param;
        }
        return KEY_PARSE_OK;
    }

    return KEY_PARSE_ERROR;
}

/* Main evaluation entry point */
int
key_eval(key_t *key, void *header_data, key_params_t params, char *buf, size_t buf_size)
{
    key_common_t *param = (key_common_t*)params;
    size_t pos = 0;

    while (param) {
        size_t val_len;
        const char* value = key->get_header(header_data, param->header, param->header_len, &val_len);

        if (value && (val_len > 0)) {
            if (pos < buf_size) { /* Room for at least one digit, no need to check those */
                int len = param->evaluator(param, value, val_len, buf, pos, buf_size);

                pos += len;
            } else {
                /* ToDo: Deal with buffer error */
                break;
            }
        } else {
            /* This deals with step 1 in all evaluators; header is not present */
            if ((buf_size - pos) >= 4) {
                memcpy(buf + pos, "none", 4);
                pos += 4;
            } else {
                /* ToDo: Deal with errors? */
                break;
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
