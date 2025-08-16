lungimePara([],E):-E mod 2 =:= 0.
lungimePara([_|T],E):- E1 is E+1,lungimePara(T,E1).

adaugSf([],E,[E]).
adaugSf([H|T],E,[H|C]):-adaugSf(T,E,C).

elimina([],_,C,C).
elimina(L,0,C,[C|L]).
elimina([H|T], N, C,R):- is_list(H),lungimePara(H,0),N1 is N-1,elimina(T,N1,C,R),!.
elimina([H|T],N,C,R):-adaugSf(C,H,C1),elimina(T,N,C1,R).