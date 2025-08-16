function lab2_2ii(N)
    hold on;
    axis square; axis off;
    rectangle('Position', [0 0 1 1], 'FaceColor', 'W');
    rectangle('Position', [0 0 1 1], 'Curvature', [1 1]);
    c = 0;
    for i=1:N;
        x=rand; y=rand;
        P=[x,y];
        C=[0.5, 0.5]
        V1=[0, 0]
        V2=[0, 1]
        V3=[1, 0]
        V4=[1, 1]
        d = pdist([P;C]);
        if d<pdist([P;V1]) && d<pdist([P;V2]) && d<pdist([P;V3]) && d<pdist([P;V4])
            plot (x, y, 'or', 'MarkerSize',3,'MarkerFaceColor', 'r');
            c=c+1
        end
    end
    p=c/N
end