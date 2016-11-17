/** @file

    Simple command line utility to test and validate various Key
    header strings, showing how the parser handled the header. This
    is used as part of regression tests, but can also be useful when
    debugging and testing the Key header properties in general.

    This is not a great example of how to use this library, but it
    can be useful for testing (including our regression tests).

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
#define KEY_HEADER_TABLE_SIZE 256

/* Produce help text, from command line parsing etc. */
static void
help()
{
    fprintf(stderr, "Usage: key-cmd [-H header] [-h] <Key string> ...\n");
    fprintf(stderr, "\t-H <header>	Set the header (e.g. 'Accept-Encoding: gzip')\n");
    exit(0);
}

/* Manage our header lookup table */
typedef struct _key_header {
    char *header;
    char *value;
    size_t value_len;
    struct _key_header *next;
} key_header_t;

static key_header_t *header_table[KEY_HEADER_TABLE_SIZE]; /* One entry for each header length */

/* Clear out all the malloced entries / values from the header table */
static void
clear_header_table()
{
    for (int i = 0; i < KEY_HEADER_TABLE_SIZE; ++i) {
        key_header_t *entry = header_table[i];

        while (entry) {
            key_header_t *e = entry;

            free(entry->header);
            free(entry->value);
            entry = entry->next;
            free(e);
        }
    }
}

/* Retrieve a header from the hash, this is not particularly efficient, but it's a demo / test tool. */
static const char *
get_header(void *data, const char *header, size_t header_len, size_t *value_len)
{
    key_header_t *slot = header_table[header_len];

    while (slot) {
        if (!strncasecmp(slot->header, header, header_len)) {
            *value_len = slot->value_len;
            return slot->value;
        }
        slot = slot->next;
    }

    *value_len = 0;

    return NULL;
}

static void
add_header(const char *header_val)
{
    char *sep = key_strchr(header_val, ':');

    if (sep) {
        *sep++ = '\0';
        while (*sep && isspace(*sep)) {
            ++sep;
        }

        if (*sep) {
            size_t header_len = strlen(header_val);

            if (header_len < KEY_HEADER_TABLE_SIZE) {
                key_header_t *entry = malloc(sizeof(key_header_t));

                memset(entry, 0, sizeof(key_header_t));
                entry->header = strdup(header_val);
                entry->value = strdup(sep);
                entry->value_len = strlen(sep);

                if (!header_table[header_len]) {
                    header_table[header_len] = entry;
                } else {
                    key_header_t *slot = header_table[header_len];

                    while (slot->next) {
                        slot = slot->next;
                    }
                    slot->next = entry;
                }

                return;
            }
        }
    }

    /* Something went wrong parsing the header */
    fprintf(stderr, "error: %s is not a valid option to -H\n\n", header_val);
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

    /* Initialize the header table */
    memset(header_table, 0, sizeof(header_table));

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
            http_key_release_params(params);
        } else {
            /* ToDo: Parse failure */
        }
    }

    clear_header_table();

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
