@echo off

call clean-samples.bat

rem --------------------
rem --- Build crt0.s ---
rem --------------------

echo Building crt0
sdasz80 -g -l -o -s crt0-sms.s

rem ------------------
rem --- Build libs ---
rem ------------------

echo Building libs
sdcc -mz80 --std-c99 -c sdsc.c
sdcc -mz80 --std-c99 -c emu-hooks.c

rem ---------------------
rem --- Build samples ---
rem ---------------------

echo Building samples
sdcc -mz80 --std-c99 --data-loc 0xc000 --no-std-crt0 crt0-sms.rel sdsc.rel emu-hooks.rel sdcc-sample.c
makebin sdcc-sample.ihx sdcc-sample.bin

echo Done

pause