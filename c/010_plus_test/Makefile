export PATH := $(PWD)/../../tools/install/bin:$(PATH)

INCPATH:=$(PWD)/../../tools/install/include/
ORG_HEX:=8000
ORG_DEC:=`echo -e "ibase=16\n$(ORG_HEX)" | bc`

PROGRAM:=plustest

CAPRICE_OPTIONS:=-O system.model=3

all:
	zcc +embedded --no-crt -m -o $(PROGRAM) $(PROGRAM).c
	cpr2bin.py --to-cpr $(PROGRAM) $(PROGRAM).cpr
	# Generate symfile
	# Disassemble from entry point
	echo 'd $$0000' > symfile
	# Extract symbols from mapfile
	awk '{ print "al ",$$3, "."$$1 }' $(PROGRAM).map | grep -v '\$$[^ ]\{5\}' >> symfile
	# Addresses are usually more interesting. Caprice takes the last label in priority.
	grep addr $(PROGRAM).map | awk '{ print "al ",$$3, "."$$1 }' | grep -v '\$$[^ ]\{5\}' >> symfile

asm:
	# --c-code-in-asm is an option, but very verbose
	zcc +embedded --no-crt -m -S -o $(PROGRAM).asm $(PROGRAM).c

run: all
	cap32 -s=symfile $(CAPRICE_OPTIONS) $(PROGRAM).cpr

clean:
	rm -f *.o *.bin *.dsk *.map symfile
