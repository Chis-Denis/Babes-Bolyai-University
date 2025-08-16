adaugSf([],E,[E]).
adaugSf([H|T],E,[H|L]):-adaugSf(T,E,L).

adauga([],_,_,_,C,C).
adauga([H|T],E,N,NR,C,R):-
	N=:=NR,!,
	adaugSf(C,H,C1),adaugSf(C1,E,C2),
	NR1 is NR - 1,
	NR2 is 2 * NR1,
	NR3 is NR2 + 1,
	N1 is N + 1,
	adauga(T,E,N1,NR3,C2,R).
adauga([H|T],E,N,NR,C,R):-
	adaugSf(C,H,C1),
	N1 is N+1,
	adauga(T,E,N1,NR,C1,R).

adaugare(L,E,R):-
	adauga(L,E,1,3,[],R).



%calculul lui x la N
%pow(x:integer, n:integer,r:integer) (i,i,o)
pow(_,0,1):-!.
pow(X,1,X):-!.
pow(X,N,R):-N1 is N-1, pow(X,N1,R1) ,R is X * R1.

%pow2(X,M,N,C,R)(i,i,i,i,o)
pow2(_,M,M,C,C):-!.
pow2(X,M,N,C,R):-
	M+1 =< N,
	C1 is C * X,
	M1 is M + 1,
	pow2(X,M1,N,C1,R).


poww(X,N,R):-
	pow2(X,0,N,1,R).
