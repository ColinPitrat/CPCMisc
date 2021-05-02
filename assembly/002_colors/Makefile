export PATH := $(PWD)/../../tools/install/bin:$(PATH)

all:
	pasmo colors.asm colors.bin
	iDSK colors.dsk -n
	iDSK colors.dsk -i colors.bin -t 1 -e 6000 -c 6000
