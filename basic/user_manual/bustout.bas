10 REM Bustout
20 REM
30 REM
40 MODE 1:BORDER 1:INK 0,1:INK 1,26:INK 2,24:INK 3,6
50 SPEED KEY 15,2
60 ENV 1,1,18,0,11,0,10
70 ENT 1,10,2,2
80 ENV 3,1,0,16,5,-3,2
90 ENV 2,5,3,3,1,-21,22,9,-3,2
100 ENT -2,10,2,2,5,-7,1,2,11,3,2,-4,8
110 REM
120 REM
130 MOVE 30,32:DRAWR 0,400,1:MOVE 610,32:DRAWR 0,400,1
140 PEN 3:LOCATE 3,1:PRINT STRING$(36,143)
150 PEN 2:LOCATE 3,2:PRINT STRING$(36,143)
160 PEN 1:FOR r=5 TO 6:LOCATE 3,r:PRINT STRING$(36,143):NEXT r
170 bx=9
180 lives=5:score=0
190 PEN 1:GOSUB 680:CLEAR INPUT
200 IF INKEY$<>CHR$(32) AND JOY(0)<16 THEN 200
210 LOCATE 11,23:PRINT SPACE$(20):LOCATE 1,24:PRINT SPACE$(40);
220 GOSUB 690:GOSUB 660:GOTO 280
230 REM
240 REM
250 LOCATE bx,24:PRINT "  ";STRING$(4,131);"  ":RETURN
260 REM
270 REM
280 xa=1:ya=1:IF INT(RND*2)=1 THEN xa=xa
290 PEN 1:GOSUB 250
300 ORIGIN 0,400
310 x=bx+4:y=11:x1=x:y1=y
320 REM
330 REM
340 x1=x+xa:y1=y+ya
350 IF x1<=3 OR x1>=38 THEN xa=-xa
360 GOSUB 540
370 IF y1=24 AND x1>bx+1 AND x1<bx+6 THEN ya=-ya:y1=y1-2:SOUND 130,44,8,7,1,1:a=((x>bx+5) OR (x<bx+2)):IF a=-1 THEN xa=xa*a:x1=x1+xa:y1=y1+1
380 IF y1=25 THEN LOCATE x,y:PRINT " ":GOTO 500
390 GOSUB 250
400 t=TEST((16*x1)-1,-(16*y1)-1)
410 IF t<>0 THEN ya=-ya:xz=x1:yz=y1:y1=y1+ya:GOSUB 590
420 IF t=3 THEN score=score+5:GOSUB 660
425 IF t=2 THEN score=score+2:GOSUB 660
430 IF t=1 THEN score=score+1:GOSUB 660
440 IF y1=1 THEN ya=1
450 LOCATE x,y:PRINT " ":LOCATE x1,y1:PRINT CHR$(233):x=x1:y=y1
460 IF y=1 OR x=3 OR x=38 THEN SOUND 129,78,8,7,1,1
470 GOTO 340
480 REM
490 REM
500 lives=lives-1:SOUND 132,19,46,12,2,2:IF lives=0 THEN 620
510 GOSUB 660:GOTO 280
520 REM
530 REM
540 IF (INKEY(8)=0 OR INKEY(74)=0) AND bx>2 THEN bx=bx-2:RETURN
550 IF (INKEY(1)=0 OR INKEY(75)=0) AND bx<32 THEN bx=bx+2:RETURN
560 RETURN
570 REM
580 REM
590 LOCATE xz,yz:PRINT " ":RETURN
600 REM
610 REM
620 IF score >= hiscore THEN hiscore=score
630 GOSUB 660:score=0:vies=5:GOTO 130
640 REM
650 REM
660 SOUND 130,0,20,13,3,0,31:LOCATE 1,25:PRINT TAB(3)"HISCORE:";hiscore;
670 LOCATE 18,25:PRINT "SCORE:";score:LOCATE 31,25:PRINT "LIVES:";lives:RETURN
680 LOCATE 11,23:PRINT "PRESS SPACE TO START":RETURN
690 LOCATE 1,25:PRINT SPACE$(40);:RETURN