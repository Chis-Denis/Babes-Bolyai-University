clc;
clear all;

A = [1021, 980, 1017, 988, 1005, 998, 1014, 985, 995, 1004, 1030, 1015, 995, 1023];
B = [1070, 970, 993, 1013, 1006, 1002, 1014, 997, 1002, 1010, 975];

% a) 5% significance level, do the population variances seem to differ?
% H0: variances are equal
% H0: variances are different (right-tailed test)

n1 = length(A);
n2 = length(B);

alpha = 0.05;

A_var = var(A);
B_var = var(B);

tt = A_var / B_var;

f_1 = finv(alpha / 2, n1 - 1, n2 - 1);
f_2 = finv(1 - alpha / 2, n1 - 1, n2 - 1);

fprintf("Rejection interval: (-inf, %3.4f) U (%3.4f, inf)\n", f_1, f_2);
fprintf("Statistic: %3.4f\n", tt);

h = tt < f_1 || tt > f_2;

if h == 1
    fprintf("H0 rejected - variances are different.\n\n");
else 
    fprintf("H0 not rejected - variances are equal.\n\n");
end

[h, s, ci, stats] = vartest2(A, B, "Alpha", alpha);
h;
s;
ci;
stats;


% b) at 5% significance level, on average, does supplier A seem to be more
% reliable? 

% H0: | A_mean - 1000 | = | B_mean - 1000 | <=> A_ = B_ 
% H1: | A_mean - 1000 | < | B_mean - 1000 | <=> A_ < B_

A_mean = mean(A);
B_mean = mean(B);

A_ = abs(A_mean - 1000);
B_ = abs(B_mean - 1000);

s_p_2 = ((n1 - 1) * A_var + (n2 - 1) * B_var) / (n1 + n2 - 2);

tt = (A_ - B_) / (sqrt(s_p_2) * sqrt(1 / n1 + 1 / n2));

t_ = tinv(alpha, n1 + n2 - 2);

fprintf('Test statistic: %f\n', tt);
fprintf('Critical value: %f\n', t_);

h = tt < t_;

if h == 1
    fprintf("H0 rejected - A is as reliable as B.\n\n");
else 
    fprintf("H0 not rejected - A is not more reliable than B.\n\n");
end
