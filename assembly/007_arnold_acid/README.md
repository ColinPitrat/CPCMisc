# 007_arnold_acid - Amstrad CPC "Acid" Test Suite

This directory contains the assembly test suite from Kevin Thacker's **Arnold "Acid" Test** for Amstrad CPC, CPC+, KC Compact, and Aleste 520EX emulators, as advertised on the CPCWiki forum:
https://cpcwiki.eu/forum/emulators/amstrad-cpc-'acid'-test/

The links from this forum post are dead but I could recover the zip thanks to the wayback machine.

Thanks a million to the creator of these tests which are awesome, and for the internet archive project to have preserved them!

This is my attempt of making these tests build with my modern setup (pasmo + iDSK) on linux.

At this point, things build but I'm far from having tested everything and some things are definitely not working as they are intended too (keyboard tests for example definitely seem to have a glitch).

## Building

From this directory (`CPCMisc/assembly/007_arnold_acid`), run:

```bash
make
```

To cleanup all generated `.bin`, `.dsk`, and `.cpr` files:

```bash
make clean
```

You can also run `make` or `make clean` inside individual test suite subdirectories (e.g. `make -C crtc`, `make -C plus`, `make -C z80tests`).

### Requirements

- `pasmo`
- `iDSK`
- `python3`

## Test Categories

| Directory | Generated Disk / Cartridge Images | Description |
| :--- | :--- | :--- |
| `aleste/` | `aleste.dsk` | Aleste 520EX video modes, palette, mapper, and RAM tests (not tested, I'm not using those personally) |
| `cpc/` | `cpc.dsk`, `mode3/mode3.dsk`, `power_on_low_rom/lowrom.dsk`, `power_on_low_rom/visual/poweron.dsk` | Standard CPC Gate Array, border, pen, mode 3, HBLANK/VBLANK, and power-on tests |
| `cpcrom/` | `cpcrom.dsk` | CPC ROM banking and selection tests |
| `cpctests/` | `cpctests.dsk`, `flood/flood2.dsk`, `images/patterns.dsk` | Gate Array interrupts, HSYNC/VSYNC timing, IM2, and raster tests |
| `crtc/` | `crtc1.dsk`, `crtc2.dsk`, `intlace/intlace.dsk`, `intsync/intsync.dsk` | CRTC (Types 0, 1, 2, 3, 4) register, status, skew, interlace, and sync tests |
| `crtc_detect/` | `crtctype.dsk` | CRTC type detection routines |
| `devices/` | `amram2/amram2.dsk`, `brunwordmk2/brunwordmk2.dsk`, `csd/csdtest.dsk`, `csd/dummycart.cpr`, `dkram/dkram.dsk`, `dobbertin/dobbertin.dsk`, `ide/ide.dsk`, `inicronram/inicron.dsk`, `kempston/kempston.dsk`, `megaflash/megaflash.dsk`, `mf2/mf2.dsk`, `multiplay/multiplay.dsk`, `playcity/playcity.dsk`, `rombo/rombo.dsk`, `sym2/sym2.dsk`, `vortexram/vortexram.dsk`, `vortexwd20/vortexwd20.dsk`, `xmem/xmem.dsk`, `yarek4mb/yarek4mb.dsk` | Peripheral and expansion hardware tests (RAM expansions, Multiface 2, PlayCity, Symbiface II, IDE, etc.) |
| `disc/` | `drivetest/drivetest.dsk`, `fdc/fdc.dsk`, `fdc_*/cpc_fdctest.dsk`, `fdctest3/cpc_fdctest.dsk`, `morefdctest/cpc_fdctest.dsk`, `fdc_poweron/cart.cpr` | NEC uPD765A FDC and floppy drive controller tests |
| `dissassembler/` | `opcodes.dsk` | Z80 instruction disassembler test |
| `examples/` | `examples.dsk` | 8K screen and double-buffering (`flipscreens`) examples |
| `kccompact/` | `kcc.dsk` | KC Compact Z8536 CIO, mode 3, border, and VBLANK tests (not tested, I'm not using those personally) |
| `keyboard/` | `keyb.dsk` | Keyboard matrix scanning tests (seem broken at the time of writing) |
| `monitor/` | `monitor.dsk`, `safezone/safezone.dsk` | Monitor horizontal/vertical range and safezone tests |
| `plus/` | `asic1.dsk`, `asic2.dsk`, `power_on_test_cart/cart.cpr`, `power_on_test_cart/visual/poweron.cpr` | CPC+ ASIC unlocking, DMA, programmable raster interrupts (PRI), hardware sprites, split screen, scrolling, and power-on cartridge tests |
| `ppi/` | `ppi.dsk` | Intel 8255 PPI port control, mode, and VSYNC/audio tests |
| `psg/` | `psg.dsk` | AY-3-8912 PSG register and I/O port tests |
| `rom/` | `short*.bin`, `norm*.bin`, `long*.bin`, `rom.bin` | Expansion ROM header and size test images |
| `snapshot/` | `snapshot.dsk` | Emulator snapshot state helper programs |
| `z80tests/` | `cpu.dsk`, `woody/z80tests/z80tests.dsk`, `patrik_z80test/cpc/z80test-1.0/z80*.dsk` | Z80 CPU instruction, flag, MEMPTR, I/O, and interrupt tests |

## What I tested in details

So far, I've not look in details to much things.

I'm mostly interested in the Plus functionalities at the moment, though I do want to look into CPC tests too later.

I'm also mostly interested in the pass/fail tests and want to make them integrable in caprice32 automated testing.

The things I'm focusing on:

### asic1.dsk

Running the following binaries:
 - `test`
 - `lock`
 - `rom`
 - `dmatest`
 - `pritest`
 - `ppi`
 - `extnram`
 - `afterlck`
 - `hscrl`
 - `scrlr8`
 - `scrlmid`
 - `split`
 - `splits`
 - `splt`
 - `dmatime`
 - `pritrig`
 - `priack`
 - `pridelay`

### asic2.dsk

Running the following binaries:
 - `vscrl`
 - `vscrlr9`
 - `sprmagmr`
 - `dmatime2`

### cart.cpr
