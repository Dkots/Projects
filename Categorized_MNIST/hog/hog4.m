clc;
clear all;
%% load data
load('data_mnist.mat', 'XTrain', 'XTrainHog4', 'YTrain', 'XTestHog4', 'YTest');
%% Απεικόνιση δεδομένων από κάθε κλάση (α ερώτημα)
classes = Inf * ones(1, 10); % Ποιά νούμερα έχουν τοποθετηθεί στο images
images = zeros(28, 28, 10); % Αποθήκευσει των εικόνων απεικόνισης
images_number = []; % Αποθήκευσει των εικόνων απεικόνισης
images_number_hog = [];
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
    [featureVector,hogVisualization] = extractHOGFeatures(img_num,'CellSize',[4 4]);
    images_number = [images_number, number]; 
    images_number_hog = [images_number_hog, hogVisualization];
    classes(number) = number; % Αποθήκευση ότι πέρασε αυτός ο αριθμός
    i = i+1; % Αύξηση μετρητή κατά ένα
end
% Sort
for i = 1:10
    for j = i+1:10
        if images_number(i) > images_number(j)
            images_number([i, j]) = images_number([j, i]);
            images_number_hog([i, j]) = images_number_hog([j, i]);
        end
    end
end
% Απεικόνιση δεδομένων 
figure;
for count = 1 : 10
    subplot(2, 5, count);
    imshow(images(:, :, count));
    hold on;
    plot(images_number_hog(count))
    title("Number: " + string(count-1));
    count = count + 1;
end
%% Εκπαίδευση SVM
trainingLabelsGrps = grp2idx(YTrain)-1;  % returns group indices  1 through 10
classifier_to_deploy = fitcecoc(XTrainHog4, trainingLabelsGrps);
%% Ταξινόμηση δεδομένων
label = predict(classifier_to_deploy, XTestHog4);
YTest = string(YTest);
YTest = double(YTest);
accuracy = sum(label == YTest)/length(label)
%% Confussion Matrix
CM = createConfusionMatrix(label,YTest);
[pr_val] = mean(Precision(CM))
[sen_val] = mean(Sensetivity(CM))
F_Score = 2*pr_val*sen_val/(pr_val+sen_val)