# HTTP Key

This is still very much work in progress, please see the *TODO* section below
for things that still needs work on. The current RFC draft is available at

https://tools.ietf.org/html/draft-ietf-httpbis-key-01


Note: Currently, the PARAM and PARTITION parameters are not implemented!

## Features

This is an attempt to implement a system agnostic runtime system for the HTTP
Key specifications. Some of the features includes:

  * Flexible and efficient memory management.
  * The parsed result is cacheable in itself, allowing to reuse the
    evaluation components for commonly used Key headers.
  * Efficient evaluation of parsed Key headers.

## Contribution

All contributions must be formatted with clang-format v3.9.0. Binaries for
this for some common platforms can be found at

https://bintray.com/apache/trafficserver/clang-format-tools/2016-04-15

## Testing and running from command line

There's a simple command line tool that is part of the build. This can be
run with a simple

    make check

Or, from command line directly

    ./cmd/key-cmd -H "Foo: 12" "Foo;div=3"


## TODO items

As mentioned, there's plenty of stuff still not working here.

* First, look for the string "ToDo" in the source, and fix those!

* Then look at
  * Performance: There are inefficiencies when multiple parameters are used
    for the same header. It would be more efficient if we'd loop over the
    relevant parameter objects for each sub-element of the relevant header.
    This could make e.g. MATCH and PARAM significantly more efficient. 
  * Add a Dump() function for the parameters list, useful for debugging / introspection.
  * We need better / more documentation in the http/key.h filem in doxygen format.
  * Support "" around string values (for the tokenizer), including escaped "'s.
  * Support the lookup / store functions for parsed Key object cache.
  * Finish the missing parsers.
    * PARAMETER parser.
    * PARTITION parser.
  * Finish the evaluators.
    * PARAMETER evaluator.
    * PARTITION evaluator.
  * The are still some error cases that we might not handle well.

## Code Layout

The public interface is in include/http/key.h, you would use it with e.g.

```C
#include <http/key.h>
```

The main library is named *libhttp_key.a*.


  ├── build
  │   └── common.m4
  ├── cmd
  │   ├── key-cmd.c             -- Basic command line tool for testing
  │   └── Makefile.am
  ├── configure.ac
  ├── Doxyfile
  ├── include
  │   └── http
  │       └── key.h             -- This is the public include file
  ├── LICENSE
  ├── Makefile.am
  ├── README.md
  ├── src
  │   ├── arena.c               -- Memory management
  │   ├── evaluators.c
  │   ├── include               -- Include file for the library internals
  │   │   ├── arena.h
  │   │   ├── evaluators.h
  │   │   ├── key_config.h.in   -- autoconf managed and generated includes
  │   │   ├── parameters.h
  │   │   ├── parser.h
  │   │   └── platform.h
  │   ├── key.c                 -- Main entry points for the library
  │   ├── Makefile.am
  │   └── parser.c              -- Parsing the Key header
  └── test                      -- Basic test scripts, using key-cmd
      ├── div.sh
      ├── Makefile.am
      ├── match.sh
      └── substr.sh

## Draft issues

While developing this, a few questions / concerns on the current draft
surfaced. I probably need to file Github issues on some of these, but
documenting them here for now:

  * There's no way to express a "priority set" of supported values in a key
    header. For example, imagine we want to support deflate, gzip, brotli and
    plain (no compression) for Content-Encoding. As far as I can tell, the
    only way to express this as a Key header is

      Key: accept-encoding;substr=deflate, accept-encoding;substr=gzip, accept-encoding;substr=brotli

    However, this can create an explosion of permutations, cache duplication and ambiguity:

    000 - No compression
    100 - deflate
    010 - gzip
    001 - brotli
    011 - gzip (or brotli?)
    110 - deflate (or gzip?)
    ...

    The intent of the draft was to not support Accept-Encoding, which is
    somewhat unfortunate since it's likely the most common Vary: header that
    requires intermediaries to implement complex, hacky code to deal with.

  * 
