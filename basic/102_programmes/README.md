# 102 programmes pour Amstrad

https://archive.org/details/102-programmes-pour-amstradacme/page/n5/mode/2up

Can be typed in the emulator with:

```
iDSK -n 102_programmes.dsk
cap32 -O system.limit_speed=0 -a '$(for file in *.bas; do cat $file; echo "save\"$file"; echo "new"; done)' 102_programmes.dsk
```

Then don't forget to save the dsk before quitting.
