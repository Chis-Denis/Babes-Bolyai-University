function out = lab3_4ii()
    clf;
    hold on;
    p=0.5; k=10;m=20;
    poz = lab3_4i(p,k,m);
    N = histc(poz(:,end),-k:k);
    bar(-k:k,N/m,'hist','FaceColor','b');
    pr = zeros(1,2*k+1);
    for sr = 0:k
        sl = k-sr;
        pf = sr-sl;
        pr (pf+k+1) = binopdf(sr,k,p);
    end
    bar(-k:k,pr,'FaceColor','y');
    set(gca,'XTick',-k:k); grid on
end 