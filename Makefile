exec: psgplay

all: exec samples

psgplay: deps
	clang -o $@ psg-play/psg-play.o psg/psg.o

deps:
	$(MAKE) -C psg-play
	$(MAKE) -C psg

clean:
	-$(MAKE) -C psg-play clean
	-$(MAKE) -C psg      clean
	-rm psgplay
	-rm *.wav


# Samples
samples: psg-sample

psg-sample: psgplay
	./psgplay | ffmpeg -ar 22050 -ac 1 -f s16be -i - psg-sample-22050.wav
