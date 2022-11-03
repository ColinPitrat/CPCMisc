export PATH := $(PWD)/../../tools/install/bin:$(PATH)

all:
	pasmo hello.asm hello.bin
	pasmo hello2.asm hello2.bin
	iDSK hello.dsk -n
	iDSK hello.dsk -i hello.bin -t 1 -e 6000 -c 6000
	iDSK hello.dsk -i hello2.bin -t 1 -e 6000 -c 6000

run: all
	cap32 -i hello.bin -o 0x6000

run_dsk: all
	cap32 -a "run\"hello" hello.dsk

clean:
	rm -f *.bin *.dsk
