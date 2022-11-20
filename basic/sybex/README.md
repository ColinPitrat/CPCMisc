# Sybex

Listing downloaded from https://cpcrulez.fr/book_sybex-jeux_en_assembleur.htm

Can be typed in the emulator with:

```
iDSK -n sybex.dsk
cap32 -O system.limit_speed=0 -a '$(for file in *.bas; do cat $file; echo "save\"$file"; echo "new"; done)' sybex.dsk
```

Then don't forget to save the dsk before quitting.
