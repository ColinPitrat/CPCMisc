export PATH := $(PWD)/../../../tools/install/bin:$(PATH)

.PHONY: all clean

all:
	python3 ../tools/build_tests.py build_dir snapshot

clean:
	rm -f *.bin *.dsk
