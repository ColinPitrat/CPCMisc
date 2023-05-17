org #6000
TXT_OUTPUT    EQU #BB5A
KM_WAIT_CHAR    EQU #BB06

start:
                           ; in lower ROM may be called.
       ld bc,&7f89         ; use MODE 1 for graphics and enable lower ROM.
       out (c),c
       
       ld hl,&1d69         ; key character table starts at &1d69 on a CPC464.
       ld a,(hl)           ; get byte at this address.
       cp &f0              ; first key in table should be character &f0.
       jr z,azert0         ; we're using a CPC464 - go to keyboard detection
                           ; routine.
       ld hl,&1eef         ; table starts at &1eef on all other CPCs.

azert0:
       ld de,67            ; set address of key 67.
       add hl,de
       ld a,(hl)           ; get character for this key.
       cp 'a'              ; is an AZERTY keyboard being used?
       jr z,azerty       ; yes.
       ld bc,&7f8d
       out (c),c           ; disable lower ROM.

qwerty:
       ld hl,qwerty_str
       call print_str
       jr end_prog

azerty:
       ld hl,azerty_str
       call print_str

end_prog:
       call KM_WAIT_CHAR
       ret

print_str:
       ld a,(hl)
       cp 0
       ret z
       inc hl
       call TXT_OUTPUT
       jr print_str

qwerty_str: defm "QWERTY"
            defb 0

azerty_str: defm "AZERTY"
            defb 0
