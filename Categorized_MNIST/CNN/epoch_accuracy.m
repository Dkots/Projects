clc;
clear all;
%% Δεδομένα 
accuracy1 = [0.7834; 0.8300; 0.7627; 0.8375; 0.8429];
accuracy2 = [0.8674; 0.8714; 0.8648; 0.8585; 0.8585];
accuracy3 = [0.9023; 0.9051; 0.9050; 0.8919; 0.9184];
accuracy4 = [0.9128; 0.9400; 0.9348; 0.9482; 0.9391];
loss1 = [0.2199; 0.1298; 0.5803; 0.1694; 0.2091];
loss2 = [0.4037; 0.0865; 0.1910; 0.1262; 0.0937];
loss3 = [0.0649; 0.0685; 0.1081; 0.1334; 0.0793];
loss4 = [0.2199; 0.1514; 0.0973; 0.1045; 0.0577];
n = [1; 2; 3; 4];
accuracies = [accuracy1, accuracy2, accuracy3, accuracy4];
losss = [loss1, loss2, loss3, loss4];
%% Box plots
figure;
boxplot(accuracies);
xlabel('epoch')
ylabel('accuracy')
title('boxplot ακρίβειας μοντέλου σε κάθε εποχή')
axis([0.5, 4.5, 0.5, 1])
figure;
boxplot(losss);
xlabel('epoch')
ylabel('loss')
title('boxplot loss function σε κάθε εποχή')
axis([0.5, 4.5, 0, 0.5])
%% Mean plots
m_acc = mean(accuracies);
m_loss = mean(losss);
figure;
plot(m_acc, '-bo');
xlabel('epoch')
ylabel('accuracy')
title('Μέση τιμή ακρίβειας μοντέλου σε κάθε εποχή')
axis([1, 4, 0.5, 1])
figure;
plot(m_loss, '-bo');
xlabel('epoch')
ylabel('loss')
title('Μέση τιμή loss function μοντέλου σε κάθε εποχή')
axis([1, 4, 0, 0.5])