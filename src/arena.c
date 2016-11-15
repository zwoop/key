/** @file

    The arena structure is an efficient wrapper around both memory
    allocation, each arena holding a linked list of Key parameter
    evaluators. This means this Arena is not a generic memory allocator,
    it's specific to hold one or more Key parameters only.

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
#include <stdio.h> /* ToDo: Remove? */

#include "include/arena.h"

key_arena_t *
p_key_arena_create(key_t *key)
{
    if (key) {
        key_arena_t *arena = (key_arena_t *)key->malloc(key->arena_size);

        if (arena) {
            arena->size = key->arena_size;
            arena->pos = KEY_ARENA_ALIGN(sizeof(key_arena_t));

            return arena;
        }
        return NULL; /* Out of memory, the caller has to decide what to do */
    }

    assert(!"Must provide a Key object (key_t*) for the Arena");
}

void
p_key_arena_destroy(key_t *key, key_arena_t *arena)
{
    assert(key);
    assert(arena);

    key->free(arena);
}

void *
p_key_arena_allocate(key_arena_t *arena, size_t size)
{
    if (size <= (arena->pos - arena->size)) {
        void *memory = (void *)((unsigned char *)arena + arena->pos);

        arena->pos = KEY_ARENA_ALIGN(arena->pos + size);
        return memory;
    }
    return NULL;
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
