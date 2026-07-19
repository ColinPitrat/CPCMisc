# C programs for the Amstrad CPC / Plus

## 001_hello_world

The most basic program revisited.

## 002_fp_quadratic_equation

A utility using floating points.

## 003_pascal_triangle

An example of recurrence, long integers and number formatting.

## 004_mandelbrot

The Mandelbrot set for the CPC.

## 005_graphics

Some graphics (putpixel, fillrect) in mode 0.

## 006_starfield

A starfield in mode 0.

## 007_sound

Playing sounds using env, ent and sound equivalents.

## 008_interrupts

An example of using raster interrupts.
This relies on a hack in z88dk and needs to be revisited to use z88dk's interrupt support:
https://www.z88dk.org/forum/viewtopic.php?t=11705

Broken by latest z88dk releases.

## 009_shooter

A simple shoot'em up game.

## 010_plus_test

An example of creating a cartridge for the plus, doing everything from scratch.
This requires:
 - initializing the hardware
 - handling interrupts
 - defining our own charmap (well, technically stealing it from the CPC ROM)
 - writing our own methods for the most basic stuff (printing text, numbers, char, ...)
 - handling the nightmare of positioning things properly in memory
