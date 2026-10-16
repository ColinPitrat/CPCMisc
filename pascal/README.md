Programs written for Hisoft Pascal.

Example of how to load, compile and run a program with Caprice32:

```
cap32 path_to/Hisoft-Pascal.dsk -a 'run"disc' -a CAP32_DELAY -a CPC_RETURN \
    $(printf ' -a CAP32_DELAY%.0s' {1..7}) -a "$(cat hanoi.pas)" -a "C"
```
