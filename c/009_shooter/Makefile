export PATH := $(PWD)/../../tools/install/bin:$(PATH)
export ZCCCFG := $(realpath $(PWD)/../../tools/install/share/z88dk/lib/config/)

INCPATH:=$(PWD)/../../tools/install/include/
ORG_HEX:=4000
ORG_DEC:=`echo -e "ibase=16\n$(ORG_HEX)" | bc`

PROGRAM:=shooter

all:
	zcc +cpc -lndos -lm -zorg=$(ORG_DEC) -I$(INCPATH) -o $(PROGRAM).bin $(PROGRAM).c -m
	iDSK $(PROGRAM).dsk -n
	iDSK $(PROGRAM).dsk -i $(PROGRAM).bin -t 1 -e $(ORG_HEX) -c $(ORG_HEX)
	# Generate symfile
	# Disassemble from entry point
	#echo 'd $$4000' > symfile
	# Disassemble and breakpoint from draw_player
	echo 'd $$4542' > symfile
	echo 'b $$4542' >> symfile
	echo 'd $$43B2' >> symfile
	echo 'b $$43B2' >> symfile
	echo 'd $$4979' >> symfile
	echo 'b $$4979' >> symfile
	# Disassemble and breakpoint from cpc_TestKey
	#echo 'd $$484D' > symfile
	#echo 'b $$484D' >> symfile
	# Disassemble and breakpoint from joystickState
	#echo 'd $$4630' >> symfile
	#echo 'b $$4630' >> symfile
	# Extract symbols from mapfile
	awk '{ print "al ",$$3, "."$$1 }' $(PROGRAM).map | grep -v '\$$[^ ]\{5\}' >> symfile
	# Addresses are usually more interesting. Caprice takes the last label in priority.
	grep addr $(PROGRAM).map | awk '{ print "al ",$$3, "."$$1 }' | grep -v '\$$[^ ]\{5\}' >> symfile

asm:
	zcc +cpc -lndos -lm -zorg=$(ORG_DEC) -I$(INCPATH) -S -o $(PROGRAM).asm $(PROGRAM).c

run: all
	cap32 -O system.joystick_emulation=1 -i $(PROGRAM).bin -o 0x$(ORG_HEX)

run_dsk: all
	cap32 -O system.joystick_emulation=1 -a "run\"$(PROGRAM)" $(PROGRAM).dsk

run_debug: all
	cap32 -O system.joystick_emulation=1 -i $(PROGRAM).bin -o 0x$(ORG_HEX) -s symfile

clean:
	rm -f *.bin *.dsk *.map symfile
