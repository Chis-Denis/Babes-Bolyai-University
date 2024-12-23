A=[1 0 -2; 2 1 3; 0 1 0]; %matrix A
B=[2 1 1; 1 0 -1; 1 1 0]; %matrix B
C=A-B                    %matrix C' 
C=A-B;                   %matrix C  
fprintf("Matrix C is:\n");
fprintf("%2d %2d %2d\n",C); 
D=A*B;                   %matrix D
fprintf("Matrix D is:\n");
fprintf("%2d %2d %2d\n",D); 
D=A.*B;                  %matrix D
fprintf("Matrix D is:\n");
fprintf("%2d %2d %2d\n",D); 
