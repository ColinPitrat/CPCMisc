export PATH := $(PWD)/../../../../tools/install/bin:$(PATH)

.PHONY: all clean

all:
	python3 ../../tools/build_tests.py build_dir cpc/power_on_low_rom

clean:
	rm -f *.bin *.dsk
