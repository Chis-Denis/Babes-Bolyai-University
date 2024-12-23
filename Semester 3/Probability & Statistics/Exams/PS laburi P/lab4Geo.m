%Simulate Geo(p) distribution
p=input('prob of success=');%parameters
X=0;
while rand>=p
    X=X+1; %count the number of failures before the 1st success
end

%Generate a sample
N=input('Nb of simulation=');
for i=1:N
    X(i)=0;
    while rand>=p
        X(i)=X(i)+1; %count the number of failures before the 1st success
    end
end

%Compare to the Bino(n,p) distribution
UX=unique(X); %distinct values of X
nX=hist(X,length(UX)); %histogram->recatngle telling us how many elements are in each class
%counts how many of each

relfreq=nX/N;
%Compare geographically
k=0:20;
pk=geopdf(k,p);
plot(k,pk,'*',UX,relfreq,'ro')
legend('Geo distr','simulation')