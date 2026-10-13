# Assembly programs for the Amstrad CPC / Plus

## 001_hello_world

The most basic program revisited.

## 002_colors/

Usage of display routine:
 - set mode
 - writing text
 - positioning cursor
 - waiting keypress

## 003_interrupts

Playing with interrupts.

## 004_keyboard

A program that identifies whether the CPC it runs on has an AZERTY or QWERTY
keyboard.

## 005_sound

Playing with sounds.

## 006_cap32_bin_load_test

A test for [Caprice32](https://github.com/ColinPitrat/caprice32) integration
tests suite.

This is used to validate that loading a program at a given address works as
expected, even when loaded in lower RAM.

For this reason, the program doesn't use any firmware routine.
