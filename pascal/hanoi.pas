   10 PROGRAM hanoi;
   20 VAR n : integer;
   30 PROCEDURE movedisk(sce, dest : integer);
   40 BEGIN
   50 write(sce:1, ' to ', dest:1, '. ')
   60 END;
   70 PROCEDURE move(n, sce, aux, dest : integer);
   80 BEGIN
   90 IF n = 1 THEN movedisk(sce, dest)
  100 ELSE
  110 BEGIN
  120 move(n-1,sce,dest,aux);
  130 movedisk(sce, dest);
  140 move(n-1,aux,sce,dest)
  150 END;
  160 END;
  170 (*MainBlock*)
  180 BEGIN
  190 write('Number of Discs? ');
  200 read(n); writeln;
  210 move(n,1,2,3)
  220 END.
