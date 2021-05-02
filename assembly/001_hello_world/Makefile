export PATH := $(PWD)/../../tools/install/bin:$(PATH)

all:
	pasmo hello.asm hello.bin
	pasmo hello2.asm hello2.bin
	iDSK hello.dsk -n
	iDSK hello.dsk -i hello.bin -t 1 -e 6000 -c 6000
	iDSK hello.dsk -i hello2.bin -t 1 -e 6000 -c 6000
