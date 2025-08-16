%generare(L:list, S:integer, C:List)

generare([E|_],E,[E]).
generare([_|T], S,C):-generare(T,S,C).
generare([H|T], S, [H|C]):-
	S-H > 0,
	S1 is S-H,
	generare(T,S1,C).




getAll(L1,S,L2):-
	findall(C,generare(L1,S,C),L2).