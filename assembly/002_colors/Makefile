export PATH := $(PWD)/../../tools/install/bin:$(PATH)

PROGRAM:=colors

all:
	pasmo $(PROGRAM).asm $(PROGRAM).bin
	iDSK $(PROGRAM).dsk -n
	iDSK $(PROGRAM).dsk -i $(PROGRAM).bin -t 1 -e 6000 -c 6000

run: all
	cap32 -i $(PROGRAM).bin -o 0x6000

run_dsk: all
	cap32 -a "run\"$(PROGRAM)" $(PROGRAM).dsk

clean:
	rm -f *.bin *.dsk
