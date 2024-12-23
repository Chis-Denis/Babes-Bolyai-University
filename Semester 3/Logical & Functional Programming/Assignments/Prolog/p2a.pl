%a. Merge two sorted lists with removing the double values.

% L1:list, L2:list, R:list (h1,t1 -first list; h2,t2- second list, R)
merge_unique([], L, L).
merge_unique(L, [], L).

merge_unique([H1|T1], [H2|T2], [H1|R]) :- H1 =:= H2, merge_unique(T1, T2, R).

merge_unique([H1|T1], [H2|T2], [H1|R]) :- H1 < H2, merge_unique(T1, [H2|T2], R).

merge_unique([H1|T1], [H2|T2], [H2|R]) :- H1 > H2, merge_unique([H1|T1], T2, R).



%merge_unique(l1..ln, k1..km)
%     l1..ln, m=0
%     k1..km, n=0
%     l1 U merge_unique(l2..ln, k2..km), l1==l2
%     l1 U merge_unique(l2..ln, k1..km), l1<l2
%     k1 U merge_unique(l1..ln, k2..km), l1>l2
%
%flow model:merge_unique(i,i,o)
%
%?- erge_unique([1, 3, 5, 7], [2, 3, 6, 7, 8], R).
%R = [1, 2, 3, 5, 6, 7, 8].
%
%?- merge_unique([4, 5, 6, 8, 10], [3, 5, 7, 8, 11], R).
%R = [3, 4, 5, 6, 7, 8, 10, 11].
