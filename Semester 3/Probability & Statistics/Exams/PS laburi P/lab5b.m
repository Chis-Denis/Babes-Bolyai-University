%b)
conflevel=input('conf level(in(0,1)='); %1-alfa
alfa=1-conflevel;
X=[7 7 4 5 9 9 ...
   4 12 8 1 8 7 ...
   3 13 2 1 17 7 ...
   12 5 6 2 1 13 ...
   14 10 2 4 9 11 ...
   3 5 12 6 10 7];
s=std(X); %case sigma jnown 
n=length(X);
xbar=mean(X);

q1= tinv(1-alfa/2,n-1);
q2= tinv(alfa/2,n-1); %q2=q1
ci1=xbar-(s/(sqrt(n)))*q1;
ci2=xbar-(s/(sqrt(n)))*q2;%q2=q1 sym/
fprintf('C.I for the pop. mean, miu, case sigma unknown(general case) is (% 3.5f, %3.5f)\n',ci1,ci2)