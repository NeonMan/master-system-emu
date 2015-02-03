# Quick and dirty hack to compile under windows

all: hello_world.sms nop_loop.sms sdsc_hello.sms

# --- Maxim's hello world.
hello_world.link:
	echo [objects] > $@
hello_world.o: hello_world.asm 
	wla-z80 -o $< $@
	echo $@ >> hello_world.link
hello_world.sms: hello_world.link hello_world.o
	wlalink -rsv $< $@
	
# --- Nop loop.
nop_loop.link:
	echo [objects] > $@
nop_loop.o: nop_loop.asm 
	wla-z80 -o $< $@
	echo $@ >> nop_loop.link
nop_loop.sms: nop_loop.link nop_loop.o
	wlalink -rsv $< $@
	
# --- SDSC Hello World.
sdsc_hello.link:
	echo [objects] > $@
sdsc_hello.o: sdsc_hello.asm 
	wla-z80 -o $< $@
	echo $@ >> sdsc_hello.link
sdsc_hello.sms: sdsc_hello.link sdsc_hello.o
	wlalink -rsv $< $@
	
# --- Helper targets ---
cleanlink:
	-del *.link

clean: cleanlink
	-del *.o
	-del *.sms
	-del *.sym
	