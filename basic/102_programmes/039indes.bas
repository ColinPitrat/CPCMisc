10 REM L'INDESIRABLE 
20 RANDOMIZE(TIME) 
30 PRINT CHR$(22)+CHR$(1) 
40 B$=STRING$(3,127) 
50 DIM B(16) 
60 CLS 
70 FOR I=1 TO 16 
80 LOCATE 4+4*(I-1-4*INT((I-1)/4)),2+4*INT((I-1)/4):PEN 3:PRINT B$ 
90 LOCATE 4+4*(I-1-4*INT((I-1)/4)),3+4*INT((I-1)/4):PRINT B$;CHR$(8);CHR$(8);:PEN 2:PRINT CHR$(64+I) 
100 NEXT I 
110 X=INT(RND*16)+1:S=1:K=O:PRINT CHR$(22)+CHR$(0) 
120 PEN 1:LOCATE 1,20:PRINT CHR$(20);:INPUT "CASE VISITEE :";C$:C=ASC(C$+" ")-64 
130 IF C<1 OR C>16 THEN 120 
140 IF B(C)=1 THEN LOCATE 14,22:PAPER 1:PEN 0:PRINT "DEJA VISITEE'":PRINT CHR$(7);:PAPER 0:GOTO 120 
150 IF C=X THEN LOCATE 3,22:PEN 2:PRINT "VOUS L'AVEZ EU EN";S;"COUPS. BRAVO":SOUND 1,478:SOUND 1,379:SOUND 1,319:K=1:GOTO 220 
160 IF X-5=C OR X-4=C OR X-3=C OR X-1=C OR X+1=C OR X+3=C OR X+4=C OR X+5=C THEN GOTO 220 
170 LOCATE 12,22:PRINT "IL N'EST PAS LA" : SOUND 1,478: SOUND 1,426: SOUND 1,379 
180 S=S+1:B(C)=1:FOR I=1 TO 16 
190 LOCATE 4+4*(I-1-4*INT((I-1)/4)),2+4*INT((I-1)/4):PEN 3-B(I):PRINT B$ 
200 NEXT I 
210 IF S=9 THEN LOCATE 5,22:PAPER 2:PEN O: PRINT "VOUS AVEZ PERDU, IL ETAIT EN ";CHR$(X+64):K=1:PAPER 0:PEN 1:ELSE GOTO 120 
220 LOCATE 5+4*(X-1-4*INT((X-1)/4)),1+4*INT((X-1)/4):PRINT "X" 
230 HE=TIME:WHILE TIME-HE<20:WEND 
240 IF K=1 THEN SOUND 1,319:SOUND 1,379:SOUND 1,478:LOCATE 1,24:END 
250 LOCATE 5+4*(X-1-4*INT((X-1)/4)),1+4*INT((X-1)/4):PRINT " " 
260 Z=INT(10*RND+1)-5:IF ABS(Z)=2 THEN 260 
270 X=X+Z:IF X>16 OR X<1 THEN X=X-Z:GOTO 260 
280 GOTO 180 
