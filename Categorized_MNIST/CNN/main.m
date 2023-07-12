clc;
clear all;
%% load data
load('data_mnist.mat', 'XTrain', 'YTrain', 'XTest', 'YTest');
%% Απεικόνιση δεδομένων από κάθε κλάση (α ερώτημα)
classes = Inf * ones(1, 10); % Ποιά νούμερα έχουν τοποθετηθεί στο images
images = zeros(28, 28, 10); % Αποθήκευσει των εικόνων απεικόνισης
imgs_train = extractdata(XTrain); % Εξαγωγή των δεδομένων από το dlarray
labels = YTrain; % Οι κατηγορίες των εικονών
i = 1; % μετρητής
while 1
    number = int16(labels(i)); % Αριθμός
    % Έλεγχος αν έχουν περάσει όλες οι κατηγορίες
    if sum(ismember(classes, Inf)) == 0
        break;
    end
    img_num = imgs_train(:, :, 1, i); % Εικόνα αριθμού
    images(:, :, number) = img_num; % Αποθήκευση αριθμού
    classes(number) = number; % Αποθήκευση ότι πέρασε αυτός ο αριθμός
    i = i+1; % Αύξηση μετρητή κατά ένα
end
% Απεικόνιση δεδομένων 
figure;
for count = 1 : 10
    subplot(2, 5, count);
    imshow(images(:, :, count))
    title("Number: " + string(count-1));
    count = count + 1;
end
%% Αρχικοποίηση των παραμέτρων και των δεδομένω του CNN (β ερώτημα)
% Ορισμός επιπέδων
layers = [
    imageInputLayer([28 28 1])
    convolution2dLayer(3,6,'Padding', 0, 'Stride', 1)
    reluLayer   
    averagePooling2dLayer(2,'Stride', 2)
    convolution2dLayer(3,16,'Padding', 0, 'Stride', 1)
    reluLayer
    averagePooling2dLayer(2,'Stride', 2)
    fullyConnectedLayer(120)
    reluLayer
    fullyConnectedLayer(84)
    reluLayer
    fullyConnectedLayer(10)
    softmaxLayer
    classificationLayer];
% Τοποθέτηση χαρακτηριστικών του cnn 
options = trainingOptions("sgdm", ...
    'Momentum', 1, ...
    'InitialLearnRate',1e-4, ...
    'Shuffle','every-epoch', ...
    'MaxEpochs', 4, ...
    'MiniBatchSize', 100, ...
    'Verbose', false, ...
    'Plots', "training-progress");
% Εκπαίδευση CNN
net = trainNetwork(XTrain,YTrain,layers,options);
%% Κατηγοριοποίηση των δεδομένων και ποσοστό ακρίβειας του μοντέλου
YPred = classify(net,XTest); % Κατηγοριοποίηση των δεδομένων
accuracy = sum(YPred == YTest)/numel(YTest) % Ακρίβεια μοντέλου
YPred = string(YPred);
YPred = double(YPred);
YTest = string(YTest);
YTest = double(YTest);
loss = crossentropy(YPred,YTest) % Συνάρτηση κόστου
%% Confussion Matrix (δ ερώτημα)
CM = createConfusionMatrix(YPred,YTest);
%% F_Score
[pr_val] = mean(Precision(CM)) % Ακρίβεια
[sen_val] = mean(Sensetivity(CM)) % Ευεσθησία
F_Score = 2*pr_val*sen_val/(pr_val+sen_val) % F_Score