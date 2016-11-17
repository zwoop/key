# HTTP Key

This is still very much work in progress, please see the *TODO* section below
for things that still needs work on. The current RFC draft is available at

https://tools.ietf.org/html/draft-ietf-httpbis-key-01

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

The main library is named *libhttpkey.a*.


    ├── cmd
    │   ├── key-cmd.c
    │   └── Makefile.am
    ├── configure.ac
    ├── include
    │   └── http
    │       └── key.h
    ├── Makefile.am
    ├── src
    │   ├── arena.c
    │   ├── evaluators.c
    │   ├── include
    │   │   ├── arena.h
    │   │   ├── evaluators.h
    │   │   ├── key_config.h.in
    │   │   ├── parameters.h
    │   │   ├── parser.h
    │   │   └── platform.h
    │   ├── key.c
    │   ├── Makefile.am
    │   └── parser.c
    ├── test
    │   ├── div.sh
    │   ├── Makefile.am
    │   ├── match.sh
    │   └── substr.sh
    └── TODO
