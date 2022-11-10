5 REM Fills the screen with snowflakes
10 cls
20 x$=string$(40," ")
30 pk = @X$
35 REM Get the actual address of the string X$
40 ad = peek(pk+2)*256+peek(pk+1)
50 for i = ad to len(x$)+ad
55 REM Replace space with snowflake
60 poke i,238
70 next
76 REM Print 24 lines of snowflakes
80 for i = 1 to 24
90 print x$;
100 next
