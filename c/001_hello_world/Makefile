export PATH := $(PWD)/../../tools/install/bin:$(PATH)

ORG_HEX:=6000
ORG_DEC:=`echo -e "ibase=16\n$(ORG_HEX)" | bc`

all:
	zcc +cpc -lndos -zorg=$(ORG_DEC) -o hello.bin hello.c
	iDSK hello.dsk -n
	iDSK hello.dsk -i hello.bin -t 1 -e $(ORG_HEX) -c $(ORG_HEX)

run: all
	cap32 -i hello.bin -o 0x$(ORG_HEX)

run_dsk: all
	cap32 -a "run\"hello" hello.dsk

clean:
	rm -f *.bin *.dsk
