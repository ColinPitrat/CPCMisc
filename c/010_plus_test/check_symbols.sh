#!/bin/bash
expected_symbols=1
bss_symbols=$(grep "\$8" plustest.map | grep -v "^__" | wc -l)

if [[ $bss_symbols -ne $expected_symbols ]]
then
  echo "Some symbols are in BSS that shouldn't be (got '$bss_symbols', want '$expected_symbols')"
  grep "\$8" plustest.map | grep -v "^__"
  exit 1
fi
