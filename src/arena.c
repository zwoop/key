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
#include "include/arena.h"

#if HAVE_STRING_H
#include <string.h>
#endif

#define ALIGN(p) (((p) + (16-1L)) & ~(16-1L))

key_arena_t*
key_arena_create(key_t *key, size_t size)
{
    void *memory = key->malloc(size);
    key_arena_t *arena = (key_arena_t *)memory;

    arena->memory = memory;
    arena->key = key;
    arena->size = size;
    arena->pos = ALIGN(sizeof(key_arena_t));

    return arena;
}

void
key_arena_destroy(key_arena_t *arena)
{
    arena->key->free(arena);
}

void*
key_arena_allocate(key_arena_t *arena, size_t size)
{
    if (size <= (arena->pos - arena->size)) {
        void *memory = (void*)(arena->memory + arena->pos);

        arena->pos = ALIGN(arena->pos + size);
        return memory;
    }
    return NULL;
}

void *
key_arena_memdup(key_arena_t *arena, void *src, size_t size)
{
    void *ptr = key_arena_allocate(arena, size);

    if (ptr) {
        memcpy(ptr, src, size);
    }

    return ptr;
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
