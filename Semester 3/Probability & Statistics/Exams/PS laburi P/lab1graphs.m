x=0:0.01:3;
y1=x.^4/10;
y2=x.*sin(x);
y3=cos(x);
plot(x,y1,'b^:',x,y2,'g*:',x,y3,'r.:');