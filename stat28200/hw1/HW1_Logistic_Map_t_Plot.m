%% 3 Part 2.3
ts = linspace(0,32,32);
rs = [2 2.5 3 3.25 3.5 3.57]; % 3.25 3.5 3.57
Nos = [.05 .2 .4 .8 .95];
cs = ['r' 'b' 'g' 'c' 'k'];
for ri = 1:length(rs)
    r = rs(ri);
    figure()
    m = [];
    hold on
    xlabel('t')
    xlim([0 32])
    ylabel("$N(t)$","Interpreter","latex")
    ylim([0 1])
    set(gca,'FontName','Times','FontSize',25)
    
    for j = 1:length(Nos)
        No = Nos(j);
        Ns = zeros(length(ts),1);
        Ns(1) = No;
        for i = 2:length(ts)
            Ns(i) = r * Ns(i-1) * (1 - Ns(i-1));
        end
    
        m = [m plot(ts,Ns,cs(j),'LineWidth',1.5)];
        scatter(ts,Ns,cs(j))
    end
    title("$r = " + r + "$",'Interpreter','Latex');
    legend(m, "$N_0 = " + Nos + "$",'Interpreter','latex')
    exportgraphics(gcf,"Logistic_map_r_" + strrep(num2str(r),".","_") + ".png",'Resolution',600 )

end 