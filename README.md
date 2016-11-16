# HTTP Key

This is still very much work in progress, please see the *TODO* file for things
that still needs work on.

## Contribution

All contributions must be formatted with clang-format v3.9.0. Binaries for
this for some common platforms can be found at

    https://bintray.com/apache/trafficserver/clang-format-tools/2016-04-15


## Code Layout

The public interface is in include/http/key.h, you would use it with e.g.

```C
#include <http/key.h>
```

The main library is named *libhttpkey.a*.


> ├── cmd
> │   ├── key-cmd.c
> │   └── Makefile.am
> ├── configure.ac
> ├── include
> │   └── http
> │       └── key.h
> ├── Makefile.am
> ├── src
> │   ├── arena.c
> │   ├── evaluators.c
> │   ├── include
> │   │   ├── arena.h
> │   │   ├── evaluators.h
> │   │   ├── key_config.h.in
> │   │   ├── parameters.h
> │   │   ├── parser.h
> │   │   └── platform.h
> │   ├── key.c
> │   ├── Makefile.am
> │   └── parser.c
> ├── test
> │   ├── div.sh
> │   ├── Makefile.am
> │   ├── match.sh
> │   └── substr.sh
> └── TODO
