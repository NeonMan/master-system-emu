# Quick and dirty hack to compile under windows

all: test_rla.sms test_rlca.sms print_hello.sms

# -------------
# --- Tests ---
# -------------

# --- RLA test ---
test_rla.link: test_rla.o test_rla.asm
	echo [objects] > $@
	echo $< >> $@
test_rla.o: test_rla.asm ./inc/sdsc_print.inc
	wla-z80 -o $< $@
test_rla.sms: test_rla.link test_rla.o
	wlalink -rsv $< $@

# --- RLCA test ---
test_rlca.link: test_rlca.o test_rlca.asm
	echo [objects] > $@
	echo $< >> $@
test_rlca.o: test_rlca.asm ./inc/sdsc_print.inc
	wla-z80 -o $< $@
test_rlca.sms: test_rlca.link test_rlca.o
	wlalink -rsv $< $@
	
# --- Hello world ---
print_hello.link: print_hello.o print_hello.asm
	echo [objects] > $@
	echo $< >> $@
print_hello.o: print_hello.asm ./inc/sdsc_print.inc
	wla-z80 -o $< $@
print_hello.sms: print_hello.link print_hello.o
	wlalink -rsv $< $@
  
# ----------------------
# --- Helper targets ---
# ----------------------
clean:
	-del *.link
	-del *.o
	-del *.sms
	-del *.sym
  