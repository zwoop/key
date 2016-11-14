#! /usr/bin/env bash
#
# Test cases for the div parameter
#
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

set -e # exit on error

CMD="../cmd/key-cmd -t"

# All in group 0
[ "0,1" != $($CMD -H "Bar: 1" "Bar;div=5") ] && exit -1
[ "0,1" != $($CMD -H "Bar: 3 , 42" "Bar;div=5") ] && exit -1
[ "0,1" != $($CMD -H "Bar: 4, " "Bar;div=5") ] && exit -1

# Not in group 0
[ "0,1" == $($CMD -H "Bar: 6, " "Bar;div=5") ] && exit -1
[ "0,1" == $($CMD -H "Bar: 50, " "Bar;div=5") ] && exit -1

# All in group 2
[ "2,1" != $($CMD -H "Bar: 12" "Bar;div=5") ] && exit -1
[ "2,1" != $($CMD -H "Bar: 10" "Bar;div=5") ] && exit -1
[ "2,1" != $($CMD -H "Bar: 14, 1" "Bar;div=5") ] && exit -1

# All in group 10
[ "10,2" != $($CMD -H "Bar: 54" "Bar;div=5") ] && exit -1
[ "10,2" != $($CMD -H "Bar:   52  , 100" "Bar;div=5") ] && exit -1

exit 0
