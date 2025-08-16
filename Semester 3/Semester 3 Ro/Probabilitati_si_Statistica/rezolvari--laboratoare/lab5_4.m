function lab5_4(n)
	x = exprnd(15,1,n);
	y = unifrnd(5,15,1,n);
	i = (15 <= x) & (x <= 10 + y);

	prob_sim = mean(i) 
	prob_t = (3*exp(-5/3)-exp(-1))/2
end