% Problem 6.3.10
close all
figure()
Energy = @(Y) [Y(1) * Y(2); Y(1) * Y(1) - Y(2)];
y1 = linspace(-4,4,20);
y2 = linspace(-4,4,20);
[x,y] = meshgrid(y1,y2);
u = zeros(size(x));
v = zeros(size(x));

for i = 1:numel(x)
    Yprime = Energy([x(i); y(i)]);
    u(i) = Yprime(1);
    v(i) = Yprime(2);
end
quiver(x,y,u,v,2,'r','LineWidth',1.5); figure(gcf)
xlabel('$y_1$','Interpreter','Latex')
ylabel('$y_2$','Interpreter','Latex')
set(gca,'FontSize',25,'FontName','times')
axis tight equal;
ax = gca;
ax.XAxisLocation = 'origin';
ax.YAxisLocation = 'origin';
exportgraphics(gcf,'6.3.10.png','Resolution',600)

%% Conservative Systems and Energy Functions: Problem 1.
close all
M = 1;
L = 1;
g = 1;
% Max velocity of pendulum is sqrt(2 * g * hmax)
maxv = sqrt(2 * g * L);
% Define energy grid
Energy = @(x,xdot) M.*L.*(g.*(1-cos(x)) + 0.5 * L * xdot .^2);
angles = linspace(-pi,pi,1000);
vs = linspace(-maxv,maxv,1000);
[anglesgrid, vsgrid] = meshgrid(angles,vs);
E_grid = Energy(anglesgrid,vsgrid);
xyint = [-pi pi -maxv maxv];

figure()
set(gcf,'Position',[0 0 600 800])
subplot(2,1,1);
hold on
fsurf(Energy, xyint);
scatter3(0,0,Energy(0,0),200,'r','filled')
contour3(anglesgrid, vsgrid, E_grid,[0 .05 .1 .25 .4 1.1 1.5 2 2.5 3],'k','LineWidth',3);
axis tight
xlabel("$\theta$",'Interpreter','latex')
ylabel("$\dot{\theta}$",'Interpreter','latex')
zlabel("$E$",'Interpreter','latex')
set(gca,'FontSize',30,'FontName','Times')

subplot(2,1,2);
contour3(anglesgrid, vsgrid, E_grid,[0 .05 .1 .25 .4 1.1 1.5 2 2.5 3],'k','LineWidth',3);
view([0 90])
xlabel("$\theta$",'Interpreter','latex')
ylabel("$\dot{\theta}$",'Interpreter','latex')
hold on
scatter3(0,0,Energy(0,0),200,'r','filled')
text(-.75,1.1,'$center$','Interpreter','latex','FontSize',40)
set(gca,'FontSize',30,'FontName','Times')

%exportgraphics(gcf,'Conservative.2.png','Resolution',600)

%% Two-Body Problem
close all
m = 0.18;
M = 1;
G = 1;
L = 0.1;
r = linspace(0.1,5,1000);
rdot = linspace(-5,5,1000);

rdot = linspace(-2.5,2.5,1000);
r = linspace(0.15,10,1000);
make_2body_plot(m,M,G,L,r,rdot);

%% Three Body Problem Contour Plot
clear all; close all;
Ms = 1.989 * 10^(30); % kg
Me = 5.972 * 10^(24); % kg
re = 148.04 * 10^9; % m radius of earth's orbit
w = 1.99 * 10^(-7); % s^-1
G = 6.67408 * 10^(-11);

%{
Ms = 100;
Me = 5;
re = 10;
G = 1;
w = sqrt(G * (Ms + Me) / (re^3))
%}
V = @(r,theta) - (G .* Ms) ./ r - (G .* Me) ./ sqrt((r.*cos(theta) - re).^2 + (r.*sin(theta)).^2) - ((r.^2) .* (w.^2) ./ 2);

rs = linspace(0.5 * re,2*re, 300);
thetas = linspace(0, 2*pi,300); 
[r,theta] = meshgrid(rs,thetas);
x = r.* cos(theta);
y = r.* sin(theta);

figure()
xlim([-max(rs) max(rs)])
ylim([min(y(:)) max(y(:))])
hold on
set(gcf,'Position',[0 0 800 800])
%contour(x,y,V(r,theta),100,'LineWidth',2);
contour(x,y,V(r,theta),10^9 .* [-10:.01:-1],'LineWidth',2);
scatter(0,0,'r','filled')
scatter(re,0,'b','filled')
colorbar
set(gca,'FontSize',30,'FontName','Times')
xlabel("$rcos(\theta)$",'Interpreter','latex');
ylabel("$rsin(\theta)$",'Interpreter','latex');
exportgraphics(gcf,"three_body_prob_phys.png",'Resolution',600);
function make_2body_plot(m,M,G,L,r,rdot)
    rstar = (M+m)*L^2 / (G*M*M*m*m);
    figure()
    set(gcf,'Position',[0 0 600 800])
    subplot(2,1,1);
    Energy = @(r,rdot) -G*M*m./r + (1/2) .* ( ((M + m)./(M * m)) .* (L^2 ./ r.^2) ...
                        + ((M.*m)./(M+m)) .* rdot.^2);
    xyint = [min(r) max(r) min(rdot) max(rdot)];
    hold on
    fsurf(Energy, xyint);
    % Make txhe mesh
    [rgrid, rdotgrid] = meshgrid(r,rdot);
    E_grid = Energy(rgrid,rdotgrid);
    levels = [-.05 0 .2 1 1.5];
    contour3(rgrid,rdotgrid,E_grid,[-.15 -.15001],'--c','LineWidth',5);
    contour3(rgrid,rdotgrid,E_grid,[0 0.0001],'--k','LineWidth',5);
    contour3(rgrid,rdotgrid,E_grid,[.2 .20001],'--m','LineWidth',5);

    axis tight
    title("$L = " + L + ", M = " + M + ", m = " + m + "$",'Interpreter','latex')
    xlabel("$r$",'Interpreter','latex')
    ylabel("$\dot{r}$",'Interpreter','latex')
    zlabel("$E$",'Interpreter','latex')
    set(gca,'FontSize',30,'FontName','Times')
    scatter3(rstar,0,Energy(rstar,0),100,'r','filled')
    xlim(xyint(1:2))
    ylim(xyint(3:4))
    zlim([-.3 .85])



    subplot(2,1,2);
    hold on
    contour3(rgrid,rdotgrid,E_grid,[-.15 -.15001],'--c','LineWidth',3.5);
    contour3(rgrid,rdotgrid,E_grid,[0 0.0001],'--k','LineWidth',3.5);
    contour3(rgrid,rdotgrid,E_grid,[.2 .20001],'--m','LineWidth',3.5);
    view([0 90])

    title("$L = " + L + ", M = " + M + ", m = " + m + "$",'Interpreter','latex')
    
    % Make arrows on level sets 
    [x,y] = meshgrid(downsample(r(20:end),50),downsample(rdot(1:end),50));
    drdot = @(r,rdot) ((M+m)./(M.*m)).^2 * (L^2 ./ (r.^3)) - G.*(M+m) ./ (r.^2);
    drdotdata = drdot(x,y);
    quiver(x,y,y,drdotdata,1.5,'r','LineWidth',1.5);
    xlabel("$r$",'Interpreter','latex')
    ylabel("$\dot{r}$",'Interpreter','latex')
    set(gca,'FontSize',30,'FontName','Times')
    scatter(rstar,0,100,'r','filled')
    xlim(xyint(1:2))
    ylim(xyint(3:4))
    

    %exportgraphics(gcf,titl + ".png",'Resolution',600);

end 