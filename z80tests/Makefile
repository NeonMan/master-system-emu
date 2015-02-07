# Quick and dirty hack to compile under windows

all: test_shift.sms hello_world.sms nop_loop.sms sdsc_hello.sms print_hello.sms

# -------------
# --- Tests ---
# -------------

# --- Bit shifting ---
test_shift.link: test_shift.o test_shift.asm
	echo [objects] > $@
	echo $< >> $@
test_shift.o: test_shift.asm 
	wla-z80 -o $< $@
test_shift.sms: test_shift.link test_shift.o
	wlalink -rsv $< $@






# --- Maxim's hello world.
hello_world.link: hello_world.o hello_world.asm
	echo [objects] > $@
	echo $< >> $@
hello_world.o: hello_world.asm 
	wla-z80 -o $< $@
hello_world.sms: hello_world.link hello_world.o
	wlalink -rsv $< $@
	
# --- Nop loop.
nop_loop.link: nop_loop.o nop_loop.asm
	echo [objects] > $@
	echo $< >> $@
nop_loop.o: nop_loop.asm 
	wla-z80 -o $< $@
nop_loop.sms: nop_loop.link nop_loop.o
	wlalink -rsv $< $@
	
# --- SDSC Hello World.
sdsc_hello.link: sdsc_hello.o sdsc_hello.asm
	echo [objects] > $@
	echo $< >> $@
sdsc_hello.o: sdsc_hello.asm
	wla-z80 -o $< $@
sdsc_hello.sms: sdsc_hello.link sdsc_hello.o
	wlalink -rsv $< $@
	
# --- Printf Hello world
print_hello.link: print_hello.o print_hello.asm
	echo [objects] > $@
	echo $< >> $@
print_hello.o: print_hello.asm sdsc_print.inc
	wla-z80 -o $< $@
print_hello.sms: print_hello.link print_hello.o
	wlalink -rsv $< $@
	
# --- Helper targets
clean:
	-del *.link
	-del *.o
	-del *.sms
	-del *.sym