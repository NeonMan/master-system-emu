#!/usr/bin/env python3

# Copyright 2015 Juan Luis Álvarez Martínez
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#Trimming utility.
#Removes a number of bytes from one file and writes the result into another.

import sys

USAGE = '''\
Usage: trim.py INPUT_FILE OUT_FILE TRIM_LEADING_BYTES \
'''

# ------------
# --- Main ---
# ------------
if __name__ == '__main__':
  if len(sys.argv) != 4:
    print(USAGE)
    exit()
  
  path_in   = sys.argv[1]
  path_out  = sys.argv[2]
  trim_size = int(sys.argv[3])
  buff = None
  
  with open(path_in, 'rb') as f:
    buff = f.read()
    f.close()
  
  with open(path_out, 'wb') as f:
    f.write(buff[trim_size:])
    f.close()
