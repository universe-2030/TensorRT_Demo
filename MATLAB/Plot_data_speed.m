Fldr_path = 'D:/STDAN_TensorRT/Data/240519_KimSejin_Speed/';

Flex_processed_prev = 0;
f_filter = 2 * pi * 15;

%% Slow
% Fldr_name = '240424_133154_KimSejin_Slow_2nd/';
% Filename = 'sEMG_MAV.txt';
% sEMG_MAV_slow = load(strcat(Fldr_path, Fldr_name, Filename));
% 
% Filename = 'IMU_Gyro.txt';
% IMU_Gyro_slow = load(strcat(Fldr_path, Fldr_name, Filename));
% 
% Filename = 'Flex_processed.txt';
% Flex_processed_slow = load(strcat(Fldr_path, Fldr_name, Filename));
% 
% %%
% Fldr_name = '240424_134403_KimSejin_Moderate_3rd/';
% Filename = 'sEMG_MAV.txt';
% sEMG_MAV_moderate = load(strcat(Fldr_path, Fldr_name, Filename));
% 
% Filename = 'IMU_Gyro.txt';
% IMU_Gyro_moderate = load(strcat(Fldr_path, Fldr_name, Filename));
% 
% Filename = 'Flex_processed.txt';
% Flex_processed_moderate = load(strcat(Fldr_path, Fldr_name, Filename));

%%
Fldr_name = '240519_144746_KimSejin_Speed_WF_WE/';
Filename = 'sEMG_MAV.txt';
sEMG_MAV_fast = load(strcat(Fldr_path, Fldr_name, Filename));

Filename = 'IMU_Gyro.txt';
IMU_Gyro_fast = load(strcat(Fldr_path, Fldr_name, Filename));

Filename = 'Flex_processed.txt';
Flex_processed_fast = load(strcat(Fldr_path, Fldr_name, Filename));

%%
flag = 3;
if (flag == 1)
    sEMG_MAV_plot = sEMG_MAV_slow;
    IMU_Gyro_plot = IMU_Gyro_slow;
    Flex_processed_plot = Flex_processed_slow;
elseif (flag == 2)
    sEMG_MAV_plot = sEMG_MAV_moderate;
    IMU_Gyro_plot = IMU_Gyro_moderate;
    Flex_processed_plot = Flex_processed_moderate;
elseif (flag == 3)
    sEMG_MAV_plot = sEMG_MAV_fast;
    IMU_Gyro_plot = IMU_Gyro_fast;
    Flex_processed_plot = Flex_processed_fast;
end

figure();
subplot(3, 1, 1);
plot(sEMG_MAV_plot);
subplot(3, 1, 2);
plot(IMU_Gyro_plot);
ylim([-500 500])
subplot(3, 1, 3);
plot(Flex_processed_plot);