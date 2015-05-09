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

import sys
import csv

IN_NAME  = 'z80_decoder_tables.csv'
OUT_NAME = 'z80_decoder_tables.h'

LICENSE = '''
// Copyright 2015 Juan Luis Álvarez Martínez
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
'''
PREFIX = '''
#ifndef __Z80_DECODER_TABLES
#define __Z80_DECODER_TABLES
'''

POSTFIX = '''

#endif
'''

# ---------------
# --- Helpers ---
# ---------------

def array_to_c(a, row_size = 8):
  rv = "{\n"
  last = len(a) - 1;
  for i in range(len(a)):
    if (i % row_size) == 0:
      rv = rv + "    "
    rv = rv + str(a[i])
    if i < last:
      rv = rv + ", "
    if (i % row_size)==7:
      rv = rv + "\n"
  rv = rv + "}"
  return rv

# ------------------------
# --- Table generators ---
# ------------------------

# --- Byte 1
#First byte (common to all)
def gen_opcode_1(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

# --- Byte 2
#Byte 2, unprefixed
def gen_opcode_2_unprefixed(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

#byte 2, CB prefix
def gen_opcode_2_CB(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

#byte 2, ED prefix
def gen_opcode_2_ED(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

# --- Byte 3
# byte 3, unprefixed
def gen_opcode_3_unprefixed(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

# byte 3, ED prefix
def gen_opcode_3_ED(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

# --- Byte 4
# byte 4, ED prefix
def gen_opcode_4_ED(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

# byte 4, DDCB/FDCB prefix
def gen_opcode_4_xxCB(f):
  lut = list()
  for i in range(256):
    lut.append(0)
  #Implement-me
  return lut

# ------------
# --- Main ---
# ------------
if __name__ == '__main__':
  rv = 0
  f_in  = open(IN_NAME, 'rb')
  f_out = open(OUT_NAME, 'wb')
  try:
    f_out.write(bytes(LICENSE[1:], 'utf-8'))
    f_out.write(bytes(PREFIX, 'utf-8'))
    #Byte 1
    v = "static const void (*op_b1[256])() = %s;\n" % array_to_c(gen_opcode_1(f_in))
    f_out.write(bytes(v, 'utf-8'))
    #Byte 2
    v = "static const void (*op_b2[256])() = %s;\n" % array_to_c(gen_opcode_2_unprefixed(f_in))
    f_out.write(bytes(v, 'utf-8'))
    v = "static const void (*op_b2_cb[256])() = %s;\n" % array_to_c(gen_opcode_2_CB(f_in))
    f_out.write(bytes(v, 'utf-8'))
    v = "static const void (*op_b2_ed[256])() = %s;\n" % array_to_c(gen_opcode_2_ED(f_in))
    f_out.write(bytes(v, 'utf-8'))
    #Byte 3
    v = "static const void (*op_b3[256])() = %s;\n" % array_to_c(gen_opcode_3_unprefixed(f_in))
    f_out.write(bytes(v, 'utf-8'))
    v = "static const void (*op_b3_ed[256])() = %s;\n" % array_to_c(gen_opcode_3_ED(f_in))
    f_out.write(bytes(v, 'utf-8'))
    #Byte 4
    v = "static const void (*op_b4_ed[256])() = %s;\n" % array_to_c(gen_opcode_4_ED(f_in))
    f_out.write(bytes(v, 'utf-8'))
    v = "static const void (*op_b4_xxcb[256])() = %s;\n" % array_to_c(gen_opcode_4_xxCB(f_in))
    f_out.write(bytes(v, 'utf-8'))
  except Exception as e:
    print("Error!\n%s" % str(e))
    rv = -1
  finally:
    f_in.close()
    f_out.close()
    sys.exit(rv)


if False:
  with open(IN_NAME, newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter='\t', quotechar='"')
    for row in spamreader:
      if row[0][0] == '#':
        continue
      for cell in row:
        print(cell)
      print("--------------")
    
