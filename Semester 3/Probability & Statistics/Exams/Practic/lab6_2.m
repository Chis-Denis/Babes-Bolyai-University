%test for 1 population
alpha = input('significance level (0,1) = ');
m0 = input('test value = ');
%h0: miu  = m0 (9)
%h1: miu < m0 (9) left-tailed test
X = [7 7 4 5 9 9 ...
    4 12 8 1 8 7 ...
    3 13 2 1 17 7 ...
    12 5 6 2 1 13 ...
    14 10 2 4 9 11 ...
    3 5 12 6 10 7]; %sample test
%alpha default: 0.05
%tail values: -1 for left tailed
%           : 0 for both, default
%           : 1 for rigth tailed
tail = 1;
[h, p, ci, stats] = ttest(X, m0, alpha, tail);
%p-P value; ci = confidence level

if h == 0
    fprintf('H0 is not rejected, i.e, the average does NOT exceed\n');
else 
    fprintf('H0 is rejected\n');
end

fprintf('stats.tstat is %1.4f\n', stats.tstat);
fprintf('P-value is %1.4f\n', p);
q1 = tinv(1-alpha, stats.df);
fprintf('Rejection region R is (%3.4f, inf)\n', q1); 


