function p= lab2_1ii()
    p = 0;
    u = ['r', 'r', 'r', 'r', 'r', 'r', 'a','a','a','v', 'v'];
    for i=1:2000
        b=randsample(u, 3, 'false');
        if b(1) == 'r'6 && b(2) == 'r' && b(3) == 'r'
            p = p+1;
        end
    end
    p = p/2000;
end    
