%teta-target parameter
%null H0: teta = teta0
%alt. H1: teta < teta0 left-tailed
%         teta > teta0 right-tailed
%         teta = teta0 two-tailed
%Decision rejection H0
%         to not reject H0
%Given d belongs to (0,1) significance level 
%P(type I error) = P(rej. H0|H0) = alfa
%Test stat. TS = P(TS0 belongs to RR|teta = teta0)
%Observed value of the TS, TS0 = TS(teta = teta0)
%Rej. region, RR
%TS0 belongs to RR yes-rej.H0
%                  no-not rej. H0
%...

%tests for 1 Pop.
alfa = input('sign. level(in(0,1))=');
%a)
m0=input('test value=');%9
%H0:miu = m0 (9) ( > m0)
%H1:miu < m0 left-tailed
X=[7 7 4  5  9 9 ...
   4 12 8 1  8 7 ...
   3 13 2 1 17 7 ...
   12 5 6 2  1 13 ...
   14 10 2 4 9 11 ...
   3 5 12 6 10 7 ...
   ];%sample data
sigma=5;%case sigma 
%z-test
%[h,p,ci,zval] = ztest(X,m0,sigma,alfa,tail)
%alfa = def. 0.05
%h-0->H0 not rej.
% -1->H0 is rej.
%P-value
%tail = -1 or 'left'
%        0 or 'both'
%        1 or 'right'
tail=-1;
[h,p,ci,zval] = ztest(X,m0,sigma,alfa,tail);

if h==0
    fprintf('H0 is not rej., i.e the efficiency standard is met \n');
else
    fprintf('H0 is rej., i.e the efficiency standard is not met\n');
end

fprintf('Observed value of the test statistic is %3.5f\n',zval);
fprintf('P value is %1.5f\n',p);
q1 = norminv(alfa,0,1);
fprintf('Rej. region RR is (-inf,%3.5f)\n',q1);
