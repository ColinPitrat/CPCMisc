org #6000
TXT_OUTPUT    EQU #BB5A
KM_WAIT_CHAR  EQU #BB06

; This program demonstrates how to access PSG registers to play sounds
start:
       ; Set envelope frequency to 4000 (0xFA0: period of ~1s)
       ld c,&0c
       ld a,&0f
       call write_to_psg
       ld c,&0b
       ld a,&a0
       call write_to_psg

       ; Set volume to 'envelope' (0x10) on channel A
       ld c,8
       ld a,&10
       call write_to_psg

       ; Activate tone on channel A
       ld c,7
       ld a,62
       call write_to_psg

       ; Set tone to 142 (0x8E) which is ~440 Hz
       ld c,0
       ld a,&8e
       call write_to_psg
       ld c,1
       ld a,0
       call write_to_psg

       ld d,0
loop:
       ld hl, message
       call print_str
       ; Print the number of the envelope
       ; TODO: Properly handle a>=10
       ld a, d
       cp 10
       jp c, units
       ld a, &31
       call TXT_OUTPUT
       ld a, d
       sub 10
units:
       add a,&30
       call TXT_OUTPUT
       ; Line feed
       ld a, &d
       call TXT_OUTPUT
       ; Carriage return
       ld a, &a
       call TXT_OUTPUT
       ; Set envelope to d
       ld c,&0d
       ld a,d
       call write_to_psg
       call KM_WAIT_CHAR
       inc d
       ld a,d
       cp 16
       jr nz,loop


end_prog:
       ret

print_str:
       ld a,(hl)
       cp 0
       ret z
       inc hl
       call TXT_OUTPUT
       jr print_str

write_to_psg:
       ld b,&f4         ; setup PSG register number on PPI port A
       out (c),c

       ld bc,&f6c0      ; tell PSG to select register
       out (c),c

       ld bc,&f600      ; put PSG function selection into inactive state
       out (c),c

       ld b,&f4         ; write data to PPI port A
       out (c),a

       ld bc,&f680      ; tell PSG to read data
       out (c),c

       ld bc,&f600      ; put PSG function selection into inactive state
       out (c),c
       ret

message:
       defm "Envelope: "
       defb 0
