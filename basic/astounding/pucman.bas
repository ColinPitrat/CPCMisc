10 REM************************
20 REM****PUCMAN'S REVENGE****
30 REM************************
40 DIM S(40, 25): DIM HS(10): DIM HS$(10)
41 OUT &FB7F,4:OUT &FB7F,0:ST=INP(&FB7F):IF ST AND 192 GOTO 50:REM TEST FOR NON-WRITE PROTECTED DISC PRESENCE
42 ON ERROR GOTO 48:OPENIN "HIGHSCOR.TXT":CLOSEIN:REM TEST FOR FILE EXISTENCE
43 ON ERROR GOTO 0
45 OPENIN "HIGHSCOR.TXT":FOR I=1 TO 10:INPUT #9,HS$(I),HS(I):NEXT I:CLOSEIN
46 GOTO 50
48 CLOSEIN:OPENOUT "HIGHSCOR.TXT":FOR I=1 TO 10:NM$="PUCMAN":WRITE #9,NM$,0:NEXT I:CLOSEOUT:RESUME 43
50 SYMBOL AFTER 32
60 BORDER 1
70 ENV 6, 15,-1,1
80 GOSUB 940
90 GOSUB 790
100 GOSUB 700
110 GOSUB 580
120 REM****MAIN LOOP****
125 IMG=IMG+1
130 J$=INKEY$: IF J$="" THEN GOTO 180
140 IF (J$="A" OR J$="a") AND X>3 THEN X=X-1: NC$=CHR$(198): P=1
150 IF (J$="Q" OR J$="q") AND X<24 THEN X=X+1: NC$=CHR$(196): P=2
160 IF (J$="O" OR J$="o") AND Y>2 THEN Y=Y-1: NC$=CHR$(197): P=3
170 IF (J$="P" OR J$="p") AND Y<38 THEN Y=Y+1: NC$=CHR$(199): P=4
180 IF J$="" AND P=1 AND X>3 THEN X=X-1
190 IF J$="" AND P=2 AND X<24 THEN X=X+1
200 IF J$="" AND P=3 AND Y>2 THEN Y=Y-1
210 IF J$="" AND P=4 AND Y<38 THEN Y=Y+1
220 IF S(Y,X)=9 THEN SC=SC+INT(RND*9)+3: SOUND 1,150,4,4: S(Y,X)=0
230 IF S(Y,X)>9 THEN GOTO 320
235 C$=NC$:REM IF IMG AND 1 THEN C$=CHR$(200) ELSE C$=NC$
237 FRAME
240 LOCATE YY, XX: PRINT" "
250 LOCATE Y,X:PEN 2: PRINT C$:PEN 1
260 SOUND 1,900,4,5,1,1: SOUND 1,0,4
270 LOCATE 7,1: PRINT SC
280 XX=X:YY=Y
290 IF RND>0.96 THEN A=INT(RND*35)+3: B=INT(RND*21)+3: LOCATE A-1,B: XR=INT(RND*4)+1:S(A, B)=9:PEN 1:PRINT CHR$(247+(XR));:PEN 1:SOUND 1,INT(RND*50)+80,4,5
290 IF RND>0.96 THEN A=INT(RND*35)+3: B=INT(RND*21)+3: LOCATE A,B: XR=INT(RND*4)+1:S(A, B)=9:PEN 1:PRINT CHR$(247+(XR));:PEN 1:SOUND 1,INT(RND*50)+80,4,5
300 IF RND>0.84 THEN PEN 3:F=INT(RND*36)+2:G=INT(RND*21)+3:LOCATE F, G:PRINT CHR$(228):S(F, G)=10
310 GOTO 120
320 REM*****LOSE*****
330 SOUND 2,600.16,7,1,12,2
340 SOUND 4,600,80,7,9,13,6
350 SOUND 1,600,30,7,4,1,15
360 CLS
370 LOCATE 14,8
380 PEN 3
390 PRINT "AAAAAAAAHHHHHH!!!"
400 PEN 1
419 PRINT: PRINT"    THEY CAUGHT YOU WITH A LOVE TRAP"
420 PRINT: PRINT"    YOU WON'T BE MUNCHING ANY MORE"
430 PRINT: PRINT"    HUMANS, AT LEAST FOR A WHILE"
440 PRINT: PRINT"    YOUR SCORE WAS "; SC
450 PRINT: PRINT
460 IF HS(10)>=SC THEN PRINT"THAT IS NOT UP TO THE LOWEST HIGH SCORE "; HS(10): GOTO 480
465 CLEAR INPUT
470 HS(10)=SC: PRINT"WELL DONE, YOU NOW HAVE A HIGH SCORE": INPUT "ENTER YOUR NAME, PLEASE"; HS$(10)
475 FOR I=9 TO 1 STEP -1
476 IF HS(I+1)>HS(I) THEN T=HS(I):T$=HS$(I):HS(I)=HS(I+1):HS$(I)=HS$(I+1):HS(I+1)=T:HS$(I+1)=T$
477 NEXT I
478 OUT &FB7F,4:OUT &FB7F,0:ST=INP(&FB7F):IF ST AND 192 GOTO 480:REM TEST FOR NON-WRITE PROTECTED DISC PRESENCE
479 OPENOUT "HIGHSCOR.TXT":FOR I=1 TO 10:WRITE #9,HS$(I),HS(I):NEXT I:CLOSEOUT
480 PRINT: PRINT: PRINT
490 WINDOW #1,6,34,5,16
500 PEN #1,3: PAPER #1,1
505 CLS
507 LOCATE 14,2:PRINT "HALL OF FAME"
510 CLS #1
520 PRINT #1,
530 FOR I = 1 TO 10: PRINT #1,"  ";:PRINT #1,using "\                \";HS$(I);:print #1,"  ";:PRINT #1,using "#####";HS(I): NEXT I
540 LOCATE 6,23
550 PRINT "ANOTHER GO? PRESS 'Y' OR 'N'"
555 CLEAR INPUT
560 D$=INKEY$: IF D$="" THEN 560
570 IF D$="Y" OR D$="y" THEN SOUND 1,180:GOTO 100
575 IF D$="N" OR D$="n" THEN GOTO 1010
576 GOTO 550
580 REM***SCREEN***
590 MODE 1
600 CLS
610 PRINT"SCORE: "; SC;
620 PEN 2
630 PRINT TAB (14); "PUCMAN'S REVENGE";
640 PEN 1
650 PRINT TAB (35); CHR$ (164); "1984"
660 PRINT"#######################################"
670 FOR T=1 TO 22: PRINT"#"; TAB (39); "#":NEXT T
680 PRINT"#######################################"
690 RETURN
700 REM*****INITIALISATION*****
710 FOR H=1 TO 40
720 FOR V=1 TO 25
730 S(H,V)=0
740 NEXT: NEXT
750 X=20: Y=13: SC=0
760 XX=X: YY=Y
770 F=0: G=0: A=0: B=0: P=0: NC$=CHR$(200): IMG=0
780 RETURN
790 REM****TITLE****
800 INK 1,24
810 CLS: LOCATE 1,5
820 FOR G=1 TO 3: FOR H=1 TO 3
830 PEN H
840 PRINT "#####  #   #  ####  #   #  #####  #   #";
850 PRINT "#   #  #   #  #     ## ##  #   #  ##  #";
860 PRINT "#####  #   #  #     # # #  #####  # # #";
870 PRINT "#      #   #  #     #   #  #   #  #  ##";
880 PRINT "#      #####  ####  #   #  #   #  #   #";
890 PRINT: PRINT TAB (7); "AND HIS REVENGE" : PRINT
900 SOUND 2, H*30+ (G*30)
910 NEXT H: NEXT G
920 SOUND 2,200,80
930 RETURN
940 REM***USER DEFINABLE GRAPHICS***
945 SYMBOL 200,60,126,255,255,255,255,126,60
950 SYMBOL 198,0,66,195,231,255,255,126,60
960 SYMBOL 199,60,126,248,240,240,248,126,60
970 SYMBOL 196,60,126,255,255,231,195,66,0
980 SYMBOL 197,60,126,31,15,15,31,126,60
990 SYMBOL 35,255,255,255,255,255,255,255,255
1000 RETURN
1010 LOCATE 6,24
1020 PRINT "BYE THEN!"
1030 END
1040 PRINT "NOT LOADING HIGHSCORES":RESUME 50