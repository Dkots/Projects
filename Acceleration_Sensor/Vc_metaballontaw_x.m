clc;
clear all;

t = 0 : 0.0001 : 0.05;
l = exp(-35*t).*sin(2*pi*25*t);
plot(t, l)
%hold on;

%%
Vin = 0.2.*sin(2*pi*400*t);
plot(t,Vin, 'b')
%hold on;

Vc = l.*Vin;
plot(t,Vc, 'r')
title("VN σε σχέση με την Vin")

%%
rVin = rms(Vin)

rVc = rVin.*l;
plot(t,rVc, 'r')
title("Ενεργός τιμή της VN")

%% 

A = 21;
Vo = A.*Vc;
plot(t,Vo, 'r')
hold on;



