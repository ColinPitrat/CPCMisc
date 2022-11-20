# Astounding

Games from the book "Astounding Arcade Games For Your AMSTRAD" with bug fixes
and minor improvements.

Can be typed in the emulator with:

```
iDSK -n astounding.dsk
cap32 -O system.limit_speed=0 -a '$(for file in *.bas; do cat $file; echo "save\"$file"; echo "new"; done)' astounding.dsk
```

Then don't forget to save the dsk before quitting.
