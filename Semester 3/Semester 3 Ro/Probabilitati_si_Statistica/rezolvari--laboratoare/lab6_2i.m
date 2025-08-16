function lab6_2i(n)
    g = @(x) exp(-x.^2);
    int_g_1 = lab6_2M1(g,-2,2,n)
    int_g_2 = lab6_2M2(g,-2,2,n)
    int_g_3 = quad(g,-2,2)
    
