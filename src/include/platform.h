/** @file

    Basic include file for various platform specific details.

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
#ifndef KEY_PLATFORM_H
#define KEY_PLATFORM_H

#include "include/key_config.h"

#if !HAVE_MEMSET
#error Need memset(), file a ticket for this platform!
#endif

#if !HAVE_MEMCHR
#error Need memchr(), file a ticket for this platform!
#endif

#if !HAVE_STRCHR
#error Need strchr(), file a ticket for this platform!
#endif

#if !HAVE_STRDUP
#error Need strdup(), file a ticket for this platform!
#endif

#if !HAVE_STRNCASECMP
#error Need strncasecmp(), file a ticket for this platform!
#endif

#if !HAVE_MALLOC
#error Need malloc() and free(), file a ticket for this platform!
#endif

#endif /* KEY_PLATFORM_H */

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
