10 REM Bombardier
20 REM Copyright Amsoft 1984
30 REM
40 MODE 1:CLS:INK 0,0:BORDER 0:INK 1,18:INK 2,6:INK 3,4:INK 5,15:INK 6,2:INK 7,24:INK 8,8:INK 9,26:INK 10,10:INK 11,20:INK 12,12:INK 13,16:INK 14,14:INK 15,21
50 SYMBOL AFTER 240:SYMBOL 241,&40,&60,&70,&7F,&7F,&3F,&7,&0:SYMBOL 242,&0,&32,&7A,&FE,&FA,&F2,&E0,&0
60 score=0:hiscore=0:avion$=CHR$(241)+CHR$(242):x=2:y=2:chute=0:a=2:b=2
70 GOSUB 480
80 CLS
90 PEN 2:LOCATE 1,14:INPUT "Niveau: 0 (As) a 5 (Debutant): ",niv
100 IF niv<0 OR niv>5 GOTO 90
110 niv=niv+10
120 LOCATE 1,15:PRINT CHR$(18);:LOCATE 1,15:INPUT "Vitesse: 0 (Rapide) a 100 (Lent): ",vit
130 IF vit<0 OR vit>100 GOTO 120
140 REM
150 REM Immeubles
160 REM
170 MODE 0:FOR base=5 TO 15:FOR haut=21 TO INT(RND(1)*8+niv) STEP -1:LOCATE base,haut:PEN base-2:PRINT CHR$(143)+CHR$(8)+CHR$(11)+CHR$(244);:NEXT:NEXT
180 PLOT 0,20,4:DRAW 640,20,4
190 LOCATE 1,25:PEN 2:PRINT "SCORE";SCORE;:LOCATE 13,25:PRINT"HI";hiscore;
200 REM
210 REM Boucle principale du jeu
220 REM
230 LOCATE x-1,y:PRINT " ";
240 PEN 1:LOCATE x,y:PRINT avion$;:PEN 2
250 IF y=21 AND x=15 THEN GOTO 290:ELSE GOTO 340
260 REM
270 REM Atterissage reussi
280 REM
290 FOR c=0 TO 1000:NEXT
300 score=score+100-(niv*2):niv=niv-1:x=2:y=2:a=2:b=2:chute=0
310 IF niv<10 THEN niv=10:vit=vit-20
320 IF vit<0 THEN vit=0
330 GOTO 150
340 FOR c=0 TO vit:NEXT
350 x=x+1
360 IF x=18 THEN LOCATE x-2,y:PRINT CHR$(18);:x=2:y=y+1:LOCATE x,y:PEN 1:PRINT avion$;:PEN 2
370 a$=INKEY$:IF a$=" " AND chute=0 THEN chute=1:b=y+2:a=x
380 IF y=21 THEN chute=0
390 IF chute=1 THEN LOCATE a,b:PRINT CHR$(252);:LOCATE a,b-1:PRINT " ";:b=b+1:IF b>21 THEN LOCATE a,b-1:PRINT " ";:a=0:b=0:chute=0:SOUND 3,4000,10,12,0,0,10
400 ga=(a-0.5)*32:gb=400-(b*16):bomb=TEST(ga,gb)
410 IF bomb>0 THEN GOTO 670
420 gx=((x+1.5)*32):gy=408-(y*16):crash=TEST(gx,gy)
430 IF crash>0 GOTO 570
440 GOTO 230
450 REM
460 REM Mode d'emploi
470 REM
480 LOCATE 1,2:PEN 1:PRINT "Vous pilotez un avion au dessus d'une ville abandonnee que vous devez 'raser' pour atterir et faire le plein de fuel. Votre avion se deplace de gauche a droite.":PRINT
490 PRINT:PRINT "Une fois le bord droit atteint, l'avion revient a gauche une ligne plus bas. Vous disposez d'une quantite illimitee de bombes et vous pouvez les larguer sur les immeubles en appuyant sur la barre espace.";:PRINT
500 PRINT:PRINT "A chaque fois que vous atterissez, soit la vitesse de votre avion, soit la hauteur des immeubles augmente.";:PRINT:PRINT:PRINT"Vous ne pouvez larguer de bombe tant que la precedente n'a pas explose.";
510 PEN 2:LOCATE 4,25:PRINT"Pressez une touche pour commencer.";
520 a$=INKEY$:IF a$="" GOTO 520
530 RETURN
540 REM
550 REM Collision
560 REM
570 LOCATE x-1,y:PRINT CHR$(32)+CHR$(32)+CHR$(32)+CHR$(253)+CHR$(8)+CHR$(238)+CHR$(8);
580 FOR t=1 TO 10:SOUND 7,4000,5,15,0,0,5:PEN t:PRINT CHR$(253)+CHR$(8)+CHR$(238)+CHR$(8)+CHR$(32)+CHR$(8);:FOR tm=0 TO 50:NEXT:NEXT:PEN 2
590 MODE 1:LOCATE 1,5:PRINT "SCORE";score;
600 IF score>hiscore THEN hiscore=score:LOCATE 1,8:PRINT "Meilleur score !";
610 score=0:LOCATE 1,12:PRINT"Tapez 'R' pour rejouer";
620 a$=INKEY$:IF a$="R" OR a$="r" THEN GOTO 630 ELSE GOTO 620
630 PEN 1:MODE 1:x=2:y=2:a=2:b=2:GOTO 90
640 REM
650 REM Immeubles detruits
660 REM
670 LOCATE a,b-1:PRINT " "
680 PEN 4
690 FOR tr=1 TO INT(RND(1)*3)+1
700 score=score+5
710 SOUND 3,4000,10,12,0,0,10
720 LOCATE a,b
730 FOR t=0 TO 4
740 PRINT CHR$(253)+CHR$(8)+CHR$(32)+CHR$(8);
750 NEXT
760 b=b+1
770 IF b=24 THEN b=b-1
780 NEXT
790 LOCATE 6,25:PRINT score;:chute=0:a=x:b=y:GOTO 230
