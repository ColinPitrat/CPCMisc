10 REM L'ELECTRICIEN FOU 
20 RANDOMIZE(TIME) 
30 DIM A(10) 
40 PAPER 0:PEN 1:CLS 
50 FOR I=1 TO 7 
60 PAPER 2:LOCATE 10,I:PRINT STRING$(21, 32):PAPER 0
70 NEXT I
80 T=0 
90 FOR I=1 TO 10 
100 T=T+A(I) 
110 LOCATE 2*I+9,4:PRINT " " 
120 IF A(I)=0 THEN LOCATE 2*I+9,4:PRINT CHR$(224) 
130 PAPER 2:PEN O:LOCATE 2*I+8,6:PRINT I-1:PAPER 0:PEN 1 
140 NEXT I 
150 IF T=10 THEN 250 
160 PEN 3:LOCATE 1,14:PRINT CHR$(7);CHR$(20);:INPUT "INTERRUPTEUR NUMERO :";N:PEN 1
170 N=N+1
180 M=M+1
190 A(N)=1-A(N)
200 R=TAN(RND+N/RND-N)-SIN(RND/N)+336*SIN(8*N)
210 N=INT(10*(R-INT(R)))
220 IF N=0 THEN N=INT(RND*10+1)
230 IF A(N)=1 THEN A(N)=0:GOTO 200
240 A(N)=1:GOTO 80
250 LOCATE 1,14:PRINT "TROUVE EN";M;"ESSAIS"
