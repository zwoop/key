/** @file

    The parsing and factory APIs, for parsing and creating the underlying
    parameter types for later evaluations.

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
#ifndef PARSER_H
#define PARSER_H

#include "include/platform.h"

#if HAVE_INTTYPES_H
#include <inttypes.h>
#endif

size_t key_strsep(const char *value, size_t value_len, const char **start, const char **next, const char separator);
uint64_t key_memtoll(const char *str, size_t len);

#endif /* PARSER_H */

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
