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

// --------------------------------------------------
// ---          DO NOT ALTER THIS FILE!!          ---
// --------------------------------------------------
// --- Decoder tables are automatically generated ---
// --- by 'z80_make_decoder_tables.py'            ---
// --------------------------------------------------

#include "z80_opcodes.h"

struct opcode_dec_s{
  void (*f)();
  signed char size;
};
typedef struct opcode_dec_s opcode_dec_t;

'''

POSTFIX = '''

#endif
'''

EMPTY_OPCODE = "{0,0}"

class Opcode:
  def __str__(self):
    return "{%s,%d}" % (self.function, self.size/8)
  def __init__(self, name, function, mask, pattern, size):
    self.name = name
    self.function = function
    self.size = size
    self.mask = []
    self.pattern = []
    for i in range(0,self.size,8):
      self.mask.insert(0, (mask>>(i))&0xFF)
      self.pattern.insert(0, (pattern>>(i))&0xFF)


# ---------------
# --- Helpers ---
# ---------------

def array_to_c(a, row_size = 8):
  rv = "{\n"
  rv = rv + "/*    % 17s  % 17s  % 17s  % 17s  % 17s  % 17s  % 17s  % 17s*/\n" % (0,1,2,3,4,5,6,7)
  last = len(a) - 1;
  for i in range(len(a)):
    if (i % row_size) == 0:
      rv = rv + "/*%02X*/" % i
    rv = rv + ("% 17s" % str(a[i]))
    if i < last:
      rv = rv + ", "
    if (i % row_size)==7:
      rv = rv + "\n"
  rv = rv + "}"
  return rv

def count_bits(n, bits):
  rv = 0
  for i in range(bits):
    if ((n>>i) & 0x01) == 0x01:
      rv = rv + 1
  return rv

# ------------------------
# --- Table generators ---
# ------------------------

#Unprefixed opcodes
def gen_opcode_1(ops):
  lut = list()
  #Create a 256 element lut
  for i in range(256):
    lut.append(EMPTY_OPCODE)
  #Set the one-byte (8bit) opcodes
  for i in range(256):
    for op in ops:
      #Prune prefixed opcodes
      if (op.pattern[0] == 0xCB) or (op.pattern[0] == 0xDD) or (op.pattern[0] == 0xED) or (op.pattern[0] == 0xFD):
        continue
      #Insert op on matching spots
      if (i & op.mask[0]) == (op.pattern[0] & 0xFF):
        if lut[i] == EMPTY_OPCODE:
          lut[i] = op
        else:
          if count_bits(lut[i].mask[0], 8) < count_bits(op.mask[0], 8):
            lut[i] = op
          else:
            print("WARNING: Overlap with same bitmask len: %s <%s>, %s <%s>" % (lut[i].name, lut[i].function, op.name, op.function))
  return lut

#XX prefix
def gen_opcode_xx(ops, pref):
  lut = list()
  #Create a 256 element lut
  for i in range(256):
    lut.append(EMPTY_OPCODE)
  #Set the xx prefixed opcodes
  for i in range(256):
    for op in ops:
      #prune unprefixed/other prefix opcode
      if op.pattern[0] != pref:
        continue
      #prune DDCB/DDED/FDCB/FDED
      if ((op.pattern[0] == 0xDD) or (op.pattern[0] == 0xFD)) and ((op.pattern[1] == 0xCB) or (op.pattern[1] == 0xED)):
        continue
      #insert op on matching spots
      if (i & op.mask[1]) == (op.pattern[1] & 0xFF):
        if lut[i] == EMPTY_OPCODE:
          lut[i] = op
        else:
          if count_bits(lut[i].mask[1], 8) < count_bits(op.mask[1], 8):
            lut[i] = op
          else:
            print("WARNING: Overlap with same bitmask len: %s <%s>, %s <%s>" % (lut[i].name, lut[i].function, op.name, op.function))
  return lut

#XXCB prefix
def gen_opcode_xxcb(ops, pref):
  lut = list()
  #Create a 256 element lut
  for i in range(256):
    lut.append(EMPTY_OPCODE)
  #set xxCB prefixed opcodes
  for i in range(256):
    for op in ops:
      #prune unprefixed/other prefix opcodes
      if (op.pattern[0] != pref) or (op.pattern[1] != 0xCB):
        continue
      #insert matching ops
      if (i & op.mask[3]) == (op.pattern[3] & 0xFF):
        if lut[i] == EMPTY_OPCODE:
          lut[i] = op
        else:
          if count_bits(lut[i].mask[3], 8) < count_bits(op.mask[3], 8):
            lut[i] = op
          if count_bits(lut[i].mask[3], 8) == count_bits(op.mask[3], 8):
            print("WARNING: Overlap with same bitmask len: %s <%s>, %s <%s>" % (lut[i].name, lut[i].function, op.name, op.function))
          else:
            pass
  return lut

# ------------
# --- Main ---
# ------------
if __name__ == '__main__':
  rv = 0
  f_in  = open(IN_NAME, 'r')
  ops = []
  #Parse CSV file
  try:
    csvr = csv.reader(f_in, delimiter='\t', quotechar='"')
    for row in csvr:
      #discard lines starting with '#'
      if row[0][0] == '#':
        continue
      name = row[0]
      function = row[1]
      size = len(row[2]) - 2
      #Mask is all the non '?' bits set to one
      s = row[2][2:]
      s = s.replace('0', '1')
      s = s.replace('?', '0')
      mask = int(s, 2)
      #Pattern is the bit string as-is.
      #Replace '?' with '0' so it can be converted to int
      s = row[2][2:]
      s = s.replace('?', '0')
      pattern = int(s, 2)
      #Check for size correctness
      if(size % 8) != 0:
        raise Exception("Opcode pattern length must be a multiple of 8")
      ops.append(Opcode(name, function, mask, pattern, size))
  except Exception as e:
    print("Error while parsing CSV file.")
    print(str(e))
    rv = -1
  finally:
    f_in.close()

  #Make header
  f_out = open(OUT_NAME, 'wb')
  try:
    f_out.write(bytes(LICENSE[1:], 'utf-8'))
    f_out.write(bytes(PREFIX, 'utf-8'))
    #--- Unprefixed opcodes ---
    #Unprefixed
    v = "static const opcode_dec_t op_unpref[256] = %s;\n" % array_to_c(gen_opcode_1(ops))
    f_out.write(bytes(v, 'utf-8'))
    #CB prefix
    v = "static const opcode_dec_t op_cb[256] = %s;\n" % array_to_c(gen_opcode_xx(ops, 0xCB))
    f_out.write(bytes(v, 'utf-8'))
    #ED prefix
    v = "static const opcode_dec_t op_ed[256] = %s;\n" % array_to_c(gen_opcode_xx(ops, 0xed))
    f_out.write(bytes(v, 'utf-8'))
    #DD prefix
    v = "static const opcode_dec_t op_dd[256] = %s;\n" % array_to_c(gen_opcode_xx(ops, 0xdd))
    f_out.write(bytes(v, 'utf-8'))
    #FD prefix
    v = "static const opcode_dec_t op_fd[256] = %s;\n" % array_to_c(gen_opcode_xx(ops, 0xfd))
    f_out.write(bytes(v, 'utf-8'))

    #DDCB prefix
    v = "static const opcode_dec_t op_ddcb[256] = %s;\n" % array_to_c(gen_opcode_xxcb(ops, 0xdd))
    f_out.write(bytes(v, 'utf-8'))
    #FDCB prefix
    v = "static const opcode_dec_t op_fdcb[256] = %s;\n" % array_to_c(gen_opcode_xxcb(ops, 0xfd))
    f_out.write(bytes(v, 'utf-8'))
    
  except Exception as e:
    print("Error!\n%s" % str(e))
    rv = -1
  finally:
    f_out.close()
  sys.exit(rv)

