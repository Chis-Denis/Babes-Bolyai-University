% a. Difference of two sets
diff([], _, []).
diff([F|L], S2, R) :- in_set(F, S2), diff(L, S2, R).
diff([F|L], S2, [F|R]) :- diff(L, S2, R), \+ in_set(F, S2).


in_set(X, [X|_]). % X is first
in_set(X, [_|T]) :- in_set(X, T).  % go through the whole list

%diff(l1..ln, k1..km)=
%			[l1..ln]			m=0
%			[]				n=0
%			l1 U diff(l2..ln, k1..km)	l1 \+ in set S2
%			diff(l2..ln, k1..km)		l1 in_set S2
%in_set(X, l1..ln)=
%			true				l1==X
%			in_set(X, l2..ln)		l1!=X
%
%
%diff([1,2,3,4], [2,4], R).
%R = [1, 3]

%F - first element in the list
%L - last element in the list
%R - result list
%S2 - second list
