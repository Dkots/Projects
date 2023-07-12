clc;
clear all;

t = 0 : 0.001 : 1;
x = exp(-5*t).*sin(pi*t);
plot(t, x)
hold on;

%%
Vin = 10*sqrt(2)*sin(2*pi*t);
plot(t,Vin)
hold on;

Vc = 0.5*(x + 1).*Vin;
plot(t,Vc, 'g')

%%
z = 100.*x;
u = [diff(z), 0];
a = -4.*u.*pi.*cot(2.*pi.*t);

plot(t, u, 'g')
hold on;
plot(t, a, 'r')