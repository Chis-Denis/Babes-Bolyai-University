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

%tests for 2 Pop.
alfa = input('sign. level(in(0,1))=');
%a)
X1=[22.4 21.7 ...
    24.5 23.4 ...
    21.6 23.3 ...
    22.4 21.6 ...
    24.8 20.0 ... 
   ];%sample data

X2=[17.7 14.8 ...
    19.6 19.6 ...
    12.1 14.8 ...
    15.4 12.6 ...
    14.0 12.2 ...
   ];%sample data
%H0:sigma1=sigma2
%H1:sigma1!=sigrma2 two-tailed
%z-test
%[h,p,ci,stats] = vartest2(X,m0,alfa,tail)
%alfa = def. 0.05
%h-0->H0 not rej.
% -1->H0 is rej.
%P-value
%tail = -1 or 'left'
%        0 or 'both'
%        1 or 'right'
tail=0;
[h,p,ci,stats] = vartest2(X1,X2,alfa,tail);

if h==0
    fprintf('H0 is not rej., i.e the variances of the two populations are equal \n');
else
    fprintf('H0 is rej., i.e the variances of the two populations are not equal\n');
end

fprintf('Observed value of the test statistic is %3.5f\n',stats.fstat);
fprintf('P value is %1.5f\n',p);
q1 = finv(alfa/2,stats.df1,stats.df2);
q2 = finv(1-alfa/2,stats.df1,stats.df2);
fprintf('Rej. region RR is (-inf,%3.5f) U (%3.5f,inf)\n',q1,q2);

%tests for 2 Pop.
alfa = input('sign. level(in(0,1))=');
%b)
X1=[22.4 21.7 ...
    24.5 23.4 ...
    21.6 23.3 ...
    22.4 21.6 ...
    24.8 20.0 ... 
   ];%sample data

X2=[17.7 14.8 ...
    19.6 19.6 ...
    12.1 14.8 ...
    15.4 12.6 ...
    14.0 12.2 ...
   ];%sample data
%H0:miu1=miu2
%H1:miu1>miu2 right-tailed
%z-test
%[h,p,ci,zval] = ztest(X1,X2,alfa,'vartype')
%vartype='equal'
%vartype='unequal'
%alfa = def. 0.05
%h-0->H0 not rej.
% -1->H0 is rej.
%P-value
%tail = -1 or 'left'
%        0 or 'both'
%        1 or 'right'
tail=1;
[h,p,ci,stats] = ttest2(X1,X2,alfa,tail,'equal');

if h==0
    fprintf('H0 is not rej., i.e gas mileage seem to be higher, on average, when premium gasoline is not used \n');
else
    fprintf('H0 is rej., i.e gas mileage seem to be higher, on average, when premium gasoline is used\n');
end

fprintf('Observed value of the test statistic is %3.5f\n',stats.tstat);
fprintf('P value is %e\n',p);
q1 = tinv(1-alfa,stats.df);
fprintf('Rej. region RR is (%3.5f,inf)\n',q1);
%P>0.05 no signif.
%0.01<P<0.05 sign. diff.
%0.001<P<0.01 distinctly significance
%P<0.001 very significance
