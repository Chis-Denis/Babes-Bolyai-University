%Simulate Bern(p) distribution
p=input('p(in(0,1))=');%parameters of distribution

%Generate one variable
U=rand;
X=(U<p);%define success U<p, failure U>=p

%Generate a sample
N=input('Number of simulations= ');%10,1e2,1e4,le5-10 to the power of 2,4,5
for i=1:N
    U=rand;
    X(i)=(U<p);%define success U<p, failure U>=p
end

%Compare to the Bern(p) distribution
UX=unique(X) %distribution values of X
nX=hist(X,length(UX)); %histogram->recatngle telling us how many elements are in each class
%counts how many of each

relfreq=nX/N
%the number of occurences of a succes

%------

%Simulate Bino(p) distribution
n=input('nb of trials=');
p=input('prob of success=');%parameters

%Generate one variable
U=rand(n,1);
X=sum(U<p);%define success U<p, failure U>=p

%Generate a sample
N=input('Nb of simulation=');
for i=1:N
    U=rand(n,1);
    X(i)=sum(U<p);
end

%Compare to the Bino(n,p) distribution
UX=unique(X) %distinct values of X
nX=hist(X,length(UX)); %histogram->recatngle telling us how many elements are in each class
%counts how many of each

relfreq=nX/N
%Compare geographically
k=0:n;
pk=binopdf(k,n,p);
plot(k,pk,'*',UX,relfreq,'ro')
legend('Bino distr','simulation')




    
