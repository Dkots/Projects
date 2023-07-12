function [val] = Sensetivity(CM)
    val = zeros(1, 10);
    for i = 1 : 10
        TP = CM(i, i);
        FN = sum(CM(i, :)) - CM(i, i);
        sen = TP/(TP + FN);
        val(i) = sen;
    end
end

