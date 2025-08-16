function p= lab2_1iiii()
    a = 0;
    c = 0;
    u = ['r', 'r', 'r', 'r', 'r', 'r', 'a','a','a','v', 'v'];
    for i=1:2000
        b=randsample(u, 3, 'false');
        if b(1) == 'r' || b(2) == 'r' || b(3) == 'r'
            a = a+1;
            if b(1) == 'r' && b(2) == 'r' && b(3) == 'r'
                c = c+1;
            end
        end
    end
    p = c/a;
end    
