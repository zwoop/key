/** @file

    Simple command line utility to test and validate various Key
    header strings, showing how the parser handled the header. This
    is used as part of regression tests, but can also be useful when
    debugging and testing the Key header properties in general.

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
#include <stdio.h>
#include <assert.h>
#include <search.h>
#include <getopt.h>
#include <ctype.h>

#include "http/key.h"

#include "include/platform.h"

#if HAVE__BOOL
#include <stdbool.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#define KEY_HASH_SIZE 8192 /* Arbitrary number of headers */

/* Some globals. Sue me, this is just a demo / test tool */
key_t *g_key;

const char *
get_header(void *data, const char *header, size_t header_len, size_t *value_len)
{
    ENTRY item;
    ENTRY *found_item;

    item.key = (char *)header;
    if ((found_item = hsearch(item, FIND)) != NULL) {
        return (const char *)found_item->data;
    }

    return NULL;
}

static void
init(void)
{
    g_key = key_init(NULL,        /* Have it allocate memory for us, using the allocator */
                     &get_header, /* Header function */
                     NULL,        /* Use system malloc */
                     NULL,        /* Use system free */
                     4096,        /* Arbitrary arena size, which also dictates roughly the size of Key */
                     NULL,        /* No cache store */
                     NULL,        /* No cache lookup */
                     NULL         /* No cache data */
                     );
    assert(g_key);
}

static void
cleanup(void)
{
    if (g_key) {
        key_release(g_key); /* Only release memory that was allocated by the Key allocator!!! */
    }
}

static void
help()
{
    fprintf(stderr, "Usage: key-cmd [-H header] [-h] <Key string> ...\n");
    fprintf(stderr, "\t-H <header>	Set the header (e.g. 'Accept-Encoding: gzip')\n");
    exit(0);
}

static void
add_header(const char *h)
{
    char *header = key_strdup(h);
    char *sep = key_strchr(header, ':');

    if (sep) {
        *sep++ = '\0';
        while (*sep && isspace(*sep)) {
            ++sep;
        }
        if (*sep) {
            ENTRY item;

            item.key = header;
            item.data = sep;
            (void)hsearch(item, ENTER);

            return;
        }
    }

    /* Something went wrong parsing the header */
    free(header);
    fprintf(stderr, "error: %s is not a valid option to -H\n\n", h);
    help();
}

int
main(int argc, const char *argv[])
{
    static const struct option longopt[] = {
        {(char *)"header", required_argument, NULL, 'H'}, {(char *)"help", no_argument, NULL, 'h'}, {NULL, no_argument, NULL, '\0'},
    };

    /* Parse the command line arguments */
    hcreate(KEY_HASH_SIZE);
    while (1) {
        int opt = getopt_long(argc, (char *const *)argv, "hH:", longopt, NULL);

        switch (opt) {
            case 'H':
                add_header(optarg);
                break;
            case 'h':
                help();
                break;
        }

        if (opt == -1) {
            break;
        }
    }

    init();

    cleanup();

    hdestroy();

    return 0;
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
