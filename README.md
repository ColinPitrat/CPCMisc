# CPCMisc

Various projects of coding for the Amstrad CPC

## Content

### basic/advanced

Advanced (or contrived, or obfuscated) basic examples.

### basic/sybex

Jeux provenant du livre "Amstrad Jeux en Assembleur" d'Eric Ravis:
https://www.cpcwiki.eu/index.php/Amstrad_Jeux_en_Assembleur

## Installing tools

```
TOOLSDIR=`pwd`/tools/
INSTALLDIR=`pwd`/tools/install
mkdir -p ${TOOLSDIR:?}
mkdir -p ${INSTALLDIR:?}/bin
```

### iDSK

```
cd ${TOOLSDIR:?}
git clone https://github.com/jeromelesaux/idsk.git
cd idsk
cmake .
make
cp iDSK ${INSTALLDIR:?}/bin/
```

### pasmo

```
cd ${TOOLSDIR:?}
wget http://pasmo.speccy.org/bin/pasmo-0.5.4.beta2.tgz
tar -xvzf pasmo-0.5.4.beta2.tgz
cd pasmo-0.5.4.beta2
./configure --prefix=${INSTALLDIR:?}
make
make install
```

### z88dk

```
cd ${TOOLSDIR:?}
git clone https://github.com/z88dk/z88dk.git
cd z88dk
git submodule init
git submodule update
rm src/config.h
./build.sh -i ${INSTALLDIR:?}
cp bin/* ${INSTALLDIR:?}/bin/
mkdir -p ${INSTALLDIR:?}/share/z88dk/
cp -r lib ${INSTALLDIR:?}/share/z88dk/
cp -r include ${INSTALLDIR:?}/
```

### cap32

```
cd ${TOOLSDIR:?}
git clone https://github.com/ColinPitrat/caprice32.git
cd caprice32
make DESTDIR=${INSTALLDIR:?} prefix="" install
```

### TRSE

Debug build and allow core with `ulimit` to investigate in case of crash.

```
cd ${TOOLSDIR:?}
git clone https://github.com/leuat/TRSE.git
cd TRSE
qmake TRSE.pro
make -j 10
ln -s Publish/tutorials tutorials
ln -s Publish/source/themes themes
# Don't create the project_templates because it's already there
mkdir bin
mv trse bin/
./bin/trse
```
