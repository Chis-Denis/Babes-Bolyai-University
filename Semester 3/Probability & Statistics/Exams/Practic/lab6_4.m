%test for 2 populations
alpha = input('significance level (0,1) = ');
%h0: sigma1  = sigma2
%h1: sigma1 >= sigma2 - rigth-tailed test

X1 = [22.4 21.7 ...
    24.5 23.4 ...
    21.6 23.3 ...
    22.4 21.6 ...
    24.8 20.0]; %sample test 1

X2 = [17.7 14.8 ...
    19.6 19.6 ...
    12.1 14.8 ... 
    15.4 12.6 ...
    14.0 12.2]; %sample test 2
%tail values: -1 for left tailed
%           : 0 for both, default
%           : 1 for rigth tailed
tail = 1;
[h, p, ci, stats] = ttest2(X1,X2,alpha,tail,'equal');
%p-P value; ci = confidence level

if h == 0
    fprintf('H0 is not rejected, i.e, sigmas are equal\n');
else 
    fprintf('H0 is rejected\n');
end

q1 = tinv(1-alpha, stats.df);
fprintf('Observed value is %1.4f\n', stats.tstat);
fprintf('P-value is %e\n', p);
fprintf('Rejection region R is (%3.4f, inf)\n', q1); 


