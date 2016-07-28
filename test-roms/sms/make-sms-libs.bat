@echo off
echo Building crt0
sdasz80 -g -l -o -s crt0-sms.rel crt0-sms.s

echo Building SMS libs

echo - IO
sdcc -mz80 --std-c99 -c io.c

echo - SDSC
sdcc -mz80 --std-c99 -c sdsc.c

echo - Emu hooks
sdcc -mz80 --std-c99 -c emu-hooks.c
