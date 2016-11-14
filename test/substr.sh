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

[ "1,1" != $($CMD -H "Abc: bennet" "Abc;substr=bennet") ] && exit -1
[ "1,1" != $($CMD -H "Abc: foo, bennet" "Abc;substr=bennet") ] && exit -1
[ "1,1" != $($CMD -H "Abc: abennet00" "Abc;substr=bennet") ] && exit -1
[ "1,1" != $($CMD -H "Abc: bar, 99bennet     , abc" "Abc;substr=bennet") ] && exit -1
# ToDo: This should work too
#[ "1,1" != $($CMD -H "Abc: "bennet"" "Abc;substr=bennet") ] && exit -1

[ "0,1" != $($CMD -H "Abc: theodore" "Abc;substr=bennet") ] && exit -1
[ "0,1" != $($CMD -H "Abc: joe, sam" "Abc;substr=bennet") ] && exit -1
[ "0,1" != $($CMD -H "Abc: Bennet" "Abc;substr=bennet") ] && exit -1
[ "0,1" != $($CMD -H "Abc: Ben net" "Abc;substr=bennet") ] && exit -1

# Some more complex tests, not from RFC
UA="User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)"
[ "1110,4" != $($CMD -H "$UA" "user-agent;substr=MSIE;substr=Windows,user-agent;substr=9.0;substr=Safari") ] && exit -1

exit 0
