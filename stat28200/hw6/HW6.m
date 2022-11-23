% Kameel Khabaz
% CAAM 28200
% Homework 6 Lorenz Simulations

sigma = 10;
b = 8/3;
rH = (sigma * (sigma + b + 3))/(sigma - b -1);
r = rH - 1;
f = @myode;


figure()
close all;
hold on;
Cplus = [sqrt(b*(r-1)) sqrt(b*(r-1)) r-1];
initial_pos = [Cplus; Cplus + 1.* [1 1 1]];
for i = 1:2 %ength(initial_pos)
    scatter3(initial_pos(i,1),initial_pos(i,2),initial_pos(i,3),100,'filled','b');
    plot_solve(initial_pos(i,:),f,sigma, b, r)
end
scatter3(Cplus(1),Cplus(2),Cplus(3),100,'filled','r'); % C+ fixed point
set(gca,'FontSize',30,'FontName','times')
xlabel("$x$",'Interpreter','latex')
ylabel("$y$",'Interpreter','latex')
zlabel("$z$",'Interpreter','latex')
set(gcf,'Position',[0 0 700 500])
title("$r = " + r + ", r_H = " + rH+  ", \sigma = " + sigma + ", b = 8/3$",'Interpreter','latex')
%exportgraphics(gcf,"Lorenz_r" + r + ".png",'Resolution',600)
axis equal

%%
figure()
hold on
xlabel("$t$",'Interpreter','latex')
ylabel("$y$",'Interpreter','latex')
for i = 2 %ength(initial_pos)
    scatter(0,initial_pos(i,2),'filled','b');
    tspan = 0:.1:100;
    [~,sol] = ode45(@(t,y) myode(t,y,sigma, b, r),tspan,initial_pos(i,:));
    plot(tspan, sol(:,2),'k','LineWidth',.5);
end
set(gca,'FontSize',30,'FontName','times')

%% 2 neighboring trajectories 
sigma = 10;
b = 8/3;
rH = (sigma * (sigma + b + 3))/(sigma - b -1);
r = rH + 1;
f = @myode;


figure()
close all;
hold on;
delta = [.001 .001 .001];
Cplus = [sqrt(b*(r-1)) sqrt(b*(r-1)) r-1];
initial_pos = [Cplus; Cplus + delta];
cols = ['b' 'g']
solutions = {};
for i = 1:2 %ength(initial_pos)
    y0 = initial_pos(i,:);
    scatter3(initial_pos(i,1),initial_pos(i,2),initial_pos(i,3),100,'filled',cols(i));
    tspan = 0:.01:1000;
    opts = odeset('RelTol',1e-7,'AbsTol',1e-6);
    [~,sol] = ode45(@(t,y) myode(t,y,sigma, b, r),tspan,y0,opts);
    solutions{end+1} = sol;
    plot3(sol(:,1), sol(:,2),sol(:,3),'k','LineWidth',.5);
end

set(gca,'FontSize',30,'FontName','times')
xlabel("$x$",'Interpreter','latex')
ylabel("$y$",'Interpreter','latex')
zlabel("$z$",'Interpreter','latex')
set(gcf,'Position',[0 0 700 500])
title("$r = " + r + ", r_H = " + rH+  ", \sigma = " + sigma + ", b = 8/3$",'Interpreter','latex')
%exportgraphics(gcf,"Lorenz_r" + r + "2_trajectories.png",'Resolution',600)
axis equal

% Discrepancies plot
figure()
hold on;
discrep = vecnorm(solutions{1} - solutions{2},2,2);
normdiscrep = discrep ./ norm(delta);
normlogd = log(normdiscrep);
plot(tspan',normlogd,'k')
%{
% Now do the fit
linidcs = 1:find(tspan == 350);
f = fit(tspan(linidcs)',normlogd(linidcs),'poly1');
h = plot(f);
h.LineStyle = '--';
h.Color = "r";
h.LineWidth = 1.5;
legend(h,"$\lambda = "+ f.p1 + "$",'Interpreter','latex')
%}
xlabel("$t$",'Interpreter','latex')
ylabel("$\ln(\frac{||y(t) - x(t)||}{||\delta||})$",'Interpreter','latex')
title("$ ||\delta|| = " + norm(delta) + "$",'Interpreter','latex')
set(gca,'FontSize',30,'FontName','times')

%% Find local maxima
z = sol(:,3);
lmax = islocalmax(z,'MinProminence',.01);
map = [[0; z(lmax)] [z(lmax); 0] ];
map = map(2:end-1,:);
x = [max(map(:))/2:.1:max(map(:))];
y = x;
figure()
hold on
scatter(map(:,1),map(:,2),10,'r','filled')
plot(x, y,'k')
set(gca,'FontSize',30,'FontName','times')
xlabel("$z_n$",'Interpreter','latex')
ylabel("$z_{n+1}$",'Interpreter','latex')
title("Lorenz Map",'Interpreter','latex')
exportgraphics(gcf,"Lorenz_Map.png",'Resolution',600)
%% Make cobweb plot
ts = linspace(0,1,30);


f1 = figure()
hold on
scatter(map(:,1),map(:,2),10,'r','filled')
plot(x, y,'k')
set(gca,'FontSize',30,'FontName','times')


xlabel("$z_n$",'Interpreter','latex')
ylabel("$z_{n+1}$",'Interpreter','latex')
xlim([25 45])
ylim([25 45])
set(gca,'FontName','Times','FontSize',30)

Ns = zeros(length(ts),1);
No = 35;
Ns(1) = No;
for i = 2:length(ts)
    if mod(i,2) == 1
        [~,mini] = min(abs(map(:,1) - Ns(i-1)));
        Ns(i) = map(mini,2);
    else
        Ns(i) = Ns(i-1);
    end 
end
Ns_p1 = Ns(2:end);
Ns = Ns(1:end-1);
h = plot(Ns,Ns_p1,'b','LineWidth',1)
scatter(Ns,Ns_p1,'b')
scatter(No,No,75,'g','filled')
title("Lorenz Map",'Interpreter','latex')
legend(h,"$N_0 = " + No + "$",'Interpreter','latex')
exportgraphics(gcf,"Lorenz_map_cobwebs_No" + No + "_ld.png",'Resolution',600)


%% Maps and Period Doubling

logistic = @(r,x) r .* x .* (1 - x);
sine = @(r, x) r .* sin(pi .* x);
xo = 0.5;
rs = linspace(3.6,4,500);

% Do logistic
figure()
hold on
for i = 1:length(rs)
    logsols = recursion(logistic, rs(i), 100000, xo);
    scatter(repmat(rs(i), 1000,1), logsols(end-1000+1:end),1,'k','filled')
end 
set(gca,'FontName','Times','FontSize',30)
xlabel("$r$",'Interpreter','latex')
ylabel("$x$",'Interpreter','latex')
title("$x_{n+1} = rx_n(1-x_n)$",'Interpreter','latex')
%exportgraphics(gcf,"Logistic_map.png","Resolution",600)

%% Do sine map
figure()
hold on
rs = linspace(.9,1,500);
for i = 1:length(rs)
    logsols = recursion(sine, rs(i), 100000, xo);
    scatter(repmat(rs(i), 1000,1), logsols(end-1000+1:end),1,'k','filled');
end 
set(gca,'FontName','Times','FontSize',30)
xlabel("$r$",'Interpreter','latex')
ylabel("$x$",'Interpreter','latex')
title("$x_{n+1} = r\sin(\pi x_n)$",'Interpreter','latex')
%exportgraphics(gcf,"Sine_map.png","Resolution",600)

%% Calculating r of bifurcations, adapted from Strang notes
rs = linspace(2.8,3.57,10^4); % Increase spacing of r 
xs = linspace(0,1,10^4);
f_logistic = @(x,r) r*x.*(1 - x);
f_sine = @(x,r) r*sin(pi*x);

f{1} = @(x,r) f_logistic(x,r);
for j = 1:31
    f{j+1} = @(x,r) f{1}(f{j}(x,r),r);
end

new_period = 0;
rBifurcations = [];
for j = 1:length(rs)
    r = rs(j);
    % find fixed points and cycles (up to period 16)
    period = new_period;
    for p = 0:period
        step = 2^p;
        % find intersections
        sign_func = @(x) sign(f{step}(x,r) - x);
        signs = sign_func(xs);
        indices{p+1} = find(abs(diff(signs)) ~= 0); % indices corresponding to equilibrium for period 2^p
        indices{p+1} = sort(indices{p+1},'ascend');
        indices{p+1}(1) = []; % drop the equilibrium at 0
        % get stability
        stability = nan(size(indices{p+1}));
        for index_n = 1:length(indices{p+1})
            index = indices{p+1}(index_n);
            slope = (f{step}(xs(index+1),r) - f{step}(xs(index-1),r))/(xs(index+1) - xs(index - 1));
            stability(index_n) = abs(slope);
        end
        unstable{p+1} = indices{p+1}(stability > 1);
        stable{p+1} = indices{p+1}(stability < 1);
    end
    
    % check if we need higher period
    if length(unstable{period+1}) == length(indices{period+1})
        new_period = period+1;
        rBifurcations = [rBifurcations r];
    end
    % bound to avoid diverging
    new_period = min(new_period,7);
    
end

function sols = recursion(f, r, nsteps, xo)
    sols = nan(nsteps,1);
    sols(1) = xo;
    for i = 2:nsteps
        sols(i) = f(r, sols(i-1));
    end
end 

function dydt = myode(~,yvec,sigma, b, r)
    x = yvec(1);
    y = yvec(2);
    z = yvec(3);
    xdot = sigma * (y - x);
    ydot = r * x - y - x .* z;
    zdot = x .* y - b * z;
    dydt = [xdot; ydot; zdot];
end