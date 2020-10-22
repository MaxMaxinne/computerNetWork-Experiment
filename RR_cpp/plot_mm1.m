clc;
close all;
m1=load("./res_output/res_mm1.dat");
subplot(1,2,1)
x1=m1(:,1);
y1=m1(:,2);
semilogy(x1,y1)
title('waitingTime')
subplot(1,2,2)
m2=load("./res_output/len_mm1.dat");
x2=m2(:,1);
y2=m2(:,2);
semilogy(x2,y2)
title('queueLength')