data_lbl = load('Dataset/Dataset_sbj1_labeled.mat');
data_ulbl = load('Dataset/Dataset_sbj1_unlabeled.mat');

data_concat_lbl = data_lbl.data_concat;
label_concat_lbl = data_lbl.label_concat;
data_concat_ulbl = data_ulbl.data_concat;
label_concat_ulbl = data_ulbl.label_concat;

idx_mot = find(label_concat_lbl <= 6);
data_concat_lbl = data_concat_lbl(idx_mot, :, :);
label_concat_lbl = label_concat_lbl(idx_mot);

idx_mot = find(label_concat_ulbl <= 6);
data_concat_ulbl = data_concat_ulbl(idx_mot, :, :);
label_concat_ulbl = label_concat_ulbl(idx_mot);

data_concat = data_concat_lbl(:, :, 1 : 6);
label_concat = label_concat_lbl;
save(strcat('Dataset_sbj1_labeled.mat'), 'data_concat', 'label_concat', '-v7.3');

data_concat = data_concat_ulbl(:, :, 1 : 6);
label_concat = label_concat_ulbl;
save(strcat('Dataset_sbj1_unlabeled.mat'), 'data_concat', 'label_concat', '-v7.3');