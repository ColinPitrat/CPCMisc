export PATH := $(PWD)/../../tools/install/bin:$(PATH)

all:
	pasmo colors.asm colors.bin
	iDSK colors.dsk -n
	iDSK colors.dsk -i colors.bin -t 1 -e 6000 -c 6000

run: all
	cap32 -i colors.bin -o 0x6000

run_dsk: all
	cap32 -a "run\"colors" colors.dsk

clean:
	rm -f *.bin *.dsk
