
% Homework 1
clear all; close all;
%2.4.9 (b)
tspan = linspace(0,10,100);
xo = 10;

figure()
hold on
xlabel('t')
ylabel('x')
[t,x] = ode45(@(t,y) -1 * y,tspan,xo);
plot(t,x,'b','LineWidth',2)
[t,x] = ode45(@(t,y) -y .^ 3,tspan,xo);
plot(t,x,'r','LineWidth',2)
set(gca,'FontName','Times','FontSize',25)
legend('$\dot{x} = -x$','$\dot{x} = -x^3$','Interpreter','Latex')
exportgraphics(gca,"2_4_9_c.png",'Resolution',600)

%% 8 Part 1.3
tspan = linspace(0,10,1000);
xo = .1;
[t,x] = ode45(@(t,y) y * (1-y),tspan,xo);
figure()
hold on
xlabel('t')
ylabel('N')
ylim([0 1])
plot(t,x,'k','LineWidth',2)
%{ 
Scaling time not working
plot(2.*t,x,'r','LineWidth',2)
[t,x] = ode45(@(t,y) 2.* y * (1-y),tspan,xo);
plot(t,x,'b','LineWidth',2)
%}
set(gca,'FontName','Times','FontSize',25)
legend("$r = 1$",'Interpreter','latex')
exportgraphics(gca,"8_p1_3.png",'Resolution',600)

%% 3 Part 2.3
ts = linspace(0,32,1000);


f1 = figure()
hold on
xlabel("$N(t)$","Interpreter","latex")
ylabel("$N(t + \Delta t)$",'Interpreter','latex')
ylim([0 1])
xlim([0 1])
set(gca,'FontName','Times','FontSize',25)
Nrange = linspace(0,1,100)
plot(Nrange,Nrange,'k','LineWidth',.5)
rs = [3.25 3.5 3.57]; % 3.25 3.5 3.57
cs = ['r' 'b' 'g']
hs = [];
for ri = 1:length(rs)
    r = rs(ri);
    plot(Nrange,r .* Nrange .* (1 - Nrange),cs(ri),'LineWidth',.5)
    Ns = zeros(length(ts),1);
    No = .4;
    xline(No,'--k')
    Ns(1) = No;
    for i = 2:length(ts)
        %
        if mod(i,2) == 1
            Ns(i) = r * Ns(i-1) * (1 - Ns(i-1));
        else
            Ns(i) = Ns(i-1);
        end
        %}
        %Ns(i) = r * Ns(i-1) * (1 - Ns(i-1));
    end
    Ns_p1 = Ns(2:end);
    Ns = Ns(1:end-1);
    hs = [hs plot(Ns,Ns_p1,cs(ri),'LineWidth',1)];
    scatter(Ns,Ns_p1,cs(ri))
    %{
    f2 = figure()
    hold on
    xlabel('t')
    ylabel("$N(t)$","Interpreter","latex")
    ylim([0 1])
    set(gca,'FontName','Times','FontSize',25)
    m = plot(ts(1:end-1),Ns,'k');
    scatter(ts(1:end-1),Ns,'k')
    legend(m, "$r = " + r + "$",'Interpreter','latex')
    exportgraphics(f2,"Logistic_map_r_" + r + ".png")
    close(f2)
    %}
end 
legend(hs,"$r = " + rs + "$",'Interpreter','latex')
exportgraphics(gca,"8_p2_3_2_r0_4.png",'Resolution',600)
%{
figure()
hold on
xlabel('t')
ylabel('N')
ylim([0 1])
set(gca,'FontName','Times','FontSize',25)
plot(ts,Ns,'LineWidth',1.5,'Color','b')
%}


%% Problem 9.3
phis = linspace(-2*pi/3,2*pi/3,10000);
omega  = 2 * pi / 24;
fs = (omega / 4) .* sqrt(4 - (sec(phis)) .^2);
dfs = diff(fs) ./ (phis(2) - phis(1));
figure()
hold on
xlabel("$\phi$","Interpreter","latex")
title("$f(\phi) = \frac{\omega}{4} \sqrt{4- sec(\phi(t))^2}$",'Interpreter','latex');
ylabel("$f(\phi)$",'Interpreter','latex')
xline(-2*pi/3,'--k')
xline(2*pi/3,'--k')
xlim([-2*pi/3,2*pi/3])
plot(phis,fs,'r','LineWidth',1.5)
set(gca,'FontName','Times','FontSize',25)
exportgraphics(gca,"9_3.png",'Resolution',600)

figure()
hold on
xlabel("$\phi$","Interpreter","latex")
title("$\frac{d}{d\phi} f(\phi)$ (numerically computed)",'Interpreter','latex');
ylabel("$\frac{d}{d\phi}$",'Interpreter','latex')
xlim([-2*pi/3,2*pi/3])
xline(-2*pi/3,'--k')
xline(2*pi/3,'--k')
set(gca,'FontName','Times','FontSize',25)
plot(phis(2:end),dfs,'b','LineWidth',1.5)
exportgraphics(gca,"9_3_2.png",'Resolution',600)