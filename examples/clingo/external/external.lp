#external p(1;2;3).
p(0) :- p(3).
%p(8) :- not p(0).
%p(0) :- not p(8).
p(0) :- not p(0).

#program succ(n).
#external p(n+3).
p(n) :- p(n+3).
p(n) :- not p(n+1), not p(n+2).
