data_total = load('Dataset_sbj0_pseudo.mat');

data = data_total.Data;
pred = data_total.Pred;

figure();
subplot(2, 1, 1);
plot(squeeze(data(:, 1, :)));
subplot(2, 1, 2);
plot(pred);