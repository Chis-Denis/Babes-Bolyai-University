function out = lab3_3ii(p,m)
    out = zeros(1,m);
    for i=1:m
        while ~lab3_1(p,1,1)  % bernoulli
            out(i)+=1;
        end
    end
end 