@echo off

rem ------------------
rem --- Build libs ---
rem ------------------
cd sms
call make-sms-libs.bat
cd ..

rem ---------------------
rem --- Build samples ---
rem ---------------------
echo Building samples
sdcc -mz80 --std-c99 --xram-size 0x4000 --code-loc 0x0090 --data-loc 0xc000 --no-std-crt0 sms/crt0-sms.rel sms/sdsc.rel sms/emu-hooks.rel sdcc-sample.c
makebin -p sdcc-sample.ihx sdcc-sample.bin

sdcc -mz80 --std-c99 --xram-size 0x4000 --code-loc 0x0090 --data-loc 0xc000 --no-std-crt0 sms/crt0-sms.rel empty.c
makebin -p empty.ihx empty.bin

echo Done

pause