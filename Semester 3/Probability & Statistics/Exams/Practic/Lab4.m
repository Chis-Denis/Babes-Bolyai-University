%simulate Bern(p) random variable
p = input ('p in (0,1)');
U = rand;
X = U-p; 

%Generate a sample of N variables
N = input('number of simulations: '); %10, 100, 1000, ...
for i=1:N
    U = rand;
    X(i)=U<p;
end

%compare to the Bern distribution
UX = unique(X);
nX = hist(X, length(UX));
relfreq = nX/N

