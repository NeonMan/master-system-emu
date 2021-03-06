# Sega Master System Emulator #

A yet to be named emulator of the Master System console by SEGA. This project
aims to create an accurate emulator under a permissive free software license.

## Current state ##

The Master System is composed of various circuits, each one of them is
emulated independently.

| Module        | Progress        | Comments                                            |
| ------------- |:---------------:| --------------------------------------------------- |
| BIOS          | Working         | Requires a rework to remove copy/pasted ROM code.   |
| FM            | Unimplemented   | YM2413 FM chip on JAP region only, not a priority.  |
| IO            | Complete        |                                                     |
| Peripheral    | WIP             | GPIO and input are ignored.                         |
| PSG           | Complete        | Complete emulation, must test for accuracy.         |
| RAM           | Complete        |                                                     |
| RAM (cart)    | Unimplemented   |                                                     |
| ROM (SEGA)    | Working         | SEGA mapper roms.                                   |
| ROM (other)   | Unimplemented   | Other mappers (Codemasters, unlicensed, custom).    |
| SDSC          | Working         | Simple SDSC console callbacks provided.             |
| VDP           | WIP             | Missing interrupts, counters, mode 4, sprite system.|
| z80           | Working/WIP     | Missing: minor LD/IO ops; INT modes 0 and 2.        |
| GUI           | WIP             | Implements a basic z80 Debugger.                    |
| Savestate     | Working/WIP     | Not tested wether all state is stored/retrieved.    |

## Known issues ##

Issues listed here describe inaccurate behaviours and inefficiencies in implemented code.

- Read-modify-writeback instructions like RLC (IX + d) redo all the work multiple times.
- ROM and BIOS share mapper, might be expected?

## Licensing ##

This project is licensed under the [Apache License] Version 2.0 that in essence allows 
you to use this project for any purpose, even on proprietary software, with very few
restrictions.

   [Apache License]:     http://www.apache.org/licenses/LICENSE-2.0