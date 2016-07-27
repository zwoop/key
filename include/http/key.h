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
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_MAX_PARTITIONS 32

/* Holds one single key parameter "rule", but it's also a linked list. */
typedef struct _key_params {
    void *param;
    struct _key_params *next;
} key_params_t;

/**
 * @brief Callback function, for retrieving a header value, managing memory and a lookup cache.
 *
 * This is a prototype declaration, for declaring the callback need to retrieve a header value,
 * manage various aspects of memory, as well as the optional parser cache.
 */
typedef const char *(key_header_t)(void *, const char *, size_t, size_t *);
typedef void *(key_malloc_t)(size_t);
typedef void(key_free_t)(void *);

typedef void(key_cache_store_t)(void *, const char *, size_t, key_params_t *);
typedef const key_params_t *(key_cache_lookup_t)(void *, const char *, size_t);

typedef struct {
    key_header_t *get_header;
    key_cache_store_t *store_params;
    key_cache_lookup_t *lookup_params;
    key_malloc_t *malloc;
    key_free_t *free;
    void *data;
} key_t;

typedef enum {
    KEY_PARSE_OK,
    KEY_PARSE_FAIL,
} key_parse_t;

key_parse_t key_parse(key_t *key, void *, key_params_t **params, size_t *num_params);
key_parse_t key_parse_string(key_t *key, const char *header, size_t header_len, key_params_t **params, size_t *num_params);

void key_release_params(key_t *key, key_params_t *params);

/* Evaluate the compiled key parameters against a header, return the string lengths, 0 or negative value (errors) */
int key_eval(key_t *key, void *, key_params_t *params, char *buf, size_t buf_size);

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
