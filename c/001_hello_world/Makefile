export PATH := $(PWD)/../../tools/install/bin:$(PATH)

# TODO(ColinPitrat): This doesn't work yet. Find why ...
all:
	zcc +cpc -lndos -o hello.bin hello.c
	iDSK hello.dsk -n
	iDSK hello.dsk -i hello.bin -t 1 -e 6000 -c 6000
