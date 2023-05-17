10 REM Arsene Lupin
20 REM
30 REM
40 MODE 0:INK 0,0:BORDER 0:INK 1,26:INK 2,15:INK 3,25
50 INK 4,14:INK 5,24:INK 6,0:INK 7,0:INK 8,0:PAPER #1,7
60 delais=200
70 DIM objx(5,20), objy(5,20),gemx(5,20),gemy(5,20)
80 GOSUB 380
90 GOSUB 720
100 pause=200:GOSUB 340
110 IF gems=0 THEN GOSUB 970
120 PEN 4
130 FOR i=10 TO 12
140 LOCATE 15,i:PRINT "butin";
150 NEXT
160 PAPER 0:CLS #2:PAPER 8
170 GOSUB 1170
180 GOSUB 1230
190 GOSUB 1370
200 GOSUB 1510
210 IF rm=0 THEN GOSUB 1900
220 IF mort=0 THEN 160
230 pause=100:GOSUB 340
240 PAPER 0:CLS:PEN 1
250 LOCATE 5,3:PRINT "Voulez-vous";
260 LOCATE 7,5:PRINT "rejouer";
270 PEN 5:LOCATE 9,7:PRINT "O/N";
280 i$=UPPER$(INKEY$):IF i$<>"O" AND i$<>"N" THEN 280
290 IF i$="N" THEN MODE 2:PEN 1:STOP
300 RUN
310 IF chien=1 THEN RETURN
320 chien=1:chienx=minx(rm):chieny=miny(rm)
330 RETURN
340 FOR loop=1 TO pause
350 FRAME
360 NEXT
370 RETURN
380 rm=1:xp=6:yp=4:homme$=CHR$(224):chien=0:vol=0
390 SYMBOL 240,8,8,8,8,8,8,8,8
400 SYMBOL 241,0,0,0,0,255,0,0,0
410 SYMBOL 242,0,0,0,0,15,8,8,8
420 SYMBOL 243,0,0,0,0,248,8,8,8
430 SYMBOL 244,8,8,8,8,248,0,0,0
440 SYMBOL 245,8,8,8,8,15,0,0,0
450 SYMBOL 246,8,12,13,14,12,12,8,8
460 SYMBOL 247,8,12,12,14,13,12,8,8
470 SYMBOL 248,8,24,88,56,24,24,8,8
480 SYMBOL 249,8,24,24,56,88,24,8,8
490 SYMBOL 250,0,0,255,255,255,255,255,0
500 SYMBOL 251,28,20,20,20,20,20,20,28
510 SYMBOL 252,0,0,255,255,255,255,255,0
520 SYMBOL 253,28,28,28,28,28,28,28,28
530 SYMBOL 255,195,165,60,126,90,60,36,24
540 ENT 1,12,-4,1
550 ENT -2,=1000,60,=3000,40
560 ENV 1,10,1,5,2,-4,1,2,-1,20
570 fenet$(1)=STRING$(2,250):fenet$(2)=CHR$(251)+CHR$(8)+CHR$(251)+CHR$(8)+CHR$(10)+CHR$(251)
580 porte$(1)=STRING$(2,252):porte$(2)=CHR$(253)+CHR$(8)+CHR$(10)+CHR$(253)+CHR$(8)+CHR$(10)+CHR$(253)
590 inter$(1,0)=CHR$(246):inter$(1,1)=CHR$(247)
600 inter$(2,0)=CHR$(248):inter$(2,1)=CHR$(249)
610 gem$=CHR$(144):obj$=CHR$(233):chien$=CHR$(255)
620 coup$=CHR$(246)+CHR$(248)+CHR$(247)+CHR$(249)+CHR$(252)+CHR$(253)+CHR$(250)+CHR$(251)+gem$+obj$+chien$
630 RESTORE 3010
640 FOR i=1 TO 5
650 READ minx(i),miny(i),maxx(i),maxy(i)
660 READ dir(i,1),dir(i,2),dir(i,3),dir(i,4)
670 NEXT
680 WINDOW #1,minx(rm)-1,maxx(rm)+1,miny(rm)-1,maxy(i)+1
690 WINDOW #2,1,14,1,25
700 CLS #1:PAPER #0,8
710 RETURN
720 ORIGIN 50,50
730 INK 6,24,12
740 RESTORE 3060
750 GOSUB 1280
760 LOCATE 2,20
770 PEN 5:PRINT "--";
780 PEN 1:PRINT "echappatoires";
790 PEN 5:PRINT "--";
800 LOCATE 8,2:PRINT "entree";
810 pause=300:GOSUB 340
820 CLS:LOCATE 1,3:INK 6,0
830 PEN 1:PRINT homme$;"    A.Lupin(vous)":PRINT
840 PEN 2:PRINT LEFT$(porte$(1),1);LEFT$(porte$(2),1);"portes":PRINT
850 PEN 3:PRINT inter$(1,0);inter$(2,0);"interrupteur eteint"
860 PEN 3:PRINT inter$(1,1);inter$(2,1);"interrupteur allume":PRINT
870 PEN 4:PRINT LEFT$(fenet$(1),1);LEFT$(fenet$(2),1);"fenetres":PRINT
880 PEN 5:PRINT gem$;"   bijoux":PRINT
890 PAPER 1:PEN 0:PRINT obj$;"   obstacles":PEN 1:PAPER 0:PRINT
900 PEN 1:PRINT chien$;"   le chien"
910 PEN 5:PRINT:PRINT
920 PRINT "Utilisez le joystick":PRINT "   ou les fleches"
930 feint=REMAIN(1)
940 AFTER delais*4,1 GOSUB 340
950 RETURN
960 REM
970 REM Generateur de rubis/obstacles
980 REM
990 FOR piece=1 TO 5
1000 gemr=INT(RND*8)+2:objr=INT(RND*10)+5
1010 minx=minx(piece):miny=miny(piece):maxx=maxx(piece):maxy=maxy(piece)
1020 FOR i=1 TO gemr
1030 x=INT(RND*(maxx-minx+1))+minx
1040 y=INT(RND*(maxy-miny+1))+miny
1050 gemx(piece,i)=x:gemy(piece,i)=y
1060 gems=gems+1
1070 NEXT i
1080 FOR i=1 TO objr
1090 x=INT(RND*(maxx-minx+1))+minx
1100 y=INT(RND*(maxy-miny+1))+miny
1110 objx(piece,i)=x:objy(piece,i)=y
1120 NEXT i
1130 gems(piece)=gemr:obj(piece)=objr
1140 NEXT piece
1150 CLS
1160 RETURN
1170 ON rm GOTO 1180,1190,1200,1210,1220
1180 RESTORE 2670:RETURN
1190 RESTORE 2740:RETURN
1200 RESTORE 2810:RETURN
1210 RESTORE 2880:RETURN
1220 RESTORE 2960:RETURN
1230 PAPER 0:READ rm$:PAPER 8
1240 WINDOW #1,minx(rm)-1,maxx(rm)+1,miny(rm)-1,maxy(rm)+1:CLS #1
1250 PEN 1:LOCATE 1,1:PRINT SPACE$(19);
1260 LOCATE 1,1:PRINT "piece: ";rm$;
1270 IF lumiere(rm) THEN INK 7,10:INK 8,10 ELSE INK 7,0:INK 8,0
1280 READ a$:IF a$="END" THEN RETURN
1290 IF a$="D" THEN 2180
1300 IF a$="W" THEN 2260
1310 IF a$="L" THEN GRAPHICS PEN 1:GOTO 2340
1320 IF a$="S" THEN 2420
1330 IF a$="F" THEN GRAPHICS PEN 6:GOTO 2340
1340 PRINT "***erreur***";:PRINT a$;
1350 STOP
1360 REM
1370 REM Affichage bijoux/objets
1380 REM
1390 PEN 6
1400 FOR i=1 TO obj(rm)
1410 LOCATE objx(rm,i),objy(rm,i)
1420 PRINT obj$;
1430 NEXT
1440 PEN 5
1450 FOR i=1 TO gems(rm)
1460 LOCATE gemx(rm,i),gemy(rm,i)
1470 PRINT gem$;
1480 NEXT
1490 PEN 1:LOCATE xp,yp:PRINT homme$;
1500 RETURN
1510 xf=0:yf=0:PEN 1
1520 IF INKEY(0)<>-1 OR INKEY(72)<>-1 THEN yf=-1
1530 IF INKEY(2)<>-1 OR INKEY(73)<>-1 THEN yf=1
1540 IF INKEY(8)<>-1 OR INKEY(74)<>-1 THEN xf=-1
1550 IF INKEY(1)<>-1 OR INKEY(75)<>-1 THEN xf=1
1560 IF xf=0 AND yf=0 THEN 1630
1570 LOCATE xp+xf,yp+yf:ht$=COPYCHR$(#0)
1580 IF ASC(ht$)>239 AND ASC(ht$)<246 THEN 1510
1590 IF ht$<>" " THEN 1660
1600 LOCATE xp,yp:PRINT " ";
1610 PAPER 0:LOCATE 15,5:PRINT "    ";:PAPER 8
1620 xp=xp+xf:yp=yp+yf
1630 LOCATE xp,yp:PRINT homme$;
1640 IF chien>0 THEN chien=chien MOD 2+1:IF chien=2 THEN 2550
1650 GOTO 1510
1660 coup=INSTR(coup$,ht$)
1665 char=ASC(MID$(coup$,coup,1))
1670 ON coup GOTO 1690,1690,1690,1690,1750,1750,1850,1900,1970,2090,2650
1680 GOTO 1600
1690 IF coup>2 AND coup<5 THEN char=char-1
1700 IF coup<3 THEN char=char+1
1710 PEN 3:LOCATE xp+xf,yp+yf:PRINT CHR$(char);
1720 lumiere(rm)=lumiere(rm) XOR 1
1730 IF lumiere(rm) THEN INK 7,10:INK 8,10 ELSE INK 7,10:INK 8,0
1740 GOTO 1510
1750 IF xf<>0 AND yf<>0 THEN 1630
1760 IF xf<0 THEN dir=4 ELSE IF xf>0 THEN dir=3
1770 IF yf<0 THEN dir=1 ELSE IF yf>0 THEN dir=2
1780 IF dir(rm,dir)=-1 THEN 1630 ELSE rm=dir(rm,dir)
1790 IF chien>0 THEN GOSUB 310
1800 IF dir=1 THEN xp=6:yp=maxy(rm)
1810 IF dir=2 THEN xp=6:yp=miny(rm)
1820 IF dir=3 THEN xp=minx(rm):yp=13
1830 IF dir=4 THEN xp=maxx(rm):yp=13
1840 RETURN
1850 IF xp>5 AND xp<8 THEN 1880
1860 IF xp<6 THEN dir=4 ELSE dir=3
1870 GOTO 1780
1880 IF yp>13 THEN dir=2 ELSE dir=1
1890 GOTO 1780
1900 PAPER 0:CLS:PEN 1
1910 LOCATE 3,3:PRINT "Vous vous en tirez !";
1920 IF gems=vol THEN LOCATE 6,4: PRINT "Avec TOUT !":LOCATE 5,6:ELSE LOCATE 4,4: PRINT "Avec ";
1940 PRINT USING " ##";vol;
1950 PEN 5:PRINT " bijoux";
1960 mort=1:RETURN
1970 LOCATE xp,yp:PRINT " ";:xp=xp+xf:yp=yp+yf
1980 i=0
1990 i=i+1
2000 IF i>gems(rm) THEN 1510
2010 IF gemx(rm,i)<>xp OR gemy(rm,i)<>yp THEN 1990
2020 IF i=gems(rm) THEN 2050
2030 gemx(rm,i)=gemx(rm,gems(rm))
2040 gemy(rm,i)=gemy(rm,gems(rm))
2050 gems(rm)=gems(rm)-1:vol=vol+1
2060 MOVE 400,150+(vol*2),1,1:DRAW 560,150+(vol*2),1,1
2070 SOUND 129,248,10,12,0,1
2080 GOTO 1980
2090 bruit=INT(RND*15)
2100 SOUND 1,3000,10,bruit,0,0,10
2110 PAPER 0:LOCATE 12,5:PRINT "CRACK !";:PAPER 8
2120 IF bruit<10 OR delais=50 THEN 1630
2130 delais=delais-50
2140 feint=REMAIN(1)
2150 AFTER delais*4,1 GOSUB 310
2160 GOTO 1630
2170 REM
2180 REM Dessin des portes
2190 REM
2200 READ no,dr$
2210 IF dr$="V" THEN dr=2 ELSE dr=1
2220 PEN 2
2230 pic$=porte$(dr):GOSUB 2500
2240 GOTO 1280
2250 REM
2260 REM Dessin des fenetres
2270 REM
2280 READ no,wi$
2290 IF wi$="V" THEN wi=2 ELSE wi=1
2300 PEN 4
2310 pic$=fenet$(wi):GOSUB 2500
2320 GOTO 1280
2330 REM
2340 REM Tire les traits
2350 REM
2360 READ x1,y1,x2,y2
2370 MOVE x1,y1,,0
2380 DRAW x1,y2,,0:DRAW x2,y2,,0
2390 DRAW x2,y1,,0:DRAW x1,y1,,0
2400 GOTO 1280
2410 REM
2420 REM Dessine les interrupteurs
2430 REM
2440 READ no,sw$
2450 IF sw$="L" THEN sw=1 ELSE sw=2
2460 PEN 3
2470 pic$=inter$(sw,0):GOSUB 2500
2480 GOTO 1280
2490 REM
2500 REM Affiche le caractere
2510 REM
2520 READ x,y:LOCATE x,y:PRINT pic$;
2530 no=no-1:IF no>0 THEN 2520
2540 RETURN
2550 PEN 1:LOCATE chienx,chieny:PRINT " ";
2560 homme$=CHR$(225)
2570 IF (chienx=xp AND chieny=yp) OR (chienx=xp+xf AND chieny=yp+yf) THEN 2650
2580 IF chienx<xp THEN chienx=chienx+1
2590 IF chienx>xp THEN chienx=chienx-1
2600 IF chieny<yp THEN chieny=chieny+1
2610 IF chieny>yp THEN chieny=chieny-1
2620 LOCATE chienx,chieny:PRINT chien$;
2630 SOUND 1,0,RND*40,10,1,2,31
2640 GOTO 1510
2650 PRINT "SNAP !";
2660 mort=1:RETURN
2670 DATA entree
2680 DATA L,64,308,226,4
2690 DATA D,2,H,6,3,6,22
2700 DATA D,2,V,4,12,9,11
2710 DATA S,1,L,4,11
2720 DATA S,1,R,9,14
2730 DATA END
2740 DATA salon
2750 DATA L,2,308,258,4
2760 DATA D,1,V,10,12
2770 DATA W,1,H,6,3
2780 DATA W,1,V,2,12
2790 DATA S,2,R,10,11,10,15
2800 DATA END
2810 DATA salle a manger
2820 DATA L,2,308,258,4
2830 DATA W,1,V,10,12
2840 DATA W,1,H,6,3
2850 DATA D,1,V,2,12
2860 DATA S,2,L,2,11,2,15
2870 DATA END
2880 DATA cuisine
2890 DATA L,2,276,384,4
2900 DATA D,2,H,6,5,6,22
2910 DATA W,1,H,10,22
2920 DATA W,1,V,14,13
2930 DATA D,1,V,2,13
2940 DATA S,1,L,2,16
2950 DATA END
2960 DATA lingerie
2970 DATA L,2,276,256,4
2980 DATA D,1,V,10,12
2990 DATA S,1,R,10,11
3000 DATA END
3010 DATA 5,4,8,21,0,4,3,2
3020 DATA 3,4,9,21,-1,-1,1,-1
3030 DATA 3,4,9,21,-1,-1,-1,1
3040 DATA 3,6,13,21,1,0,-1,5
3050 DATA 3,6,9,21,-1,-1,4,-1
3060 DATA L,64,308,480,100
3070 DATA F,250,98,294,102
3080 DATA F,250,306,294,310
3090 DATA F,390,94,430,106
3100 DATA F,390,302,430,314
3110 DATA F,474,240,488,270
3120 DATA F,474,124,488,154
3130 DATA F,58,240,72,270
3140 DATA L,226,308,322,180
3150 DATA L,160,180,480,100
3160 DATA L,64,180,160,100
3170 DATA END