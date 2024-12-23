%10. For a list a1... an with integer and distinct numbers, define a
%predicate to determine all subsets with
%sum of elements divisible with n


% Predicate to compute subsets of a list(L:list, F:list)
subset([], []).
subset([H|T], [H|Subset]) :-
    subset(T, Subset).
subset([_|T], Subset) :-
    subset(T, Subset).

% Predicate to calculate the sum of elements in a list(L:list, Sum:Integer)
sum_list([], 0).
sum_list([H|T], Sum) :-
    sum_list(T, RestSum),
    Sum is H + RestSum.

% Predicate to find subsets whose sum is divisible by N
%(List:list, N:integer, Subsets:list)
subsets_divisible_by_n(List, N, Subsets) :-
    findall(Subset,
            (subset(List, Subset),
             sum_list(Subset, Sum),
             Sum mod N =:= 0),
            Subsets).

%subsets_divisible_by_n(l1..ln, N):
%                      subset U Subsets ,sum % N == 0
%                      subset(List, Subset(sum_list(Subset, Sum)))

%subset(l1..ln):
%      [], n=0
%      l1 U subset(l2..ln), true
%      subset(l2..ln), true

%sum_list(l1..ln)
%
%        Sum = l1 + Sum, sum_list(l2..ln)
%

%flow_model:
%          subsets_divisible_by_n(i,i,o)
%          subset(i,o)
%          sum_list(i,o)

%subsets_divisible_by_n([1, 2, 3, 4], 3, Subsets).
%Subsets = [[1, 2, 3], [1, 2], [2, 3, 4], [2, 4], [3], []].
%
%subsets_divisible_by_n([3, 5, 6, 1], 4, Subsets).
%Subsets = [[3, 5], [3, 1], [5, 6, 1], []].
