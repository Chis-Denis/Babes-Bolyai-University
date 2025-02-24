%simulate Bern(p) rand var
p=input('p in (0,1)');
%gen 1 variable
U=rand;
X=(U<p);
%gen a sample of vars
N=input('nr of simulation='); %10,1e2,1e3,1e4
for i=1:N
    U = rand;
    X(i)=U<p;
end
%compare it to the Bern(p) distribution
UX=unique(X) %distinct values
nX=hist(X,length(UX));%nr of each distinct value=the frequence
relfr=nX/N
