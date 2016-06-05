# Sega Master System Emulator #

A yet to be named emulator of the Master System console by SEGA. This project
aims to create an accurate emulator under a permissive free software license.

## Current state ##

The Master System is composed of various circuits, each one of them is
emulated independently.

| Module        | Progress        | Comments                                            |
| ------------- |:---------------:| --------------------------------------------------- |
| FM            | Unimplemented   | JAP region only, not a priority                     |
| IO            | Complete        |                                                     |
| Peripherial   | WIP             | GPIO and input are ignored                          |
| PSG           | Complete        | Complete emulation, must test for accuracy          |
| RAM           | Complete        |                                                     |
| ROM (SEGA)    | Working         | SEGA mapper roms                                    |
| ROM (other)   | Unimplemented   | Other mappers (Codemasters, unlicensed, custom)     |
| SDSC          | Working         | Simple SDSC console callbacks provided              |
| VDP           | Working/WIP     | Missing interrupts, counters, mode 4, sprite system |
| z80           | Working/WIP     | i8080 subset mostly implemented, missing interrupts |
| GUI           | WIP             | Implements a basic z80 Debugger                     |

## Known issues ##

Issues listed here describe inaccurate behaviours and inefficiencies in implemented code.

* Most opcodes that modify flags, fail the ZEXALL/ZEXDOC tests.
* Read-modify-writeback instructions like RLC (IX + d) redo all the work multiple times.
* ALU16 flags are internally using ALU8 macros which might or might not work.

## Licensing ##

This project is licensed under the 
[Apache License Version 2.0](http://www.apache.org/licenses/LICENSE-2.0). In 
essence you can use this project, even on proprietary software, with very few
restrictions.
