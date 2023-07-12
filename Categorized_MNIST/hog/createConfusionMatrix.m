function [Confusion_Matrix] = createConfusionMatrix(YPred,YTest)
    % rows: πραγματικά, columns: προβλεπόμενα
    Confusion_Matrix = zeros(10, 10);
    len_Y = length(YPred); % Μέγεθος test label
    % Δημιουργεία confussion matrix
    for i = 1 : len_Y
        yp = YPred(i)+1;
        yt = YTest(i)+1;
        Confusion_Matrix(yt, yp) = Confusion_Matrix(yt, yp)+1;
    end
    % Απεικόνιση του confussion matrix
    figure;
    heatmap(Confusion_Matrix);
    xlabel('Predicted')
    ylabel('Accuracy')
    title('confussion matrix mnist')
end

