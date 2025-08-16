function lab2_2i(N)
    hold on;
    axis square; axis off;
    rectangle('Position', [0 0 1 1], 'FaceColor', 'W');
    rectangle('Position', [0 0 1 1], 'Curvature', [1 1]);
    c = 0;
    for i=1:N
        x=rand; y=rand;
        P=[x,y];
        C=[0.5, 0.5];
        if pdist([P;C])<=0.5
            plot(x,y, 'or', 'MarkerSize', 3, 'MarkerFaceColor', 'r');
            c=c+1;
        end
    end
    p=c/N
end