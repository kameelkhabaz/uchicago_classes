% Proble m2  Script for matrix exponential
% Compare multiplication and with pattern
A = [0 1; 
    -1 -2];

for k = 0:10
    disp("Exponential A^" + k);
    disp(A^k)
    disp("Exponential A^" + k + " Using Pattern");
    disp(pattern(k))
end 


%% Crticially Damped Oscillator Matrix Exponential Approximation
A = [0 1; 
    -1 -2];

% Solution trajectories are given by y(t) = expo(At) * y(0)
close all
figure();
set(gcf,'Position',[0 0 800 1000])
t = linspace(0,5,100);
z0 = [1; 1];
solns = nan(2,length(t));

subplot(3,2,1);
n = 1
solve_plot(n,A,t,z0);

subplot(3,2,2);
n = 3
solve_plot(n,A,t,z0);

subplot(3,2,3);
n = 5
solve_plot(n,A,t,z0);

subplot(3,2,4);
n = 7
solve_plot(n,A,t,z0);

subplot(3,2,5);
n = 9
solve_plot(n,A,t,z0);

subplot(3,2,6);
n = 10
solve_plot(n,A,t,z0);
exportgraphics(gcf,"n_approx_trajs.eps")
%%
figure()
t = linspace(0,10,100);
n = 100;
solve_plot(n,A,t,z0);
exportgraphics(gcf,"Approx_traj_100_term.eps")

t = linspace(0,100,100);
n = 100;
solve_plot(n,A,t,z0);
exportgraphics(gcf,"Approx_traj_100_term2.eps")

%% Problem 2.7 - Solution with Explicit Form of Matrix Exponential
figure()
t = linspace(0,10,100);
explicit_soln_plot(A,t,z0)
exportgraphics(gcf,"explicit_form_soln.eps")

t = linspace(0,100,100);
explicit_soln_plot(A,t,z0)
exportgraphics(gcf,"explicit_form_soln_lt.eps")
function explicit_soln_plot(A,t,z0)
    solns = nan(2,length(t))
    for i = 1:length(t)
        solns(:,i) = exp(-t(i)) * ([1 0; 0 1] + t(i) * [1 1; -1 -1]) * z0;
    end 
    hold on
    di = min(find(abs(solns(1,:)) > 2))
    if isinteger(di)
        xline(t(di),'--r','LineWidth',1.5)
    end 
    h1= plot(t,solns(1,:),'Color','k','LineWidth',2);
    h2 = plot(t,solns(2,:),'Color','k','LineWidth',2,'LineStyle','--');
    title("Explicit Form of Matrix Exponential",'Interpreter','latex')
    set(gca,'FontSize',30,'FontName','times')
    xlabel("$t$",'Interpreter','latex')
    ylabel('$z(\tau)$','Interpreter','latex')
    lgd = legend([h1 h2], "z_1","z_2")
    lgd.Location = "northeast";
    lgd.FontSize = 18;
    
end 

function solve_plot(n,A,t,z0)
    for i = 1:length(t)
        solns(:,i) = soln_exp_approx(A,t(i),n,z0);
    end
    hold on
    di = min(find(abs(solns(1,:)) > 2))
    if isinteger(di)
        xline(t(di),'--r','LineWidth',1.5)
    end 
    h1= plot(t,solns(1,:),'Color','k','LineWidth',2);
    h2 = plot(t,solns(2,:),'Color','k','LineWidth',2,'LineStyle','--');
    title("$ n = " + n + " $",'Interpreter','latex')
    set(gca,'FontSize',30,'FontName','times')
    xlabel("$t$",'Interpreter','latex')
    ylabel('$z(\tau)$','Interpreter','latex')
    lgd = legend([h1 h2], "z_1","z_2")
    lgd.Location = "northwest";
    lgd.FontSize = 18;
    
end 

function [soln] = soln_exp_approx(A,t,n,z0)
    expA = [1 0; 0 1];
    for k = 1:n
        expA = expA + (1/factorial(k)) .* (A .*t )^k;
    end
    soln = expA * z0;
end 

function [A] = pattern(k)
    A = [(-1)^(k-1) * (k-1)  k * (-1)^(k-1); 
         (-1)^k * k          (k+1) * (-1)^k];
end 