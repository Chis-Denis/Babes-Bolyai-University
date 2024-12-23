% b. Add 1 after every even element
add1([], []).
add1([F|L], [F,1|R]) :- 0 is F mod 2, add1(L, R).
add1([F|L], [F|R]) :- 1 is F mod 2, add1(L, R).

%add1(l1..ln) =
%		[]				n=0
%		l1 U 1 U add1(l2...ln)		0 is l1 mod 2
%		l1 U add1(l2..ln)		1 is l1 mod 2
%
%		add1([1,2,3,4], R).
%		R = [1, 2, 1, 3, 4, 1]

%F - first element in the list
%L - last elemnt in the list
%R - result list
