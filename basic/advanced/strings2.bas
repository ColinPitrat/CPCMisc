5 REM Fills the screen with snowflakes
10 cls
15 REM Reserves 22 bytes for our assembly routine
20 x$=string$(22," ")
25 REM Our compiled assembly routine
30 data &21,&01,&01,&CD,&75,&BB,&01,&E8,&03
40 data &C5,&3E,&EE,&CD,&5D,&BB,&C1,&0B,&79
50 data &B0,&20,&F4,&C9
55 REM Poke our routine in memory
60 pk=@x$
70 ad=peek(pk+2)*256+peek(pk+1)
80 for i = ad to ad+len(x$)-1
90 read t
100 poke i,t
110 next i
115 REM Call our routine
120 call ad
125 REM Wait a bit
130 for i = 0 to 1000:next

REM 21 01 01       - ld hl $0101
REM CD 75 BB       - call $bb75 ; TXT_SET_CURSOR -> set cursor to (1,1)
REM 01 E8 03       - ld bc $03e8 ; 1003
REM loop:
REM C5             - push bc
REM 3E EE          - ld a, $ee ; 238
REM CD 5D BB       - call $bb5d ; TXT_WR_CHAR
REM C1             - pop bc
REM 0B             - dec bc
REM 79             - ld a,c
REM B0             - or b
REM 20 F4          - jr nz,$f4 ; -12 -> loop)
REM C9             - ret
