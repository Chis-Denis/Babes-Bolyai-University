function out=lab3_1(p,l,k)
    out = randsample([0 1],l*k,true,[1-p p]);
    out = reshape(out,l,k);
end