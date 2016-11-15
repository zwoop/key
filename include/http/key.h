/** @file

    This is the main include file for the HTTP Key library. Use it
    like

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
#ifndef HTTP_KEY_H
#define HTTP_KEY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_MAX_PARTITIONS 32
#define KEY_MIN_ARENA 128

/* Holds one single key parameter "rule", which is opaque in the public APIs. This does hold
   all the information necessary for a single parameter rule, but you must not modify it directly. */
typedef struct _key_params *key_params_t;

/**
 * @brief Callback function, for retrieving a header value, managing memory and a lookup cache.
 *
 * This is a prototype declaration, for declaring the callback need to retrieve a header value,
 * manage various aspects of memory, as well as the optional parser cache.
 */
typedef const char *(*key_header_t)(void *, const char *, size_t, size_t *);
typedef void *(*key_malloc_t)(size_t);
typedef void (*key_free_t)(void *);

typedef void (*key_cache_store_t)(void *, const char *, size_t, key_params_t);
typedef const key_params_t (*key_cache_lookup_t)(void *, const char *, size_t);

typedef struct {
    key_header_t get_header;
    key_malloc_t malloc;
    key_free_t free;
    size_t arena_size;

    /* These are optional */
    struct {
        key_cache_store_t store;
        key_cache_lookup_t lookup;
        void *data;
    } cache;
} key_t;

typedef enum {
    KEY_PARSE_OK,
    KEY_PARSE_ERROR,
} key_parse_status;

/* Public interfaces */
key_t *key_init(key_t *key, key_header_t get_header, key_malloc_t mem_alloc, key_free_t mem_free, size_t arena_size,
                key_cache_store_t cache_store, key_cache_lookup_t cache_lookup, void *cache_data);
void key_release(key_t *key);

key_parse_status key_parse(key_t *key, const char *key_string, size_t key_string_len, key_params_t *params, size_t *num_params);
key_parse_status key_parse_buffer(void *buffer, size_t buffer_size, const char *key_string, size_t key_string_len,
                                  key_params_t *params, size_t *num_params);

int key_eval(key_t *key, void *header_data, key_params_t params, char *buf, size_t buf_size);

void key_release_params(key_t *key, key_params_t params);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_KEY_H */

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
