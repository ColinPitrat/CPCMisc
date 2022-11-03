export PATH := $(PWD)/../../tools/install/bin:$(PATH)

INCPATH:=$(PWD)/../../tools/install/include/
ORG_HEX:=4000
ORG_DEC:=`echo -e "ibase=16\n$(ORG_HEX)" | bc`

all:
	zcc +cpc -lndos -zorg=$(ORG_DEC) -I$(INCPATH) -o hello.bin hello.c -m
	iDSK hello.dsk -n
	iDSK hello.dsk -i hello.bin -t 1 -e $(ORG_HEX) -c $(ORG_HEX)
	# Generate symfile
	# Disassemble from entry point
	echo 'd $$4000' > symfile
	# Some interesting breakpoints
	# Entry point
	#echo 'b $$4003' >> symfile
	# main
	#echo 'b $$4030' >> symfile
	# cpc_SetTxtCursorPos
	#echo 'b $$4187' >> symfile
	# Extract symbols from mapfile
	awk '{ print "al ",$$3, "."$$1 }' hello.map | grep -v '\$$[^ ]\{5\}' >> symfile
	# Addresses are usually more interesting. Caprice takes the last label in priority.
	grep addr hello.map | awk '{ print "al ",$$3, "."$$1 }' | grep -v '\$$[^ ]\{5\}' >> symfile

run: all
	cap32 -i hello.bin -o 0x$(ORG_HEX)

run_dsk: all
	cap32 -a "run\"hello" hello.dsk

run_debug: all
	cap32 -i hello.bin -o 0x$(ORG_HEX) -s symfile

clean:
	rm -f *.bin *.dsk
