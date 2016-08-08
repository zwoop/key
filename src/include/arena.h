/** @file

    Include file for the Arena object and linked list manager.
    Note: This is not a generic memory arena manager, it's specific
    just for holding one or more Key evaluator objects.

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
#ifndef ARENA_H
#define ARENA_H

#include "http/key.h"
#include "include/key_config.h"

/* ToDo: This might be x64 specific? But regardless, hardcoded to 16 byte alignments for now. */
#define KEY_ARENA_ALIGN(p) (((p) + (16 - 1L)) & ~(16 - 1L))

/* Thsi holds an arena, which is a sequence of Key parameter objects and strings */
typedef struct {
    key_t *key;
    size_t size;
    size_t pos;
} key_arena_t;

key_arena_t *key_arena_create(key_t *key, size_t size);
void key_arena_destroy(key_arena_t *arena);

void *key_arena_allocate(key_arena_t *arena, size_t size);

#endif /* ARENA_H */

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
