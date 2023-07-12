function [val] = Precision(CM)
    val = zeros(1, 10);
    for i = 1 : 10
        TP = CM(i, i);
        FP = sum(CM(:, i)) - CM(i, i);
        pr = TP/(TP + FP);
        val(i) = pr;
    end
end

