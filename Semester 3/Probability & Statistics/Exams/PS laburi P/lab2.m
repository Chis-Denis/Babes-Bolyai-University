%n=input('Number of trials(in natural numbers): ');
%p=input('Probability of success:(in the inteval (0,1)): ');
%xpdf=0:n;
%ypdf=binopdf(xpdf,n,p);
%plot(xpdf,ypdf,'+--');

%xcdf=0:0.01:n;
%ycdf=binocdf(xcdf,n,p);
%plot(xcdf,ycdf,'+');

xpdf=0:3;
ypdf=binopdf(xpdf,3,1/2);
plot(xpdf,ypdf,'+-');

xcdf=0:0.1:3;
ycdf=binocdf(xcdf,3,1/2);
plot(xcdf,ycdf,'*-');

%legend('xcdf','ycdf');
%legend('xpdf','ypdf');

%a) P(X=0),P(x!=1)
pa1=binopdf(0,3,1/2);
pa2=1-binopdf(1,3,1/2);


%d) P(x<=2),P(x<2)
pd1=binocdf(2,3,1/2);

%d) P(x<2)=P(x<=1)(1.5,1.9,1.9999)
pd2=binocdf(1,3,1/2);

%e) P(x>=1), P(x>1)
%P(x>=1)=1-P(x<1)
pe1=1-binocdf(1,3,1/2);
%P(a<x<=b)=F(b)-F(a)

fprintf('Prob1. in part a) is %1.4f\n',pa1);
fprintf('Prob2. in part a) is %1.4f\n',pa2);
fprintf('Prob1. in part d) is %1.4f\n',pd1);
fprintf('Prob2. in part d) is %1.4f\n',pd2);
fprintf('Prob1. in part e) is %1.4f\n',pe1);
