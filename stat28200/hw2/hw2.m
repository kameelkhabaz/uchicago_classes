close all


r = 0;
[x,y] = meshgrid(-1:0.1:1);
z = zeros(size(x));
u = -x + y;
v = r * x - y;

figure()
quiver(x,y,u,v,2,'Color','black','LineWidth',1)
l = streamslice(x,y,u,v,.3)
set(l,'LineWidth',1.5,'Color','r')
axis tight 
xlabel('$x$','Interpreter','latex')
ylabel('$y$','Interpreter','latex')
title("$ r =  0$",'Interpreter','latex')
xline(0,'LineWidth',2)
yline(0,'LineWidth',2)
set(gca,'FontSize',30,'FontName','times')
exportgraphics(gcf,"phase_portrait_r_0.png",'Resolution',600)