clear all
close all

M_test = [9.03,3.43,1.09,0.7,0.4,0.3,0.2,0.1];
M_comsol = [7.1,3.2,1.25,0.6,0.35,0.2,0.15,0.1];

plot(linspace(0,35,8),M_comsol,'b',linspace(0,35,8),M_test,'r');
xlabel('Distance from Coil (mm)');
ylabel('Magnetic Flux Density (mT)');
legend('COMSOL Magnetic Flux Density','Experimental Magnetic Flux Density');
grid on;

