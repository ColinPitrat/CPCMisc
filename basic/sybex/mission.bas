10 REM *********
20 REM *MISSION*
30 REM *********
40 GOSUB 390
50 MODE 1:MEMORY 9000
60 BORDER 0
70 INK 0,0:INK 1,24
80 INK 2,23:INK 3,6
90 X=20:SC=0:M=0:P=41:N=0:TA=6
100 L=1:C=40
110 SC=SC+1:N=N+1
120 IF N<100 THEN 170
130 N=0:TA=TA+1:IF TA=25 THEN SOUND 1,100:SOUND 1,300:SOUND 1,200:FOR T=1 TO 5000:NEXT T:CLS:LOCATE 12,5:PRINT "MISSION  ACCOMPLIE":GOTO 370
140 INK 3,TA:M=M+1:P=P-1:C=C-2:L=L+1
150 IF X=M THEN X=M+1
160 IF X=P THEN X=P-1
170 PEN 2:LOCATE X,25:PRINT CHR$(239);
180 LOCATE X,24:CALL 10008:IF PEEK(39000)<>32 THEN 270
190 PEN 3:B=INT(RND(1)*C)+L
200 LOCATE B,3:PRINT CHR$(202)
210 CALL 10000
220 A=X:X=X-INKEY(8)+INKEY(1)
230 IF A=X THEN 110
240 IF X=P OR X=M THEN X=A:GOTO 110
250 PEN 2:LOCATE A,25:PRINT CHR$(32);
260 LOCATE X,24:CALL 10008:IF PEEK(39000)=32 THEN 110
270 SOUND 1,2000:SOUND 1,2000
280 FOR T=1 TO 100
290 LOCATE X,25:PRINT CHR$(238);
300 LOCATE X,25:PRINT CHR$(32);
310 NEXT T
320 CLS:PEN 1
330 LOCATE 16,5:PRINT "SCORE  :";SC
340 IF SC>RE THEN RE=SC
350 LOCATE 16,7:PRINT "RECORD :";RE
360 LOCATE 16,9:PRINT "TABLEAU:";TA-5
370 LOCATE 13,11:PRINT "APPUYER SUR 'f0'"
380 IF INKEY(15)=0 THEN 50 ELSE 380
390 RESTORE 450
400 FOR T=10000 TO 10014
410 READ A
420 POKE T,A
430 NEXT T
440 RETURN
450 DATA 62,0,6,0,205,77,188,201,205,96,187,50,88,152,201
