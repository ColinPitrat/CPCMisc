disc_motor_on:
ld bc,$fa7e
ld a,1
out (c),a
ld b,30
fdc_motor_wait:
halt
djnz fdc_motor_wait
ret

fdc_write_command:
ld bc,&fb7e
push af
fwc1:
in a,(c)
add a,a
jr nc,fwc1
add a,a
jr nc,fwc2
pop af
ret
fwc2:
pop af
inc c
out (c),a
dec c
ld a,5
fwc3:
dec a
jr nz,fwc3
ret

fdc_result:
ld hl,result_data
ld bc,&fb7e
fr1:
in a,(c)
cp &c0
jr c,fr1
inc c
in a,(c)
dec c
ld (hl),a
inc hl
ld a,5
fr2:
dec a
jr nz,fr2
in a,(c)
and &10
jr nz,fr1
ret

fdc_seek:
ld a,%00001111
call fdc_write_command
ld a,(drive)
call fdc_write_command
ld a,(track)
call fdc_write_command
call fdc_seek_or_recalibrate
jp nz,fdc_seek
ret

fdc_recalibrate:
ld a,%0111
call fdc_write_command
ld a,(drive)
call fdc_write_command
call fdc_seek_or_recalibrate
jp nz,fdc_recalibrate
ret

fdc_seek_or_recalibrate:
ld a,%1000
call fdc_write_command
call fdc_result
ld ix,result_data
bit 5,(ix+0)
jr z,fdc_seek_or_recalibrate
bit 4,(ix+0)
ret

result_data:
defs 8
