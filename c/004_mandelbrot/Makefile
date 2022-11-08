export PATH := $(PWD)/../../tools/install/bin:$(PATH)

INCPATH:=$(PWD)/../../tools/install/include/
ORG_HEX:=4000
ORG_DEC:=`echo -e "ibase=16\n$(ORG_HEX)" | bc`

PROGRAM:=mandelbrot

all:
	zcc +cpc -lndos -lm -zorg=$(ORG_DEC) -I$(INCPATH) -o $(PROGRAM).bin $(PROGRAM).c -m
	iDSK $(PROGRAM).dsk -n
	iDSK $(PROGRAM).dsk -i $(PROGRAM).bin -t 1 -e $(ORG_HEX) -c $(ORG_HEX)
	# Generate symfile
	# Disassemble from entry point
	echo 'd $$4000' > symfile
	# Some interesting breakpoints
	# Entry point
	echo 'b $$4003' >> symfile
	# main
	echo 'b $$41A0' >> symfile
	# double_assign
	echo 'b $$418A' >> symfile
	# cpc_SetTxtCursorPos
	#echo 'b $$4187' >> symfile
	# Extract symbols from mapfile
	awk '{ print "al ",$$3, "."$$1 }' $(PROGRAM).map | grep -v '\$$[^ ]\{5\}' >> symfile
	# Addresses are usually more interesting. Caprice takes the last label in priority.
	grep addr $(PROGRAM).map | awk '{ print "al ",$$3, "."$$1 }' | grep -v '\$$[^ ]\{5\}' >> symfile

asm:
	zcc +cpc -lndos -lm -zorg=$(ORG_DEC) -I$(INCPATH) -S -o $(PROGRAM).asm $(PROGRAM).c

run: all
	cap32 -i $(PROGRAM).bin -o 0x$(ORG_HEX)

run_dsk: all
	cap32 -a "run\"$(PROGRAM)" $(PROGRAM).dsk

run_debug: all
	cap32 -i $(PROGRAM).bin -o 0x$(ORG_HEX) -s symfile

clean:
	rm -f *.bin *.dsk *.map symfile
