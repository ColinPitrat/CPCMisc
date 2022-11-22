# User Manual

Games from the Amstrad CPC 6128 Plus user manual, French version, with bug fixes.

Can be typed in the emulator with:

```
iDSK -n user_manual.dsk
cap32 -O system.limit_speed=0 -a '$(for file in *.bas; do cat $file; echo "save\"$file"; echo "new"; done)' user_manual.dsk
```

Then don't forget to save the dsk before quitting.
