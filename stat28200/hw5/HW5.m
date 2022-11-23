% 7.3.2

[x,y] = meshgrid(-1.25:.1:1.25,-1.25:.1:1.25);
xdot = x - y - x * (x.^2 + 5 * y.^2);
ydot = x + y - y * (x.^2 + y.^2);

figure()
hold on
quiver(x,y,xdot,ydot,'r','LineWidth',2);
f1 = @ode_732;
plot_solve([.1 0],f1)
plot_solve([-.1 0],f1)
plot_solve([0 .1 ],f1)
plot_solve([0 -.1 ],f1)
plot_solve([1.2 0],f1)
plot_solve([1.2 1.2],f1)
plot_solve([1.2 -1.2],f1)
plot_solve([-1.2 -1.2],f1)
plot_solve([-1.2 1.2],f1)
plot_solve([-1.2 0],f1)
plot_solve([0 1.2],f1)
plot_solve([0 -1.2],f1)
set(gca,'FontSize',30,'FontName','times')
xlabel("$x$",'Interpreter','latex')
ylabel("$y$",'Interpreter','latex')
rectangle('Position',[-1/sqrt(2) -1/sqrt(2) 2.*1/sqrt(2) 2.*1/sqrt(2)],'Curvature',[1 1],'EdgeColor','b','LineWidth',2)
rectangle('Position',[-1 -1 2.*1 2.*1],'Curvature',[1 1],'EdgeColor','b','LineWidth',2)
exportgraphics(gcf,"7.3.2_plot.png","Resolution",600)

%% 8.2.3 
close all
f2 = @ode_823;
mu = 0.01
figure()
hold on
[x,y] = meshgrid(-1:.1:1,-1:.1:1);
xdot = -y + (mu.*x) + (x .* y.^2);
ydot = x + (mu.*y) - (x.^2);
quiver(x,y,xdot,ydot,'r','LineWidth',2);
initial_pos = [.5 .5; .1 0; 0 .1; -.2 0; 0 -.3; 0 0; -.5 -.5; .5 0; 0 .5; .5 .5; -.5 .5; .5 -.5; -.5 0; .95 0; -.8 0; -.6 0];
for i = 1:length(initial_pos)
    scatter(initial_pos(i,1),initial_pos(i,2),'filled','b');
    plot_solve(initial_pos(i,:),f2,mu)
end
xlim([min(x(:)) max(x(:))])
ylim([min(y(:)) max(y(:))])
set(gca,'FontSize',30,'FontName','times')
xlabel("$x$",'Interpreter','latex')
title("$\mu = " + mu + "$",'Interpreter','latex')
ylabel("$y$",'Interpreter','latex')
exportgraphics(gcf,"Hopf_823_phase_mu" + mu + ".png",'Resolution',600)

%% 8.2.4
close all
figure()
hold on
mu = -.01;
rlimcycle = sqrt(-8 * mu);
rs = linspace(0,1.5*rlimcycle,20);
thetas = linspace(0,2*pi,20);
[r, theta] = meshgrid(rs,thetas);
rdot = mu .* r + (1/8) .* r.^3;
thetadot = ones(size(theta));
x = r.* cos(theta);
y = r.* sin(theta);
xdot = rdot .* cos(thetadot) - r .* thetadot .* sin(theta);
ydot = rdot .* sin(thetadot) + r .* thetadot .* cos(theta);
quiver(x,y,xdot,ydot,'r','LineWidth',2);

% in r and theta
initial_pos = rlimcycle.*[0 0; .1 0; .4 0; .9 0; 1.1 0; 1 0; 1.2 pi; 0.95 -pi; 1.05 3*pi/2];
for i = 1:length(initial_pos)
    [xp,yp] = pol2cart(initial_pos(i,2),initial_pos(i,1));
    scatter(xp,yp,'filled','b');
    plot_solve_polar(initial_pos(i,:),mu)
end
set(gca,'FontSize',30,'FontName','times')
hold on
p = nsidedpoly(1000, 'Center', [0 0], 'Radius', rlimcycle);
plot(p, 'EdgeColor', 'g','LineWidth',2,'FaceAlpha',0)
xlim([min(x(:)) max(x(:))])
ylim([min(y(:)) max(y(:))])
xlabel("$x$",'Interpreter','latex')
title("$\mu = " + mu + "$",'Interpreter','latex')
ylabel("$y$",'Interpreter','latex')
exportgraphics(gcf,"Heuristic_824" + mu + ".png",'Resolution',600)

%% 8.2.11

close all
figure()
hold on
[x,y] = meshgrid(-2:.1:2, -2:.1:2);
mu = 0
xdot = y;
ydot = x.^3 - x - mu .* y;
quiver(x,y,xdot,ydot,'r','LineWidth',2);
f1 = @ode_8211;
quiver(x,y,xdot,ydot,'r','LineWidth',2);
% in r and theta
initial_pos = [1.5 0; -1.5 0; 0 -1.5; 0 1.5; 1.5 -1.5; -1.5 1.5; 0 0; -.1 0; .1 .1; -.5 -.5; .3 .3; .9 .9; -.6 0; .6 0; 1 0; -1 0; 0 1; 0 -1; 0 .8; 0 .6; .7 0; .8 0; .9 0; 1.1 0; 0 -.8; 0 -.9; -.8 0; -.7 0; -.9 0; -1.1 0]
for i = 1:length(initial_pos)
    scatter(initial_pos(i,1),initial_pos(i,2),'filled','b');
    plot_solve(initial_pos(i,:),f1,mu);
end
set(gca,'FontSize',30,'FontName','times')
hold on
xlim([min(x(:)) max(x(:))])
ylim([min(y(:)) max(y(:))])
xlabel("$x$",'Interpreter','latex')
title("$\mu = " + mu + "$",'Interpreter','latex')
ylabel("$\dot x$",'Interpreter','latex')
exportgraphics(gcf,"PhasePlot_8211_" + mu + ".png",'Resolution',600)

%% 8.2.17e 
close all
figure(1)
hold on
figure(2)
hold on
tspan = 0:.05:1000;
T = 100;
b = 5;
g = 1;
I = 5;

[x1, y1, x2, y2] = ndgrid([0:.15:3]); 
F = @(x) 1 ./ (1+exp(-x));
x1dot = -x1 + F(I - b.*x2 - g.*y1);
x2dot = -x2 + F(I-b.*x1-g.*y2);
y1i = 10;
y2i = 5;
figure(1)
quiver(x1(:,y1i,:,y2i),x2(:,y1i,:,y2i),x1dot(:,y1i,:,y2i),x2dot(:,y1i,:,y2i),'r','LineWidth',2);

y1o = y1(y1i,y1i,y1i,y1i)
y2o = y2(y2i,y2i,y2i,y2i);
initial_pos = [1 y1o 2.5 y2o; 2 y1o 1 y2o; y1o .1 3 y2o; 3 y1o .5 y2o; .1 y1o .5 y2o;.3 y1o .1 y2o];
for i = 1:length(initial_pos)
    
    y0 = initial_pos(i,:);
    [~,sol] = ode45(@(t,y) ode_8217(t,y,b,g,T,I),tspan,y0);
    figure(1)
    scatter(y0(1),y0(3),'filled','b');
    plot(sol(:,1), sol(:,3),'k','LineWidth',1);

    figure(2)
    scatter(y0(2),y0(4),'filled','b');
    plot(sol(:,2), sol(:,4),'k','LineWidth',1);
end

% Solve for fixed point
syms u
ustar = solve(F(I - b.*u - g.*u) == u, u);

figure(1)
xlim([min(x1(:)) max(x1(:))])
ylim([min(x2(:)) max(x2(:))])
set(gca,'FontSize',30,'FontName','times')
hold on
xlabel("$x_1$",'Interpreter','latex')
ylabel("$x_2$",'Interpreter','latex')
scatter(ustar,ustar,100,"g",'filled');

figure(2)
xlim([min(y1(:)) max(y1(:))])
ylim([min(y2(:)) max(y2(:))])
set(gca,'FontSize',30,'FontName','times')
hold on
xlabel("$y_1$",'Interpreter','latex')
ylabel("$y_2$",'Interpreter','latex')
scatter(ustar,ustar,100,"g",'filled');

figure(1)
title("$b = " + b + ",T = " + T  + ",g = " + g + ",I = " + I + "$",'Interpreter','latex')
exportgraphics(gcf,"xs_HopfNeurons_b_" + b + "_T" + T + "_g" + g+ "I_" + I + ".png",'Resolution',600)

figure(2)
title("$b = " + b + ",T = " + T  + ",g = " + g + ",I = " + I + "$",'Interpreter','latex')
exportgraphics(gcf,"ys_HopfNeurons_b_" + b + "_T" + T + "_g" + g+ "I_" + I + ".png",'Resolution',600)

function plot_solve(y0, myode,mu)
    tspan = 0:.01:100;
    [~,sol] = ode45(@(t,y) myode(t,y,mu),tspan,y0);
    plot(sol(:,1), sol(:,2),'k','LineWidth',1);
end 
function plot_solve_polar(y0,mu)
    tspan = 0:.1:100;    
    % y vector here has r and theta 
    [~,sol] = ode45(@(t,y) ode_polar_824(t,y,mu),tspan,y0);
    % Convert solution from polar coor to cartesian coor 
    x = sol(:,1).* cos(sol(:,2));
    y = sol(:,1).* sin(sol(:,2));
    plot(x,y,'k','LineWidth',1);
end 
function dydt = ode_polar_824(~,yvec,mu)
    % compute polar derivative
    r = yvec(1);
    theta = yvec(2);
    rdot = mu .* r + (1/8) .* r.^3;
    thetadot = ones(size(theta));
    dydt = [rdot; thetadot];
end
function dydt = ode_732(~,yvec)
    x = yvec(1);
    y = yvec(2);
    dydt = [x - y - x * (x.^2 + 5 * y.^2); x + y - y * (x.^2 + y.^2)];
end
function dydt = ode_823(~,yvec,mu)
    x = yvec(1);
    y = yvec(2);
    dydt = [-y + (mu.*x) + (x .* y.^2); x + (mu.*y) - (x.^2)];
end
function dydt = ode_8211(~,yvec,mu)
    x = yvec(1);
    y = yvec(2);
    dydt = [y; x.^3 - x - mu .* y];
end
function dydt = ode_8217(~,yvec,b,g,T,I)
    x1 = yvec(1);
    y1 = yvec(2);
    x2 = yvec(3);
    y2 = yvec(4);
    F = @(x) 1 ./ (1+exp(-x));
    dydt = [-x1 + F(I - b.*x2 - g.*y1); 
            (x1 - y1) ./ T;
            -x2 + F(I-b.*x1-g.*y2);
            (-y2 + x2) ./ T];
end