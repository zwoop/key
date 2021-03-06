#                                               -*- Autoconf -*-
# Process this file with `autoreconf -i` to create a 'configure' file.

#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
m4_define([KEY_VERSION_S],[1.0.0])
m4_define([KEY_VERSION_N],[1000000])

AC_PREREQ([2.69])
AC_INIT([HTTP Key Library], KEY_VERSION_S(), [leif@ogre.com], [key], [https://github.com/zwoop/key])

AC_CONFIG_HEADERS([src/include/key_config.h])
AC_CONFIG_AUX_DIR([build/_aux])
AC_CONFIG_MACRO_DIR([build])

AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability tar-ustar foreign no-installinfo no-installman subdir-objects 1.9.2])
AM_MAINTAINER_MODE([enable])

LT_INIT([disable-shared])

# Library version details
KEY_VERSION_MAJOR=$((KEY_VERSION_N() / 1000000 ))
KEY_VERSION_MINOR=$(((KEY_VERSION_N() / 1000) % 1000 ))
KEY_VERSION_MICRO=$((KEY_VERSION_N() % 1000 ))

KEY_LIBTOOL_VERSION=$KEY_VERSION_MAJOR:$KEY_VERSION_MICRO:$KEY_VERSION_MINOR

AC_SUBST(KEY_LIBTOOL_VERSION)
AC_SUBST(KEY_VERSION_MAJOR)
AC_SUBST(KEY_VERSION_MINOR)
AC_SUBST(KEY_VERSION_MICRO)

TS_CONFIG_NICE([config.nice])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_SED
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_LIBTOOL
# AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stddef.h stdint.h stdlib.h string.h strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memchr memset strchr strdup strncasecmp])

# Do this later, because otherwise the library and function checks can fail oddly (due to e.g. -Werror)
TS_ADDTO(CFLAGS, [-std=c11 -pedantic -Werror -Wall])
TS_ADDTO(CPPFLAGS, [-D_GNU_SOURCE])

# Genereated files
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 cmd/Makefile
                 test/Makefile])

# End
AC_OUTPUT

# Produce a brief summary of how this build is being performed
AC_MSG_NOTICE([Build option summary:
    CC:                 $CC
    CFLAGS:             $CFLAGS
    CPPFLAGS:           $CPPFLAGS
    LDFLAGS:            $LDFLAGS
    LIBTOOL_LINK_FLAGS: $LIBTOOL_LINK_FLAGS
])
