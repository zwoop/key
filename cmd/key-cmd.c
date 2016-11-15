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

#if HAVE_STRING_H
#include <string.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#define KEY_HASH_SIZE 8192 /* Arbitrary number of headers */

/* Produce help text, from command line parsing etc. */
static void
help()
{
    fprintf(stderr, "Usage: key-cmd [-H header] [-h] <Key string> ...\n");
    fprintf(stderr, "\t-H <header>	Set the header (e.g. 'Accept-Encoding: gzip')\n");
    exit(0);
}

/* Retrieve a header from the hash, this is not particularly efficient, but it's a demo / test tool. */
static const char *
get_header(void *data, const char *header, size_t header_len, size_t *value_len)
{
    ENTRY item;
    ENTRY *found_item;

    item.key = (char *)header;
    if ((found_item = hsearch(item, FIND)) != NULL) {
        *value_len = strlen((const char *)found_item->data);
        return (const char *)found_item->data;
    }

    *value_len = 0;

    return NULL;
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
            char *h = header;

            /* Store the header value in all lower-case */
            while (*h) {
                *h = tolower(*h);
                ++h;
            }
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
    http_key_t key;
    int i;
    int terse = 0;

    /* getopt() options */
    static const struct option longopt[] = {
        {(char *)"header", required_argument, NULL, 'H'}, {(char *)"help", no_argument, NULL, 'h'}, {NULL, no_argument, NULL, '\0'},
    };

    /* Setup the main key object */
    http_key_init(&key, &get_header, /* Header function */
                  NULL,              /* Use system malloc */
                  NULL,              /* Use system free */
                  4096,              /* Arbitrary arena size, which also dictates roughly the size of Key */
                  NULL,              /* No cache store */
                  NULL,              /* No cache lookup */
                  NULL               /* No cache data */
                  );

    /* We use the posix hash table for the header lookups */
    hcreate(KEY_HASH_SIZE);

    /* Parse the command line arguments */
    while (1) {
        int opt = getopt_long(argc, (char *const *)argv, "hH:t", longopt, NULL);

        switch (opt) {
            case 'H':
                add_header(optarg);
                break;
            case 't':
                terse = 1;
                break;
            case 'h':
                help();
                break;
        }

        if (opt == -1) {
            break;
        }
    }

    argc -= optind;
    argv += optind;

    /* ToDo: It'd be neat to have a way to do e.g.

       curl -s -D - -o /dev/null https://example.com | key-cmd "accept-encoding;substr=gzip".

       -- or --

       key-cmd -u https://example.com "accept-encoding;substr=gzip".
    */

    /* Loop over the remaining arguments, and parse those as if they were Key: headers */
    for (i = 0; i < argc; ++i) {
        http_key_params_t params;
        size_t num_params;
        unsigned char arena[8192];
        char buf[8192];

        if (HTTP_KEY_PARSE_OK ==
            http_key_parse_buffer((void *)arena, sizeof(arena), argv[i], strlen(argv[i]), &params, &num_params)) {
            size_t len = http_key_eval(&key, NULL, params, buf, sizeof(buf) - 1);

            if (terse) {
                printf("%.*s,%d\n", (int)len, buf, (int)len);
            } else {
                printf("\tKey: %s -> \"%.*s\"\n", argv[i], (int)len, buf);
            }
            http_key_release_params(&key, params);
        } else {
            /* ToDo: Parse failure */
        }
    }

    /* No need to release our Key object, since it's on the stack. */
    hdestroy(); /* Destroy the posix hash table */

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
