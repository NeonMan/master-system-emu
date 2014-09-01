exec: psgplay testrom

all: exec samples

psgplay: deps
	clang -o $@ psg-play/psg-play.o psg/psg.o
	
testrom: deps
	clang -o $@ test-rom/test-rom.o rom/rom.o

deps:
	$(MAKE) -C psg-play
	$(MAKE) -C psg
	$(MAKE) -C rom
	$(MAKE) -C ram
	$(MAKE) -C test-rom
	$(MAKE) -C z80

clean:
	-$(MAKE) -C psg-play clean
	-$(MAKE) -C psg      clean
	-$(MAKE) -C rom      clean
	-$(MAKE) -C ram      clean
	-$(MAKE) -C test-rom clean
	-$(MAKE) -C z80      clean
	-rm psgplay
	-rm testrom
	-rm *.wav


# Samples
samples: psg-sample

psg-sample: psgplay
	./psgplay | ffmpeg -ar 22050 -ac 1 -f s16be -i - psg-sample-22050.wav
