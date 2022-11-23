clear

%% loop over mass ratio
ratios = 10.^linspace(0,-4,100);
for j = 1:length(ratios);
    ratio = ratios(j);
    
    %% constants
    G = 1;
    M_s  = 10; % sun
    m_e = ratio*M_s; % earth
    r_e = 10; % distance from earth to sun
    omega = sqrt(G*(M_s + m_e)/r_e^3); % radial velocity
    
    %% grid space
    n = 250;
    range = 15;
    xs = linspace(-range,range,n);
    ys = xs;
    
    [x_grid,y_grid] = meshgrid(xs,ys);
    x_list = reshape(x_grid,[n^2,1]);
    y_list = reshape(y_grid,[n^2,1]);
    
    %%
    d_e = @(x,y) sqrt((x - (M_s/(M_s + m_e))*r_e).^2 + y.^2); % distance to earth
    d_s = @(x,y) sqrt((x + (m_e/(M_s + m_e))*r_e).^2 + y.^2); % distance to sun
    r = @(x,y) sqrt(x.^2 + y.^2); % distance from center of coordinate system
    
    U = @(x,y) -(G*(M_s./d_s(x,y) + m_e./d_e(x,y)) + (1/2)*omega^2*r(x,y).^2);% potential when static
    
    %% evaluate
    us = U(x_list,y_list);
    us(us < -5) = nan;
    
    %% display
    figure(1)
    clf;
    hold on
    surf(xs,ys,reshape(us,[n,n]),'EdgeColor','none','FaceColor','interp')
    contour3(xs,ys,reshape(us,[n,n]),-10.^(linspace(log(max(-us))/log(10),log(min(-us))/log(10),80)),'k')
    scatter3([-(m_e/(M_s + m_e))*r_e,(M_s/(M_s + m_e))*r_e],[0,0],[0,0],20,'r','fill')
    plot3([-(m_e/(M_s + m_e))*r_e,(M_s/(M_s + m_e))*r_e],[0,0],[0,0],'r','Linewidth',1)
    scatter3([0],[0],[0],20,'b','fill')
    colorbar
    grid on
    axis square
    view([0,0,1]);
    
    title_string = strcat('Mass ratio: ',{' '},num2str(ratio,2));
    title(title_string,'FontSize',16,'interpreter','latex')
    
    drawnow
    
end

