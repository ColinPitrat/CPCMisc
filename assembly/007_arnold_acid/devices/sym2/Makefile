export PATH := $(PWD)/../../../../tools/install/bin:$(PATH)

.PHONY: all clean

all:
	python3 ../../tools/build_tests.py build_dir devices/sym2

clean:
	rm -f *.bin *.dsk
