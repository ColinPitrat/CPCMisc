#!/bin/bash

cd $(dirname $0)

TOOLSDIR=`pwd`/tools/
INSTALLDIR=`pwd`/tools/install

# TODO: (Re-)install other tools, selectively (re-)install tools based on
# arguments.

mkdir -p ${TOOLSDIR:?}
mkdir -p ${INSTALLDIR:?}/bin

# Install z88dk
cd ${TOOLSDIR:?}
if [ ! -d z88dk ]
then
  git clone https://github.com/z88dk/z88dk.git
  cd z88dk
  git submodule init
  git submodule update
else
  cd z88dk
  git pull
fi
rm src/config.h
./build.sh -C -c -i ${INSTALLDIR:?}
cp bin/* ${INSTALLDIR:?}/bin/
mkdir -p ${INSTALLDIR:?}/share/z88dk/
cp -r lib ${INSTALLDIR:?}/share/z88dk/
cp -r include ${INSTALLDIR:?}/

# Patch for 008_interrupts
cp $(INSTALLDIR)/tools/install/share/z88dk/lib/target/cpc/classic/cpc_crt0.asm $(INSTALLDIR)/tools/install/share/z88dk/lib/target/cpc/classic/cpc_crt0.asm.ori
sed -i 's/^__fw_int_address__/PUBLIC __fw_int_address__/' $(INSTALLDIR)/tools/install/share/z88dk/lib/target/cpc/classic/cpc_crt0.asm
