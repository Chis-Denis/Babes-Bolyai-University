%simulate Bino(n,p) rand val
n=input('nr of trials=');
p=input('probability of success=');
%gen 1 variable
U=rand(n,1);
X=sum(U<p);
%gen a sample of vars
N=input('nr of simulation='); %10,1e2,1e3,1e4
for i=1:N
    U = rand(n,1);
    X(i)=sum(U<p);
end
%compare it to the Binomial(n,p) distribution graphically
UX=unique(X); %distinct values
nX=hist(X,length(UX));%nr of each distinct value=the frequence
relfr=nX/N;
%graph
k=0:n;
pk=binopdf(k,n,p);
clf;
plot(k,pk,'*',UX,relfr,'r+')
legend('Bino distr','Simulation')