close all;
clear; clc;

%% Parameter setting
sbj_idx = '0';

Win_size = 150;
N_class = 4;
N_features = 4;
N_CH = 5;

MAV_max_val = 0;
WL_max_val = 0;

std_scale = 10.0;

Fldr_path = 'D:/TensorRT_demo/Data/';
Fldr_name_lbl = '240603_100250_KimSejin_Training/';
Fldr_name_ulbl = '240603_100519_KimSejin_Unlabeled_DAQ/';

%% Load the labeled training data
tStart = tic;

fprintf("Labeled training data) Processing start\n");
Filename = 'sEMG_MAV.txt';
sEMG_MAV_lbl = load(strcat(Fldr_path, Fldr_name_lbl, Filename));
sEMG_MAV_lbl = reshape(sEMG_MAV_lbl, [size(sEMG_MAV_lbl, 1), 1, size(sEMG_MAV_lbl, 2)]);

Filename = 'sEMG_WL.txt';
sEMG_WL_lbl = load(strcat(Fldr_path, Fldr_name_lbl, Filename));
sEMG_WL_lbl = reshape(sEMG_WL_lbl, [size(sEMG_WL_lbl, 1), 1, size(sEMG_WL_lbl, 2)]);

Filename = 'sEMG_SSC.txt';
sEMG_SSC_lbl = load(strcat(Fldr_path, Fldr_name_lbl, Filename));
sEMG_SSC_lbl = reshape(sEMG_SSC_lbl, [size(sEMG_SSC_lbl, 1), 1, size(sEMG_SSC_lbl, 2)]);

Filename = 'sEMG_ZC.txt';
sEMG_ZC_lbl = load(strcat(Fldr_path, Fldr_name_lbl, Filename));
sEMG_ZC_lbl = reshape(sEMG_ZC_lbl, [size(sEMG_ZC_lbl, 1), 1, size(sEMG_ZC_lbl, 2)]);

Filename = 'Motion_label.txt';
label_lbl = load(strcat(Fldr_path, Fldr_name_lbl, Filename));

% Concatenate all features
sEMG_data_lbl = cat(2, sEMG_MAV_lbl, sEMG_WL_lbl, sEMG_SSC_lbl, sEMG_ZC_lbl);

L = sum(label_lbl ~= 0);
data_concat_lbl_total = zeros(L, N_features, N_CH);
label_concat_lbl_total = zeros(L, 1);

idx = 1;
for i = 1 : length(sEMG_data_lbl)
    if (label_lbl(i) > 0)
        data_concat_lbl_total(idx, :, :) = sEMG_data_lbl(i, :, :);
        label_concat_lbl_total(idx) = label_lbl(i);

        MAV_max = max(squeeze(sEMG_data_lbl(i, 1, :)));
        if (MAV_max_val <= MAV_max)
            MAV_max_val = MAV_max;
        end

        WL_max = max(squeeze(sEMG_data_lbl(i, 2, :)));
        if (WL_max_val <= WL_max)
            WL_max_val = WL_max;
        end

        idx = idx + 1;
    end

    if (i == fix(length(sEMG_data_lbl) / 10 * 1))
        fprintf("10%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 2))
        fprintf("20%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 3))
        fprintf("30%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 4))
        fprintf("40%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 5))
        fprintf("50%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 6))
        fprintf("60%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 7))
        fprintf("70%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 8))
        fprintf("80%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 9))
        fprintf("90%% ");
    elseif (i == fix(length(sEMG_data_lbl) / 10 * 10))
        fprintf("100%%\n");
    end
end

Time_elapse_lbl = toc(tStart);
fprintf("Labeled training dataset processing complete - Elapsed time : %.2f seconds\n", Time_elapse_lbl);

%% Load the unlabeled training data
tStart = tic;

fprintf("Unlabeled training data) Processing start\n");
Filename = 'sEMG_MAV.txt';
sEMG_MAV_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));
sEMG_MAV_ulbl = reshape(sEMG_MAV_ulbl, [size(sEMG_MAV_ulbl, 1), 1, size(sEMG_MAV_ulbl, 2)]);

Filename = 'sEMG_WL.txt';
sEMG_WL_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));
sEMG_WL_ulbl = reshape(sEMG_WL_ulbl, [size(sEMG_WL_ulbl, 1), 1, size(sEMG_WL_ulbl, 2)]);

Filename = 'sEMG_SSC.txt';
sEMG_SSC_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));
sEMG_SSC_ulbl = reshape(sEMG_SSC_ulbl, [size(sEMG_SSC_ulbl, 1), 1, size(sEMG_SSC_ulbl, 2)]);

Filename = 'sEMG_ZC.txt';
sEMG_ZC_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));
sEMG_ZC_ulbl = reshape(sEMG_ZC_ulbl, [size(sEMG_ZC_ulbl, 1), 1, size(sEMG_ZC_ulbl, 2)]);

% Concatenate all features
sEMG_data_ulbl = cat(2, sEMG_MAV_ulbl, sEMG_WL_ulbl, sEMG_SSC_ulbl, sEMG_ZC_ulbl);

% Calculate the mean and standard deviation of sEMG MAV
sEMG_MAV_ulbl = squeeze(sEMG_MAV_ulbl);
sEMG_MAV_ulbl_rest = sEMG_MAV_ulbl(1000 : 4000, :);
sEMG_MAV_ulbl_rest_mean = mean(sEMG_MAV_ulbl_rest, 1);
sEMG_MAV_ulbl_rest_std = std(sEMG_MAV_ulbl_rest, [], 1);

idx_active = find(sum(sEMG_MAV_ulbl, 2) >= sum(sEMG_MAV_ulbl_rest_mean + std_scale * sEMG_MAV_ulbl_rest_std));
L = length(idx_active);
data_concat_ulbl_total = zeros(L, 4, N_CH);

idx = 1;
for i = 1 : length(sEMG_data_ulbl)
    if (sum(sEMG_MAV_ulbl(i, :), 2) >= sum(sEMG_MAV_ulbl_rest_mean + std_scale * sEMG_MAV_ulbl_rest_std))
        data_concat_ulbl_total(idx, :, :) = sEMG_data_ulbl(i, :, :);

        MAV_max = max(squeeze(sEMG_data_ulbl(i, 1, :)));
        if (MAV_max_val <= MAV_max)
            MAV_max_val = MAV_max;
        end

        WL_max = max(squeeze(sEMG_data_ulbl(i, 2, :)));
        if (WL_max_val <= WL_max)
            WL_max_val = WL_max;
        end

        idx = idx + 1;
    end

    if (i == fix(length(sEMG_data_ulbl) / 10 * 1))
        fprintf("10%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 2))
        fprintf("20%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 3))
        fprintf("30%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 4))
        fprintf("40%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 5))
        fprintf("50%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 6))
        fprintf("60%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 7))
        fprintf("70%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 8))
        fprintf("80%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 9))
        fprintf("90%% ");
    elseif (i == fix(length(sEMG_data_ulbl) / 10 * 10))
        fprintf("100%%\n");
    end
end

Time_elapse_ulbl = toc(tStart);
fprintf("Unlabeled training dataset processing complete - Elapsed time : %.2f seconds\n", Time_elapse_ulbl);

%% Generate the labeled training dataset
data_concat = data_concat_lbl_total;
data_concat(:, 1, :) = data_concat(:, 1, :) ./ sqrt(sum(data_concat(:, 1, :).^2, 3));
data_concat(:, 2, :) = data_concat(:, 2, :) ./ sqrt(sum(data_concat(:, 2, :).^2, 3));
data_concat(:, 3, :) = data_concat(:, 3, :) / Win_size;
data_concat(:, 4, :) = data_concat(:, 4, :) / Win_size;
label_concat = label_concat_lbl_total;

data_concat_lbl = data_concat;
label_concat_lbl = label_concat;

% Convert the data into .mat dataset
save(strcat('Dataset/Dataset_sbj', sbj_idx, '_labeled.mat'), ...
            'data_concat', 'label_concat', '-v7.3');
fprintf("Labeled training dataset save complete\n");

%% Generate the unlabeled training dataset
data_concat = data_concat_ulbl_total;
data_concat(:, 1, :) = data_concat(:, 1, :) ./ sqrt(sum(data_concat(:, 1, :).^2, 3));
data_concat(:, 2, :) = data_concat(:, 2, :) ./ sqrt(sum(data_concat(:, 2, :).^2, 3));
data_concat(:, 3, :) = data_concat(:, 3, :) / Win_size;
data_concat(:, 4, :) = data_concat(:, 4, :) / Win_size;

data_concat_ulbl = data_concat;

% Convert the data into .mat dataset
save(strcat('Dataset/Dataset_sbj', sbj_idx, '_unlabeled.mat'), 'data_concat', '-v7.3');
fprintf("Unlabeled training dataset save complete\n");

fprintf("Maximum MAV value : %f\n", MAV_max_val);
fprintf("Maximum WL value : %f\n", WL_max_val);

%% Check the data) Notice the maximum value of MAV and WL
figure('position', [0 0 1920 1000]);
subplot(3, 1, 1);
plot(squeeze(data_concat_lbl(:, 1, :)), 'LineWidth', 2);
xlim([1 size(data_concat_lbl, 1)]);
subplot(3, 1, 2);
plot(label_concat_lbl, 'k', 'LineWidth', 2);
xlim([1 size(label_concat_lbl, 1)]);
ylim([0 N_class + 1]);
subplot(3, 1, 3);
plot(squeeze(data_concat_ulbl(:, 1, :)), 'LineWidth', 2);
xlim([1 size(data_concat_ulbl, 1)]);