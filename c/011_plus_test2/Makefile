export PATH := $(PWD)/../../tools/install/bin:$(PATH)
export ZCCCFG := $(realpath $(PWD)/../../tools/install/share/z88dk/lib/config/)

INCPATH:=$(PWD)/../../tools/install/include/
ORG_HEX:=0000
ORG_DEC:=`echo -e "ibase=16\n$(ORG_HEX)" | bc`

PROGRAM:=plustest

CAPRICE_OPTIONS:=-O system.model=3

all:
	zcc +cpc -subtype=cart -create-app -m -o $(PROGRAM) $(PROGRAM).c
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
