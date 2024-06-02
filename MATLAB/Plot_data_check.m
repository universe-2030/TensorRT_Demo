Fldr_path = 'D:/STDAN_TensorRT/Data/';
Fldr_name_ulbl = '240419_145833_KimSejin_Unlabeled_DAQ/';

fprintf("Unlabeled training data) Processing start\n");
Filename = 'sEMG_MAV.txt';
sEMG_MAV_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));

Filename = 'IMU_Ori.txt';
IMU_Ori_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));

Filename = 'Flex_processed.txt';
Flex_processed_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));

Filename = 'Motion_label.txt';
label_ulbl = load(strcat(Fldr_path, Fldr_name_ulbl, Filename));

figure();
subplot(4, 1, 1);
plot(sEMG_MAV_ulbl);
subplot(4, 1, 2);
plot(IMU_Ori_ulbl);
subplot(4, 1, 3);
plot(Flex_processed_ulbl);
subplot(4, 1, 4);
plot(label_ulbl);