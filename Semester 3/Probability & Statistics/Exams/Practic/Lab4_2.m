%simulate Bino(n,p) r.vrb
n = input('nr of trials = ');
p = input('prob of succes = ');
U = rand(n,1);
X = sum(U<p);
N = input('Nr of simulations = ');
for i=1:N
    U = rand(n,1);
    X(i) = sum(U<p);
end

%Compare to the Bino(x,p) distr. graphically
UX = unique(X);
nX = hist(X, length(UX));
relfreq = nX/N;
k = 0:n;
pk = binopdf(k,n,p);
clf;
plot(k, pk, '*', UX, relfreq, 'ro')
legend('Bino Distr', 'Simulation')
