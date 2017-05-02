clear all
close all

%% Drug Chamber Dimensions and Data

n = 12; %number of holes
L_chamber = 40e-3; % length of drug chamber
V_chamber = 3.142 * (5.225e-3)^2 * (L_chamber-0.01); % volume of chamber
L_holes = 2.5e-3; % length of holes (thickness of drug chamber)
R_holes = 0.4e-3; % radius of holes
A_magnet = 3.142 * (5e-3)^2;
p = 10; % dynamic viscosity of drug (maple syrup)

%% COMSOL Force Result

Fs = [0.1880354122069975,0.15739323283959697,0.13041959385995178,0.1089995530967161,0.09203059140871228,0.07832833779141345,0.06737922547319548,0.058625538848939805,0.05162789646546658,0.0460510769298196,0.04175408706169915,0.038403656330158256,0.03586549855069732,0.03411377725336179,0.03305443943399392,0.03271978819617875,0.03296387725192348,0.03390807702008977,0.03556206686782465,0.038012165140788624,0.041226321518958775,0.04556897199034779,0.05104041772424117,0.057938866547897615,0.06662352180430278,0.07745200247852893,0.09094605900247006,0.10796498314778663,0.12922673005183952,0.15601336369635851,0.19031455560730753];

%% Predicted Drug Flow 

V1 = zeros(1,31); % flow rate - Hagen Poiseuille Equation
V2 = zeros(1,31); % cumulative flow rate since the activation of the coils
Drug_Delivered = zeros(1,31);
Percentage_Drug_Delivered = zeros(1,31);
Percentage_Drug_Delivered2 = zeros(1,32);

%% Test Results

Percentage_Drug_Delivered_Average = zeros(1,8);
Percentage_Drug_Delivered_Test1 = [0,(0.681*100)/3.514,(1.083*100)/3.514,(1.474*100)/3.514,(1.776*100)/3.514,(2.340*100)/3.514,(2.718*100)/3.514,(2.720*100)/3.514,(2.720*100)/3.514,(2.720*100)/3.514,(2.720*100)/3.514,(2.720*100)/3.514,(2.720*100)/3.514];
Percentage_Drug_Delivered_Test2 = [0,(0.397*100)/3.489,(0.863*100)/3.489,(1.195*100)/3.489,(1.507*100)/3.489,(1.884*100)/3.489,(2.400*100)/3.489,(2.496*100)/3.489,(2.574*100)/3.489,(2.574*100)/3.489,(2.574*100)/3.489,(2.574*100)/3.489,(2.574*100)/3.489];
Percentage_Drug_Delivered_Test3 = [0,(0.474*100)/3.546,(1.053*100)/3.546,(1.374*100)/3.546,(1.537*100)/3.546,(1.73*100)/3.546,(1.925*100)/3.546,(2.013*100)/3.546,(2.102*100)/3.546,(2.263*100)/3.546,(2.265*100)/3.546,(2.341*100)/3.546,(2.343*100)/3.546];
Percentage_Drug_Delivered_Test4 = [0,(0.285*100)/3.494,(0.660*100)/3.494,(0.943*100)/3.494,(1.172*100)/3.494,(1.457*100)/3.494,(1.675*100)/3.494,(2.039*100)/3.494,(2.574*100)/3.494,(2.574*100)/3.494,(2.574*100)/3.494,(2.574*100)/3.494,(2.574*100)/3.494];
Percentage_Drug_Delivered_Test5 = [0,(0.547*100)/3.506,(0.913*100)/3.506,(1.219*100)/3.506,(1.558*100)/3.506,(1.905*100)/3.506,(2.438*100)/3.506,(2.539*100)/3.506,(2.539*100)/3.506,(2.539*100)/3.506,(2.539*100)/3.506,(2.539*100)/3.506,(2.539*100)/3.506];
Percentage_Drug_Delivered_Test6 = [0,(0.417*100)/3.455,(0.824*100)/3.455,(1.160*100)/3.455,(1.541*100)/3.455,(2.431*100)/3.455,(2.441*100)/3.455,(2.441*100)/3.506,(2.521*100)/3.455,(2.521*100)/3.455,(2.521*100)/3.455,(2.521*100)/3.455,(2.521*100)/3.455];
Percentage_Drug_Delivered_Test7 = [0,(0.675*100)/3.483,(1.174*100)/3.483,(1.745*100)/3.483,(2.657*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483,(2.742*100)/3.483];
Percentage_Drug_Delivered_Test8 = [0,(0.578*100)/3.401,(0.982*100)/3.401,(1.355*100)/3.401,(1.850*100)/3.401,(2.456*100)/3.401,(2.544*100)/3.401,(2.544*100)/3.401,(2.544*100)/3.401,(2.544*100)/3.401,(2.544*100)/3.401,(2.544*100)/3.401,(2.544*100)/3.401];
Percentage_Drug_Delivered_Test9 = [0,(0.576*100)/3.621,(1.075*100)/3.621,(1.449*100)/3.621,(2.039*100)/3.621,(2.567*100)/3.621,(2.636*100)/3.621,(2.636*100)/3.621,(2.636*100)/3.621,(2.636*100)/3.621,(2.636*100)/3.621,(2.636*100)/3.621,(2.636*100)/3.621];
Percentage_Drug_Delivered_Test10 = [0,(1.032*100)/3.502,(1.691*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502,(2.626*100)/3.502];

%% Model

for z = 0:1:30 % distance from coil 1 (LHS)
    
    % flow rate of drug
    V1(z+1) = (n*Fs(z+1)*3.142*(R_holes^4)) / (8*A_magnet*p*(L_holes));
    
end

for z = 0:1:30
    
    for x = 0:1:z
        
        V2(z+1) = V2(z+1) + V1(x+1);
        
    end
    
    
end

for z = 0:1:30
    
    Drug_Delivered(z+1) = V2(z+1) * 17.8263;
    
    Percentage_Drug_Delivered(z+1) = (Drug_Delivered(z+1) / (V_chamber)) * 100;
    
end

%% Test

for i = 1:1:13
    
    Percentage_Drug_Delivered_Average(i) = (Percentage_Drug_Delivered_Test1(i) + Percentage_Drug_Delivered_Test2(i) + Percentage_Drug_Delivered_Test3(i) + Percentage_Drug_Delivered_Test4(i) + Percentage_Drug_Delivered_Test5(i) + Percentage_Drug_Delivered_Test6(i) + Percentage_Drug_Delivered_Test7(i) + Percentage_Drug_Delivered_Test8(i) + Percentage_Drug_Delivered_Test9(i) + Percentage_Drug_Delivered_Test10(i)) / 10;
    
end

%% Zeroing Drug Delivery Array

x1 = [18.42051,25];
y1 = [100,100];
Percentage_Drug_Delivered2(2:32) = Percentage_Drug_Delivered(1:31);

%% Plots

figure (1);

plot(0:30,1000*Fs(1:31),'r');
xlabel('Distance between Edge of Coil and Edge of Magnet (mm)');
ylabel('Force Exerted on the Magnet(mN)');
grid on;

figure (2);

plot(linspace(0,17.8263,31),Drug_Delivered(1:31),'r');
xlabel('Time the Coils have been activated (s)');
ylabel('Drug Delivered (m^3)');
grid on;

figure (3);

plot(linspace(0,18.42051,32),Percentage_Drug_Delivered2(1:32),'r');
xlabel('Time the Coils have been activated (s)');
ylabel('Percentage of Total Drug Delivered (%)');
grid on;

figure (4);

plot(linspace(0,18.42051,32),Percentage_Drug_Delivered2(1:32),'r',linspace(0,25,11),Percentage_Drug_Delivered_Average(1:11),'b');
xlabel('Time the Coils have been activated (s)');
ylabel('Percentage of Total Drug Delivered (%)');
legend('COMSOL Model','Experimental Result','Location','NorthWest');
line(x1,y1,'Color','red');
grid on;
