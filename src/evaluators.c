/** @file

    Implementation for the parameter evaluators.

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
#include <assert.h>
#include "include/platform.h"

#if HAVE_STRING_H
#include <string.h>
#endif

#include "include/parameters.h"
#include "include/evaluators.h"

size_t
key_eval_div(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_div_t *div = (key_param_div_t *)param;

    assert(div->c.type == KEY_PARAM_DIV);

    /* 2.3.1:

       1)  If "parameter_value" is "0", fail parameter processing
           Section 2.2.2.
       2)  If "header_value" is the empty string, return "none".
       3)  If "header_value" contains a ",", remove it and all subsequent
           characters.
       4)  Remove all WSP characters from "header_value".
       5)  If "header_value" does not match the div ABNF rule, fail
           parameter processing (Section 2.2.2).
       6)  Return the quotient of "header_value" / "parameter_value"
           (omitting the modulus).
    */

    return 0;
}

size_t
key_eval_partition(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_partition_t *partition = (key_param_partition_t *)param;

    assert(partition->c.type == KEY_PARAM_PARTITION);

    /* 2.3.2:

       1)  If "header_value" is the empty string, return "none".
       2)  If "header_value" contains a ",", remove it and all subsequent
           characters.
       3)  Remove all WSP characters from "header_value".
       4)  If "header_value" does not match the segment ABNF rule, fail
           parameter processing (Section 2.2.2).
       5)  Let "segment_id" be 0.
       6)  Create a list "segment_list" by splitting "parameter_value" on
           ":" characters.
       7)  For each "segment_value" in "segment_list":
           1)  If "header_value" is less than "segment_value" when they are
               numerically compared, skip to step 7.
           2)  Increment "segment_id" by 1.
       8)  Return "segment_id".
    */

    return 0;
}

size_t
key_eval_match(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_match_t *match = (key_param_match_t *)param;

    assert(match->c.type == KEY_PARAM_MATCH);

    /* 2.3.3:

       1)  If "header_value" is the empty string, return "none".
       2)  Create "header_list" by splitting "header_value" on ","
           characters.
       3)  For each "header_item" in "header_list":
           1)  Remove leading and trailing WSP characters in "header_item".
           2)  If the value of "header_item" is character-for-character
               identical to "parameter_value", return "1".
       4)  Return "0".
    */

    return 0;
}

size_t
key_eval_substr(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_substr_t *substr = (key_param_substr_t *)param;

    assert(substr->c.type == KEY_PARAM_SUBSTR);
    assert(value && (value_len > 0));
    assert(start < buf_size); /* Room for at least a "1" or a "0" */

    /* 2.3.4:

       1)  If "header_value" is the empty string, return "none".
       2)  Create "header_list" by splitting "header_value" on ","
           characters.
       3)  For each "header_item" in "header_list":
           1)  Remove leading and trailing WSP characters in "header_item".
           2)  If the value of "parameter_value" is character-for-character
               present as a substring of "header_value", return "1".
       4)  Return "0".
    */
    if ((value_len >= substr->substr_len) && memmem(value, value_len, substr->substr, substr->substr_len)) {
        *buf = '1';
        return 1;
    }

    *buf = '0';
    return 1;
}

size_t
key_eval_param(key_common_t *param, const char *value, size_t value_len, char *buf, size_t start, size_t buf_size)
{
    key_param_param_t *substr = (key_param_param_t *)param;

    assert(substr->c.type == KEY_PARAM_PARAM);

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
