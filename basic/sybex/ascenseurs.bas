10 REM ************
20 REM *ASCENSEURS*
30 REM ************
40 MODE 1
50 GOSUB 660
60 MEMORY 15000
70 BORDER 0
80 INK 0,0:INK 1,18:INK 2,2:INK 3,6
90 RESTORE 180
100 FOR T=0 TO 5
110 FOR I=0 TO 7
120 READ A
130 IF T>3 THEN B=A AND 240:C=(A AND 15)*16 ELSE B=(A AND 240)/16:C=A AND 15
140 POKE 39168+T*16+I,B
150 POKE 39176+T*16+I,C
160 NEXT I
170 NEXT T
180 DATA 7,7,7,7,1,1,127,127
190 DATA 224,224,224,224,128,128,254,254
200 DATA 1,1,6,6,24,24,24,24
210 DATA 128,128,96,96,24,24,24,24
220 DATA 255,255,255,255,0,0,0,0
230 DATA 0,0,0,0,0,0,0,0
240 CLS:SC=0:X=0:Y=12:TE=0
250 CO=240:SE=2
260 POKE 34998,Y:POKE 34999,X
270 FOR T=0 TO 27
280 POKE 35000+T*3,5+INT(RND(1)*6)+(T MOD 2)*10
290 POKE 35001+T*3,2+INT(T/2)*2-4*(T>9)-4*(T>17)
300 POKE 35002+T*3,((T-(T MOD 2))/2) MOD 2
310 NEXT T
320 PEN 1
330 A$=CHR$(143)+CHR$(143)
340 LOCATE 1,14:PRINT A$
350 LOCATE 39,14:PRINT A$
360 LOCATE 13,14:PRINT A$;A$
370 LOCATE 25,14:PRINT A$;A$
380 GOSUB 610
390 CALL 40067
400 Y=PEEK(34998)
410 TE=PEEK(34997)
420 IF Y<4 OR Y>22 THEN 530
430 A=X:N=0
440 IF INKEY(9)=0 THEN X=X+SE:FOR T=1 TO 100-N:NEXT T:GOTO 460
450 N=N+1:IF N<20 THEN 440
460 IF X=A THEN 390
470 IF TE=1 THEN X=A:GOTO 390
480 IF X=-2 OR X=40 THEN SE=-SE:X=A+SE
490 SC=SC+1:GOSUB 610
500 TE=1:CALL 40024
510 SOUND 1,200
520 POKE 34999,X:GOTO 390
530 FOR T=0 TO 3
540 SOUND 1,2000:CALL 40031
550 FOR I=1 TO 200:NEXT I
560 SOUND 1,1000:CALL 40024
570 FOR I=1 TO 200:NEXT I
580 NEXT T
590 LOCATE 13,5:PRINT "APPUYER SUR 'f0'"
600 IF INKEY(15)=-1 THEN 600 ELSE 240
610 PEN 3
620 LOCATE 11,1:PRINT RIGHT$(STR$(10000+SC),4)
630 IF SC>RE THEN RE=SC
640 LOCATE 27,1:PRINT RIGHT$(STR$(10000+RE),4)
650 RETURN
660 RESTORE 730
670 FOR T=40000 TO 40247
680 READ A
690 POKE T,A
700 NEXT T
710 RETURN
720 REM *AFFICHE
730 DATA 22,153,205,73,156,205,73,156,201,229,1,8,8,26,119,28,124,129,103,16
740 DATA 248,225,35,201
750 REM *SUITE
760 DATA 17,80,80,205,102,156,201,17,32,0,205,102,156,201,42,182,136,205,26,188
770 DATA 213,205,64,156,205,64,156,42,182,136,45,205,26,188,209,90,205,64,156,205
780 DATA 64,156,201,205,95,156,33,184,136,34,160,140,6,28,197,42,160,140,17,136
790 DATA 144,1,3,0,237,176,42,136,144,205,26,188,30,80,205,64,156,30,80,205
800 DATA 64,156,58,138,144,254,1,40,18,58,136,144,254,2,32,5,62,23,50,136
810 DATA 144,33,136,144,53,24,16,58,136,144,254,22,32,5,62,1,50,136,144,33
820 DATA 136,144,52,42,136,144,205,26,188,30,64,205,64,156,30,64,205,64,156,33
830 DATA 136,144,237,91,160,140,1,3,0,237,176,237,83,160,140,193,16,152,62,0
840 DATA 50,181,136,42,182,136,205,26,188,126,254,240,32,9,33,182,136,53,205,95
850 DATA 156,24,40,42,182,136,44,205,26,188,126,254,240,40,28,42,182,136,44,44
860 DATA 205,26,188,126,254,240,40,5,62,1,50,181,136,205,88,156,33,182,136,52
870 DATA 205,95,156,201
