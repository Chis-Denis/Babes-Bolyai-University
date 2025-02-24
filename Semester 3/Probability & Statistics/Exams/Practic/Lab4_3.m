%simulate Geo(p) r.vrb
p = input('prob of succes = ');
X = 0;
while rand>=p
    X = X+1;
end
N = input('Nr of simulations = ');
for i=1:N
    X(i) = 0;
    while rand>=p
        X(i) = X(i)+1;
    end
end

%Compare to the Geo(p) distr. graphically
UX = unique(X);
nX = hist(X, length(UX));
relfreq = nX/N;
k = 0:20;
pk = geopdf(k,p);
clf;    
plot(k, pk, '*', UX, relfreq, 'ro')
legend('Geo Distr', 'Simulation')
