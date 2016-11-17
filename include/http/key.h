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

#define HTTP_KEY_MAX_PARTITIONS 32
#define HTTP_KEY_MIN_ARENA 128

/* Holds one single key parameter "rule", which is opaque in the public APIs. This does hold
   all the information necessary for a single parameter rule, but you must not modify it directly. */
typedef struct _http_key_params *http_key_params_t;

/**
 * @brief Callback function, for retrieving a header value, managing memory and a lookup cache.
 *
 * Prototype declaration for callback needed to retrieve a header value during evaluation.
 */
typedef const char *(*http_key_header_t)(void *, const char *, size_t, size_t *);

/**
 * @brief Callback function, for memory allocation during Key header parsing
 *
 * Prototype declaration for callback needed to perform memory allocation. When not explicitly
 * specified, we default to system malloc().
 */
typedef void *(*http_key_malloc_t)(size_t);

/**
 * @brief Callback function, for freeing previously allocate memory
 *
 * Prototype declaration for callback needed to free memory. When not explicitly
 * specified, we default to system free().
 */
typedef void (*http_key_free_t)(void *);

/**
 * @brief Callback function, storing a parsed parameter list in a cache
 *
 * Prototype declaration for the (optional) parameter list cache system. This is used to
 * store previously parsed Key headers in e.g. an LRU type structure. The cache stores
 * are optional to the library, but when used, you must provide both a store and a lookup
 * callback.
 */
typedef void (*http_key_cache_store_t)(void *, const char *, size_t, http_key_params_t);

/**
 * @brief Callback function, looking up a Key in the parameter cache
 *
 * Prototype declaration for the (optional) parameter list cache system. This particular
 * callback is used for subsequent lookups against the cache.
 */
typedef const http_key_params_t (*http_key_cache_lookup_t)(void *, const char *, size_t);

/* ToDo: Should this be opaque as well? If so, we need a constructor wrapper for this? */
typedef struct {
    http_key_header_t get_header;
    http_key_malloc_t malloc;
    http_key_free_t free;
    size_t arena_size;

    /* These are optional */
    struct {
        http_key_cache_store_t store;
        http_key_cache_lookup_t lookup;
        void *data;
    } cache;
} http_key_t;

typedef enum {
    HTTP_KEY_PARSE_OK,
    HTTP_KEY_PARSE_ERROR,
} http_key_parse_status;

/* Public interfaces */
http_key_t *http_key_init(http_key_t *key, http_key_header_t get_header, http_key_malloc_t mem_alloc, http_key_free_t mem_free,
                          size_t arena_size, http_key_cache_store_t cache_store, http_key_cache_lookup_t cache_lookup,
                          void *cache_data);

http_key_parse_status http_key_parse(void *buffer, size_t buffer_size, const char *key_string, size_t key_string_len,
                                     http_key_params_t *params, size_t *num_params);
http_key_parse_status http_key_parse_alloc(http_key_t *key, const char *key_string, size_t key_string_len,
                                           http_key_params_t *params, size_t *num_params);

size_t http_key_eval(http_key_t *http_key, void *header_data, http_key_params_t params, char *buf, size_t buf_size);

void http_key_release(http_key_params_t params);

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
