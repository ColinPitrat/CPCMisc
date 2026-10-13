org #6000

di

; Overwrite the interrupt handler to prevent firmware from re-enaling ROM
ld hl,&0038
ld (hl), &fb  ; EI
inc hl
ld (hl), &c9  ; RET

ei

; Fill the screen
ld hl, &c000
ld (hl), &ff
ld d, h
ld e, l
inc de
ld bc, &3fff
ldir

loop:
jr loop
