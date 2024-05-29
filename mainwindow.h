#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <QRect>
#include <QtWidgets>
#include <QMetaObject>
#include <Qt3DInput>
#include <QFileDialog>
#include <QTextStream>
#include <QRadioButton>
#include <QMutex>
#include <QAction>
#include <QDateTime>
#include <QDir>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <QPushButton> // 순서 바뀌면 setEnabled를 method로 인식하지 못함. Qt headers 맨 마지막에 두기.

// C++ standard headers
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>
#include <cmath>
#include <ctime>
#include <cstdlib>

// Custom headers
#include "tensorrt_module.h"
#include "NIDAQmx.h"
#include "ni_ai_ao_do.h"
#include "qcustomplot.h"

// API headers
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "Timer_TwinCAT.h"

// Video Dialog headers
#include "traininguserform.h"
#include "unlabeleddaquserform.h"
#include "testoneuserform.h"

// UI Form headers
#include "trainingform.h"
#include "unlabeleddaqform.h"
#include "testoneform.h"

// Eigen API
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

#define TS 0.001
#define FS 1000

// sEMG MAV & WL Normalization
#define MAV_MAX_VAL 0.2204
#define WL_MAX_VAL 38.1962

// Real-time graph macros
#define GRAPH_MAV_Y_MIN 0.0
#define GRAPH_MAV_Y_MAX 0.3
#define GRAPH_IMU_MIN -180.0
#define GRAPH_IMU_MAX 180.0
#define GRAPH_FLEX_MIN -1.45
#define GRAPH_FLEX_MAX 0.45

// sEMG DAQ macros
#define DELSYS_CH_MAX 16
#define N_FEATURES 4
#define N_EMG 7
#define WIN_SIZE 150
#define N_MOTIONS 8
#define N_REPEAT 40

// Flexor sensor macros
# define N_FLEX_CH 5

// Maximal sum of MAV
#define MAV_MAX_AMP 10.0

// Minimal & maximal sampling probability
#define PROB_MIN 0.45
#define PROB_MAX 0.7
#define N_MOTION_MIN 3
#define N_MOTION_MAX 8

// Thresholds for distinguishing 'rest' and 'motions'
#define STD_THRES_SCALE 5.0

// IMU macros
#define N_IMU 1
#define N_IMU_CH 3
#define IMU_CHANNEL 0

// TensorRT flag
#define TRT_ON true

// sEMG MAV, WL, and Flex sensor bias & std calculation timing (Training)
#define T_OFFSET_START_TRN 0.3
#define T_OFFSET_END_TRN 1.0
#define T_REST_START_TRN 1.0
#define T_REST_END_TRN 3.0

// sEMG MAV, WL, and Flex sensor bias & std calculation timing (Ball Control)
#define T_OFFSET_START_BALL_CTR 0.5
#define T_OFFSET_END_BALL_CTR 1.5
#define T_REST_START_BALL_CTR 1.5
#define T_REST_END_BALL_CTR 4.5

// Time-related labeling variables
#define T_READY 5.0
#define T_REST 3.0
#define T_CONTRACT 5.0
#define T_MARGIN_START 0.5
#define T_MARGIN_END 0.5
#define T_MARGIN_START_TRN 0.5
#define T_MARGIN_END_TRN 0.1
#define T_MARGIN_START_UNLABELED_DAQ 1.0
#define T_MARGIN_END_UNLABELED_DAQ 0.1
#define T_RESIDUAL 60.0

#define WF_ROM 75.0
#define WE_ROM 60.0
#define RD_ROM 35.0
#define UD_ROM 35.0
#define WP_ROM 70.0
#define WS_ROM 70.0
#define HC_ROM 80.0
#define HO_ROM 10.0

// Ball control cases
#define N_TRIALS 3
#define N_TRAJECTORY 2

// Maximal ball control velocity
#define INC_MAX_X 1
#define INC_MAX_Y 1
#define INC_MAX_ROT 0.2
#define INC_MAX_SCALE 0.002

// Thresholds for regarding the current state as 'SUCCESS'
#define THRES_POS 25
#define THRES_ANG 5.0
#define THRES_SCALE 0.05

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

signals:
    void playTrainingVideo();
    void playUnlabeledDAQVideo();

    // (Training) Visual cue
    void setPalmRotX_Cue_Trn(double value);
    void setLowerArmRotY_Cue_Trn(double value);
    void setPalmRotZ_Cue_Trn(double value);
    void setThumbRotZ_Cue_Trn(double value);
    void setIndexRotZ_Cue_Trn(double value);
    void setMiddleRotZ_Cue_Trn(double value);
    void setRingRotZ_Cue_Trn(double value);
    void setLittleRotZ_Cue_Trn(double value);

    // (Training) Motion capture
    void setPalmRotX_MoCap_Trn(double value);
    void setLowerArmRotY_MoCap_Trn(double value);
    void setPalmRotZ_MoCap_Trn(double value);
    void setThumbRotZ_MoCap_Trn(double value);
    void setIndexRotZ_MoCap_Trn(double value);
    void setMiddleRotZ_MoCap_Trn(double value);
    void setRingRotZ_MoCap_Trn(double value);
    void setLittleRotZ_MoCap_Trn(double value);

    // (Unlabeled DAQ) Visual cue
    void setPalmRotX_Cue_UnlabeledDAQ(double value);
    void setLowerArmRotY_Cue_UnlabeledDAQ(double value);
    void setPalmRotZ_Cue_UnlabeledDAQ(double value);
    void setThumbRotZ_Cue_UnlabeledDAQ(double value);
    void setIndexRotZ_Cue_UnlabeledDAQ(double value);
    void setMiddleRotZ_Cue_UnlabeledDAQ(double value);
    void setRingRotZ_Cue_UnlabeledDAQ(double value);
    void setLittleRotZ_Cue_UnlabeledDAQ(double value);

    // (Unlabeled DAQ) Motion capture
    void setPalmRotX_MoCap_UnlabeledDAQ (double value);
    void setLowerArmRotY_MoCap_UnlabeledDAQ(double value);
    void setPalmRotZ_MoCap_UnlabeledDAQ(double value);
    void setThumbRotZ_MoCap_UnlabeledDAQ(double value);
    void setIndexRotZ_MoCap_UnlabeledDAQ(double value);
    void setMiddleRotZ_MoCap_UnlabeledDAQ(double value);
    void setRingRotZ_MoCap_UnlabeledDAQ(double value);
    void setLittleRotZ_MoCap_UnlabeledDAQ(double value);

    // (Test One) Motion capture
    void setPalmRotX_MoCap_TestOne (double value);
    void setLowerArmRotY_MoCap_TestOne(double value);
    void setPalmRotZ_MoCap_TestOne(double value);
    void setThumbRotZ_MoCap_TestOne(double value);
    void setIndexRotZ_MoCap_TestOne(double value);
    void setMiddleRotZ_MoCap_TestOne(double value);
    void setRingRotZ_MoCap_TestOne(double value);
    void setLittleRotZ_MoCap_TestOne(double value);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Dialog open
    void OpenTrainingUserForm();

    // Real-time graph initialization
    void GraphInit_sEMG(QCustomPlot* Graph_);
    void GraphInit_IMU(QCustomPlot* Graph_);
    void GraphInit_Flex(QCustomPlot* Graph_);
    void GraphInit_Label(QCustomPlot* Graph_, int Session);

    // Real-time graph plot
    void RealTimeDataPlot();

    // Individual Initialization
    void Initialize_NI();
    void Initialize_SerialComm();
    void Initialize_GUI();
    void Initialize_Variables();
    void Initialize_DAVariables();
    void Initialize_MenuBar();
    void Initialize_StackedWidget(int mode);
    void Initialize_Figures();

    // Wrap-up Initialization
    void Initialize_trnForm();
    void Initialize_unlabeledDAQForm();
    void Initialize_testOneForm();
    void Free_Variables();

    // DAQ
    void DAQ_Online();
    void DAQ_Offline();

    // sEMG processing
    void Feature_Extraction();
    void Normalize_Features();
    void Offset_Calculation();

    // Flex sensor processing
    void Flex_Angle_Calculation();

    // Motion classification
    void Rest_mean_std_Calculation();
    bool Rest_Mot_Classification();
    int  Get_Motion_Label(int mode, double time);
    int  Get_State_Indicator(int mode, double time);
    void Set_Motion_Trajectory(int mode);

    // Ball control
    bool Determine_Success(int traj_idx);

    // Serial Connection
    void fillPortsInfo();
    QSerialPort* m_serialPort = nullptr;

    // sEMG feature extraction
    void Calculate_MAV();
    void Calculate_WL();
    void Calculate_SSC();
    void Calculate_ZC();
    void Normalize_MAV();
    void Normalize_WL();
    void Normalize_SSC();
    void Normalize_ZC();
    void Calculate_MAV_Amp();

    // Stack & Save data
    void StackData();
    void PopUpData(unsigned int N_delete);
    void SaveData_Training();
    void SaveData_UnlabeledDAQ();
    void SaveData_BallControl(int trial_idx, int traj_idx);
    void ClearData_Training();
    void ClearData_UnlabeledDAQ();
    void ClearData_BallControl(int trial_idx, int traj_idx);

public:
    // 'Get' functions
    TrainingUserForm* get_VideoPlayer_Training();
    UnlabeledDAQUserForm* get_VideoPlayer_UnlabeledDAQ();

    // 'Set' functions
    void Set_TimeStr();

private slots:
    // Serial Connection
    void readSerialIMU();
    void handleError(QSerialPort::SerialPortError error);

    // GUI slots
    void OnTimerControlTimeCallbackFunc();
    void OnTimerRTPlotCallbackFunc();
    void OnTimerFigCallbackFunc();
    void on_BtnSwitch_clicked();
    void on_BtnDelete_clicked();
    void on_BtnStackSave_clicked();
    void on_BtnLoad_clicked();
    void on_lineEdit_WinSize_textEdited(const QString &arg1);
    void on_lineEdit_MAVMax_textEdited(const QString &arg1);
    void on_lineEdit_Sbj_Name_textEdited(const QString &arg1);
    void on_lineEdit_Trial_Idx_textEdited(const QString &arg1);
    void on_lineEdit_Traj_Idx_textEdited(const QString &arg1);

    // Radio controls
    void RadioCtrl_Mode();
    void RadioCtrl_Network();
    void RadioCtrl_SaveOpt();
    void RadioCtrl_StrOpt();
    void Close_Dialog();

    void on_BtnSkipPractice_clicked();

private:
    Ui::MainWindow *ui;

    // Ball control limb pos & trial idx
    int Trial_idx = 1;
    int Traj_idx = 1;

    bool PracticeOrNot = true;
    bool isSuccess = false;
    bool isSuccess_time_flag = false;
    bool isTimeOver = false;
    bool isTimeOver_time_flag = false;
    bool isRun = false;
    bool isFirstRun = true;
    bool** SuccessOrNot;
    double** BallCtrElapsedTime;

    double T_trial_start = 0.0;
    double T_trial_end = 0.0;

    // UI Form variables
    TrainingForm trnForm;
    UnlabeledDAQForm unlabeledDAQForm;
    TestOneForm testOneForm;

    // User dialog variables
    TrainingUserForm* player_Training;
    UnlabeledDAQUserForm* player_UnlabeledDAQ;
    TestOneUserForm player_TestOne;

    // Serial Connection
    QString portName;

    // sEMG variables
    int Win_size;
    double* sEMG_raw;
    double* sEMG_MAV;
    double  sum_sEMG_MAV = 0.0;
    double* sEMG_MAV_norm;
    double* sEMG_WL;
    double* sEMG_WL_norm;
    double* sEMG_SSC;
    double* sEMG_SSC_norm;
    double* sEMG_ZC;
    double* sEMG_ZC_norm;

    double* sEMG_MAV_offset;
    double* sEMG_MAV_rest_mean;
    double* sEMG_MAV_rest_square_mean;
    double* sEMG_MAV_rest_std;
    double sum_sEMG_MAV_rest_mean = 0.0;
    double sum_sEMG_MAV_rest_std = 0.0;
    double* sEMG_WL_offset;
    double* sEMG_WL_rest_mean;
    double* sEMG_WL_rest_square_mean;
    double* sEMG_WL_rest_std;
    double sum_sEMG_WL_rest_mean = 0.0;
    double sum_sEMG_WL_rest_std = 0.0;

    double rest_thres = 0.0;
    int*   sEMG_CH;

    // Randomized motion rate & Corresponding target motion list
    double* Motion_prob;
    int*    Target_motion_N;
    int*    Target_motion_list;

    // IMU variables
    double* IMU_Ori_Lower;
    double* IMU_Gyro_Lower;
    double* IMU_Acc_Lower;
    double* IMU_Mag_Lower;

    // Flexor sensor variables
    double* Flex_raw;
    double* Flex_raw_offset;
    double* Flex_processed;
    double* Flex_Angle;

    // Label & Estimation variables
    int Motion_label = 0;
    int State_indicator = 0;
    int Motion_est = 0;
    int Motion_est_Fine_Tune = 0;
    int Motion_est_STM = 0;
    int Motion_est_Proposed = 0;

    // RTGraph variables
    double Graph_MAV_max;

    // Time elapse variables
    double Time_elapse_Processing = 0.0;
    double Time_elapse_TensorRT = 0.0;
    double Time_elapse_Algorithm = 0.0;

    // Vecotrs for Training & Unlabeled DAQ
    std::vector<double>* stack_sEMG_raw;
    std::vector<double>* stack_sEMG_MAV;
    std::vector<double>* stack_sEMG_WL;
    std::vector<double>* stack_sEMG_SSC;
    std::vector<double>* stack_sEMG_ZC;

    std::vector<double>* stack_IMU_ori;
    std::vector<double>* stack_IMU_gyro;
    std::vector<double>* stack_IMU_acc;
    std::vector<double>* stack_IMU_mag;

    std::vector<double>* stack_Flex_raw;
    std::vector<double>* stack_Flex_processed;
    std::vector<double>* stack_Flex_Angle;

    std::vector<double>  stack_Time_elapse_Processing;
    std::vector<double>  stack_Time_elapse_TensorRT;
    std::vector<int>     stack_Motion_label;

    // Vectors for Ball Control
    std::vector<double>*** stack_Ball_Ctr_sEMG_raw;
    std::vector<double>*** stack_Ball_Ctr_sEMG_MAV;
    std::vector<double>*** stack_Ball_Ctr_sEMG_WL;
    std::vector<double>*** stack_Ball_Ctr_sEMG_SSC;
    std::vector<double>*** stack_Ball_Ctr_sEMG_ZC;

    std::vector<double>*** stack_Ball_Ctr_IMU_ori;
    std::vector<double>*** stack_Ball_Ctr_IMU_gyro;
    std::vector<double>*** stack_Ball_Ctr_IMU_acc;
    std::vector<double>*** stack_Ball_Ctr_IMU_mag;

    std::vector<double>*** stack_Ball_Ctr_Flex_raw;
    std::vector<double>*** stack_Ball_Ctr_Flex_processed;
    std::vector<double>*** stack_Ball_Ctr_Flex_Angle;

    std::vector<double>** stack_Ball_Ctr_X;
    std::vector<double>** stack_Ball_Ctr_Y;
    std::vector<double>** stack_Ball_Ctr_Rot;
    std::vector<double>** stack_Ball_Ctr_Scale;

    std::vector<double>** stack_Ball_Ctr_Time_elapse_Processing;
    std::vector<int>**    stack_Ball_Ctr_Motion_est;

    // Vectors stacked in TensorRT thread
    std::vector<double>** stack_Ball_Ctr_sEMG_MAV_amp;
    std::vector<double>** stack_Ball_Ctr_Time_elapse_TensorRT;
    std::vector<int>**    stack_Ball_Ctr_isMotionExerted;
    std::vector<int>**    stack_Ball_Ctr_Motion_est_TensorRT_thread;

    // Arm trajectory
    std::vector<double> trj_shd_X;
    std::vector<double> trj_shd_Y;
    std::vector<double> trj_shd_axis;
    std::vector<double> trj_shd_Z;
    std::vector<double> trj_elb_X;

    // Wrist trajectory
    std::vector<double> trj_wrist_X;
    std::vector<double> trj_wrist_Y;
    std::vector<double> trj_wrist_Z;

    // Hand trajectory
    std::vector<double> trj_hand;

    // Loaded data variable
    bool isDataLoaded = false;
    std::vector<double>  stack_Motion_label_load;
    std::vector<double>* stack_sEMG_MAV_load;
    std::vector<double>* stack_sEMG_WL_load;
    std::vector<double>* stack_sEMG_SSC_load;
    std::vector<double>* stack_sEMG_ZC_load;

    // Vector-delete variables
    unsigned int N_delete;

    // Labeling variables
    double Train_M_start[N_MOTIONS];
    double Train_M_end[N_MOTIONS];
    double Pred_M_start[N_MOTIONS];
    double Pred_M_end[N_MOTIONS];

    // Timer
    QTimer m_pTimer_ControlTime;
    QTimer m_pTimer_plot;
    QTimer m_pTimer_fig;
    int key_cnt = 0;

    // TwinCAT variables
    double m_time = 0.0;
    double m_time_last = 0.0;
    int m_time_cnt = 0;
    int m_time_last_cnt = 0;
    std::mutex mtx;
    bool m_flag_Switch = 0;
    bool isTwinCATStarted = false;
    HANDLE hMutex;
    HANDLE hMemory;
    STARTUPINFO	startupinfo;
    PROCESS_INFORMATION	processinfo;
    struct Shared_Data* pShared_Data;

    // NI_DAQ variables
    NI_AI_sEMG* AI_sEMG;
    NI_AI_Flex* AI_Flex;

    // TensorRT module
    bool isEstimated = true;
    bool isTensorRTUsed = false;
    TensorRT_module* TRT_Fine_Tune = nullptr;
    TensorRT_module* TRT_STM = nullptr;
    TensorRT_module* TRT_Proposed = nullptr;

    // Real-Time Graph
    double key_current;
    double Width_X = 10.0;

    // Workers for std::thread
    void Thread_TwinCAT_func();
    void Thread_TensorRT_func();

    // Variables for Saving Data
    QString PathStr;
    QString TimeStr;
    QString NameStr = "KimSejin";
    QString ModeStr;
    QString NetworkStr;
    QString SaveFolderStr;
    QTextStream SaveOut;

    // RadioButton variables
    int m_radioMode;
    int m_radioNetwork;
    int m_radioSaveOpt;
    int m_radioStrOpt;

    // Figure container variables
    cv::Mat* Figures_motion;
};
#endif // MAINWINDOW_H
