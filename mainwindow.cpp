#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./ui_trainingform.h"
#include "./ui_unlabeleddaqform.h"
#include "./ui_testoneform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_serialPort(new QSerialPort(this)),
    ui(new Ui::MainWindow),
    m_radioMode(0), m_radioNetwork(0), m_radioSaveOpt(2), m_radioStrOpt(0) {
    ui->setupUi(this);
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Initialization
    Initialize_MenuBar();
    Initialize_Variables();
    Initialize_DAVariables();
    Initialize_NI();
    Initialize_GUI();
    Initialize_Figures();
    Initialize_StackedWidget(m_radioMode);

    Initialize_trnForm();
    Initialize_unlabeledDAQForm();
    Initialize_testOneForm();

    Set_Motion_Trajectory(m_radioMode);

    // Training video player) For immediate play & pause
    player_Training = new TrainingUserForm();
    player_Training->get_QMediaPlaylist()->setCurrentIndex(0);
    player_Training->get_QMediaPlayer()->play();
    player_Training->get_QMediaPlayer()->pause();
    connect(this, SIGNAL(playTrainingVideo()), player_Training->get_QMediaPlayer(), SLOT(play()));

    // Unlabeled DAQ video player) For immediate play & pause
    player_UnlabeledDAQ = new UnlabeledDAQUserForm();
    player_UnlabeledDAQ->get_QMediaPlaylist()->setCurrentIndex(0);
    player_UnlabeledDAQ->get_QMediaPlayer()->play();
    player_UnlabeledDAQ->get_QMediaPlayer()->pause();
    connect(this, SIGNAL(playUnlabeledDAQVideo()), player_UnlabeledDAQ->get_QMediaPlayer(), SLOT(play()));

    QDesktopWidget widget;
    QRect screenGeometry = widget.availableGeometry(widget.primaryScreen());
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    player_Training->move((width - 1600) / 2 + 1920,
                          (height - 900) / 2 - 30);
    player_UnlabeledDAQ->move((width - 1600) / 2 + 1920,
                              (height - 900) / 2 - 30);

//    if (m_radioMode == 0) { // Training mode
//        player_Training->show();
//    }
//    else if (m_radioMode == 1) { // Unlabeled dataset acquisition mode
//        player_UnlabeledDAQ->show();
//    }

    // 3D model) signal connection
    connect(this, &MainWindow::setPalmRotX_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Palm_RotX);
    connect(this, &MainWindow::setLowerArmRotY_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Lower_arm_RotY);
    connect(this, &MainWindow::setPalmRotZ_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Palm_RotZ);
    connect(this, &MainWindow::setThumbRotZ_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Thumb_RotZ);
    connect(this, &MainWindow::setIndexRotZ_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Index_RotZ);
    connect(this, &MainWindow::setMiddleRotZ_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Middle_RotZ);
    connect(this, &MainWindow::setRingRotZ_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Ring_RotZ);
    connect(this, &MainWindow::setLittleRotZ_Cue_Trn, player_Training->get_Model(), &SceneModifier::set_Little_RotZ);

    connect(this, &MainWindow::setPalmRotX_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Palm_RotX);
    connect(this, &MainWindow::setLowerArmRotY_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Lower_arm_RotY);
    connect(this, &MainWindow::setPalmRotZ_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Palm_RotZ);
    connect(this, &MainWindow::setThumbRotZ_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Thumb_RotZ);
    connect(this, &MainWindow::setIndexRotZ_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Index_RotZ);
    connect(this, &MainWindow::setMiddleRotZ_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Middle_RotZ);
    connect(this, &MainWindow::setRingRotZ_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Ring_RotZ);
    connect(this, &MainWindow::setLittleRotZ_MoCap_Trn, trnForm.get_Model(), &SceneModifier::set_Little_RotZ);

    connect(this, &MainWindow::setPalmRotX_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Palm_RotX);
    connect(this, &MainWindow::setLowerArmRotY_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Lower_arm_RotY);
    connect(this, &MainWindow::setPalmRotZ_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Palm_RotZ);
    connect(this, &MainWindow::setThumbRotZ_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Thumb_RotZ);
    connect(this, &MainWindow::setIndexRotZ_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Index_RotZ);
    connect(this, &MainWindow::setMiddleRotZ_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Middle_RotZ);
    connect(this, &MainWindow::setRingRotZ_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Ring_RotZ);
    connect(this, &MainWindow::setLittleRotZ_Cue_UnlabeledDAQ, player_UnlabeledDAQ->get_Model(), &SceneModifier::set_Little_RotZ);

    connect(this, &MainWindow::setPalmRotX_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Palm_RotX);
    connect(this, &MainWindow::setLowerArmRotY_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Lower_arm_RotY);
    connect(this, &MainWindow::setPalmRotZ_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Palm_RotZ);
    connect(this, &MainWindow::setThumbRotZ_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Thumb_RotZ);
    connect(this, &MainWindow::setIndexRotZ_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Index_RotZ);
    connect(this, &MainWindow::setMiddleRotZ_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Middle_RotZ);
    connect(this, &MainWindow::setRingRotZ_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Ring_RotZ);
    connect(this, &MainWindow::setLittleRotZ_MoCap_UnlabeledDAQ, unlabeledDAQForm.get_Model(), &SceneModifier::set_Little_RotZ);

    connect(this, &MainWindow::setPalmRotX_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Palm_RotX);
    connect(this, &MainWindow::setLowerArmRotY_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Lower_arm_RotY);
    connect(this, &MainWindow::setPalmRotZ_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Palm_RotZ);
    connect(this, &MainWindow::setThumbRotZ_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Thumb_RotZ);
    connect(this, &MainWindow::setIndexRotZ_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Index_RotZ);
    connect(this, &MainWindow::setMiddleRotZ_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Middle_RotZ);
    connect(this, &MainWindow::setRingRotZ_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Ring_RotZ);
    connect(this, &MainWindow::setLittleRotZ_MoCap_TestOne, testOneForm.get_Model(), &SceneModifier::set_Little_RotZ);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::OpenTrainingUserForm() {
    player_Training->show();
}

/////////////////////////////////// Initialization ///////////////////////////////////
void MainWindow::GraphInit_sEMG(QCustomPlot* Graph_) {
    Graph_->addGraph();
    Graph_->graph(0)->setPen(QPen(QColor(255, 0, 0))); // Red line
    Graph_->graph(0)->setName("CH 1");
    Graph_->addGraph();
    Graph_->graph(1)->setPen(QPen(QColor(0, 255, 0))); // Green line
    Graph_->graph(1)->setName("CH 2");
    Graph_->addGraph();
    Graph_->graph(2)->setPen(QPen(QColor(0, 0, 255))); // Blue line
    Graph_->graph(2)->setName("CH 3");
    Graph_->addGraph();
    Graph_->graph(3)->setPen(QPen(QColor(255, 0, 255))); // Purple line
    Graph_->graph(3)->setName("CH 4");
    Graph_->addGraph();
    Graph_->graph(4)->setPen(QPen(QColor(255, 255, 0))); // Yellow line
    Graph_->graph(4)->setName("CH 5");
    Graph_->addGraph();
    Graph_->graph(5)->setPen(QPen(QColor(0, 255, 255))); // Magenta line
    Graph_->graph(5)->setName("CH 6");
    Graph_->addGraph();
    Graph_->graph(6)->setPen(QPen(QColor(0, 0, 0))); // Black line
    Graph_->graph(6)->setName("CH 7");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    Graph_->xAxis->setTicker(timeTicker);
    Graph_->axisRect()->setupFullAxesBox();
    Graph_->yAxis->setRange(GRAPH_MAV_Y_MIN, GRAPH_MAV_Y_MAX);

    Graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    Graph_->axisRect()->setRangeDrag(Qt::Vertical); // Only Y-directional drag is available.

    Graph_->axisRect()->setAutoMargins(QCP::msLeft | QCP::msTop | QCP::msBottom);
    Graph_->axisRect()->setMargins(QMargins(0,0,10,0));
    Graph_->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
    Graph_->axisRect()->insetLayout()->setInsetRect(0, QRectF(1.1,0,0.1,0.1));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(Graph_->xAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->xAxis2, SLOT(setRange(QCPRange)));
    connect(Graph_->yAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::GraphInit_IMU(QCustomPlot* Graph_) {
    Graph_->addGraph();
    Graph_->graph(0)->setPen(QPen(QColor(255, 0, 0))); // Red line
    Graph_->graph(0)->setName("Roll");
    Graph_->addGraph();
    Graph_->graph(1)->setPen(QPen(QColor(0, 255, 0))); // Green line
    Graph_->graph(1)->setName("Pitch");
    Graph_->addGraph();
    Graph_->graph(2)->setPen(QPen(QColor(0, 0, 255))); // Blue line
    Graph_->graph(2)->setName("Yaw");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    Graph_->xAxis->setTicker(timeTicker);
    Graph_->axisRect()->setupFullAxesBox();

    Graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    Graph_->axisRect()->setRangeDrag(Qt::Vertical); // Only Y-directional drag is available.

    Graph_->axisRect()->setAutoMargins(QCP::msLeft | QCP::msTop | QCP::msBottom);
    Graph_->axisRect()->setMargins(QMargins(0,0,10,0));
    Graph_->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
    Graph_->axisRect()->insetLayout()->setInsetRect(0, QRectF(1.1,0,0.1,0.1));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(Graph_->xAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->xAxis2, SLOT(setRange(QCPRange)));
    connect(Graph_->yAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::GraphInit_Flex(QCustomPlot* Graph_) {
    Graph_->addGraph();
    Graph_->graph(0)->setPen(QPen(QColor(255, 0, 0))); // Red line
    Graph_->graph(0)->setName("Thumb");
    Graph_->addGraph();
    Graph_->graph(1)->setPen(QPen(QColor(0, 255, 0))); // Green line
    Graph_->graph(1)->setName("Index");
    Graph_->addGraph();
    Graph_->graph(2)->setPen(QPen(QColor(0, 0, 255))); // Blue line
    Graph_->graph(2)->setName("Middle");
    Graph_->addGraph();
    Graph_->graph(3)->setPen(QPen(QColor(255, 0, 255))); // Purple line
    Graph_->graph(3)->setName("Ring");
    Graph_->addGraph();
    Graph_->graph(4)->setPen(QPen(QColor(255, 255, 0))); // Yellow line
    Graph_->graph(4)->setName("Little");

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    Graph_->xAxis->setTicker(timeTicker);
    Graph_->axisRect()->setupFullAxesBox();
    Graph_->yAxis->setRange(GRAPH_FLEX_MIN, GRAPH_FLEX_MAX);

    Graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    Graph_->axisRect()->setRangeDrag(Qt::Vertical); // Only Y-directional drag is available.

    Graph_->axisRect()->setAutoMargins(QCP::msLeft | QCP::msTop | QCP::msBottom);
    Graph_->axisRect()->setMargins(QMargins(0,0,10,0));
    Graph_->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
    Graph_->axisRect()->insetLayout()->setInsetRect(0, QRectF(1.1,0,0.1,0.1));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(Graph_->xAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->xAxis2, SLOT(setRange(QCPRange)));
    connect(Graph_->yAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::GraphInit_Label(QCustomPlot* Graph_, int Session) {
    if (Session == 0 || Session == 1) {
        Graph_->addGraph();
        Graph_->graph(0)->setPen(QPen(QColor(255, 0, 0))); // Red line
        Graph_->graph(0)->setName("Ground Truth");
    }
    else if (Session == 2) {
        Graph_->addGraph();
        Graph_->graph(0)->setPen(QPen(QColor(255, 0, 0))); // Red line
        Graph_->graph(0)->setName("Estimation");
    }

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    Graph_->xAxis->setTicker(timeTicker);
    Graph_->axisRect()->setupFullAxesBox();
    Graph_->yAxis->setRange(-0.5, N_MOTIONS + 1.5);

    Graph_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    Graph_->axisRect()->setRangeDrag(Qt::Vertical); // Only Y-directional drag is available.

    Graph_->axisRect()->setAutoMargins(QCP::msLeft | QCP::msTop | QCP::msBottom);
    Graph_->axisRect()->setMargins(QMargins(0,0,10,0));
    Graph_->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
    Graph_->axisRect()->insetLayout()->setInsetRect(0, QRectF(1.1,0,0.1,0.1));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(Graph_->xAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->xAxis2, SLOT(setRange(QCPRange)));
    connect(Graph_->yAxis, SIGNAL(rangeChanged(QCPRange)),
            Graph_->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::Initialize_Variables() {
    Win_size = WIN_SIZE;
    Graph_MAV_max = GRAPH_MAV_Y_MAX;
    N_delete = 0;

    if (m_radioNetwork == 0)
        NetworkStr = "_Fine_Tune";
    else if (m_radioNetwork == 1)
        NetworkStr = "_STM";
    else if (m_radioNetwork == 2)
        NetworkStr = "_Proposed";
}

void MainWindow::Initialize_DAVariables() {
    /////////////////////////////////// Training & Prediction ///////////////////////////////////
    stack_sEMG_raw = new std::vector<double>[N_EMG];
    stack_sEMG_MAV = new std::vector<double>[N_EMG];
    stack_sEMG_WL = new std::vector<double>[N_EMG];
    stack_sEMG_SSC = new std::vector<double>[N_EMG];
    stack_sEMG_ZC = new std::vector<double>[N_EMG];

    stack_Flex_raw = new std::vector<double>[N_FLEX_CH];
    stack_Flex_processed = new std::vector<double>[N_FLEX_CH];
    stack_Flex_Angle = new std::vector<double>[N_FLEX_CH];

    sEMG_MAV = new float64[N_EMG];
    sEMG_MAV_norm = new float64[N_EMG];
    sEMG_WL = new float64[N_EMG];
    sEMG_WL_norm = new float64[N_EMG];
    sEMG_SSC = new float64[N_EMG];
    sEMG_SSC_norm = new float64[N_EMG];
    sEMG_ZC = new float64[N_EMG];
    sEMG_ZC_norm = new float64[N_EMG];

    sEMG_MAV_offset = new float64[N_EMG];
    sEMG_MAV_rest_mean = new float64[N_EMG];
    sEMG_MAV_rest_square_mean = new float64[N_EMG];
    sEMG_MAV_rest_std = new float64[N_EMG];

    sEMG_WL_offset = new float64[N_EMG];
    sEMG_WL_rest_mean = new float64[N_EMG];
    sEMG_WL_rest_square_mean = new float64[N_EMG];
    sEMG_WL_rest_std = new float64[N_EMG];

    for (int i = 0; i < N_EMG; i++) {
        sEMG_MAV[i] = 0.0;
        sEMG_MAV_norm[i] = 0.0;
        sEMG_WL[i] = 0.0;
        sEMG_WL_norm[i] = 0.0;
        sEMG_SSC[i] = 0.0;
        sEMG_SSC_norm[i] = 0.0;
        sEMG_ZC[i] = 0.0;
        sEMG_ZC_norm[i] = 0.0;

        sEMG_MAV_offset[i] = 0.0;
        sEMG_MAV_rest_mean[i] = 0.0;
        sEMG_MAV_rest_square_mean[i] = 0.0;
        sEMG_MAV_rest_std[i] = 0.0;

        sEMG_WL_offset[i] = 0.0;
        sEMG_WL_rest_mean[i] = 0.0;
        sEMG_WL_rest_square_mean[i] = 0.0;
        sEMG_WL_rest_std[i] = 0.0;
    }

    sEMG_CH = new int[N_EMG];
    sEMG_CH[0] = 0;
    sEMG_CH[1] = 1;
    sEMG_CH[2] = 2;
    sEMG_CH[3] = 3;
    sEMG_CH[4] = 4;
    sEMG_CH[5] = 5;
    sEMG_CH[6] = 6;

    Motion_prob = new double[N_MOTIONS];
    Target_motion_N = new int[N_MOTIONS];
    for (int m = 0; m < N_MOTIONS; m++) {
        Motion_prob[m] = 0.0;
        Target_motion_N[m] = 0;
    }

    Target_motion_list = new int[N_REPEAT];
    for (int i = 0; i < N_REPEAT; i++) {
        Target_motion_list[i] = 0;
    }

    /////////////////////////////////// Ball control vector ///////////////////////////////////
    stack_Ball_Ctr_sEMG_raw = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_sEMG_MAV = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_sEMG_WL = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_sEMG_SSC = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_sEMG_ZC = new std::vector<double>**[N_TRIALS];

    stack_Ball_Ctr_IMU_ori = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_IMU_gyro = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_IMU_acc = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_IMU_mag = new std::vector<double>**[N_TRIALS];

    stack_Ball_Ctr_Flex_raw = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_Flex_processed = new std::vector<double>**[N_TRIALS];
    stack_Ball_Ctr_Flex_Angle = new std::vector<double>**[N_TRIALS];

    stack_Ball_Ctr_X = new std::vector<double>*[N_TRIALS];
    stack_Ball_Ctr_Y = new std::vector<double>*[N_TRIALS];
    stack_Ball_Ctr_Rot = new std::vector<double>*[N_TRIALS];
    stack_Ball_Ctr_Scale = new std::vector<double>*[N_TRIALS];

    stack_Ball_Ctr_Time_elapse_Processing = new std::vector<double>*[N_TRIALS];
    stack_Ball_Ctr_Motion_est = new std::vector<int>*[N_TRIALS];

    stack_Ball_Ctr_sEMG_MAV_amp = new std::vector<double>*[N_TRIALS];
    stack_Ball_Ctr_Time_elapse_TensorRT = new std::vector<double>*[N_TRIALS];
    stack_Ball_Ctr_isMotionExerted = new std::vector<int>*[N_TRIALS];
    stack_Ball_Ctr_Motion_est_TensorRT_thread = new std::vector<int>*[N_TRIALS];

    for (int i = 0; i < N_TRIALS; i++) {
        stack_Ball_Ctr_sEMG_raw[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_sEMG_MAV[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_sEMG_WL[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_sEMG_SSC[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_sEMG_ZC[i] = new std::vector<double>*[N_TRAJECTORY];

        stack_Ball_Ctr_IMU_ori[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_IMU_acc[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_IMU_gyro[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_IMU_mag[i] = new std::vector<double>*[N_TRAJECTORY];

        stack_Ball_Ctr_Flex_raw[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_Flex_processed[i] = new std::vector<double>*[N_TRAJECTORY];
        stack_Ball_Ctr_Flex_Angle[i] = new std::vector<double>*[N_TRAJECTORY];

        for (int j = 0; j < N_TRAJECTORY; j++) {
            stack_Ball_Ctr_sEMG_raw[i][j] = new std::vector<double>[N_EMG];
            stack_Ball_Ctr_sEMG_MAV[i][j] = new std::vector<double>[N_EMG];
            stack_Ball_Ctr_sEMG_WL[i][j] = new std::vector<double>[N_EMG];
            stack_Ball_Ctr_sEMG_SSC[i][j] = new std::vector<double>[N_EMG];
            stack_Ball_Ctr_sEMG_ZC[i][j] = new std::vector<double>[N_EMG];

            stack_Ball_Ctr_IMU_ori[i][j] = new std::vector<double>[N_IMU_CH];
            stack_Ball_Ctr_IMU_acc[i][j] = new std::vector<double>[N_IMU_CH];
            stack_Ball_Ctr_IMU_gyro[i][j] = new std::vector<double>[N_IMU_CH];
            stack_Ball_Ctr_IMU_mag[i][j] = new std::vector<double>[N_IMU_CH];

            stack_Ball_Ctr_Flex_raw[i][j] = new std::vector<double>[N_FLEX_CH];
            stack_Ball_Ctr_Flex_processed[i][j] = new std::vector<double>[N_FLEX_CH];
            stack_Ball_Ctr_Flex_Angle[i][j] = new std::vector<double>[N_FLEX_CH];
        }

        stack_Ball_Ctr_X[i] = new std::vector<double>[N_TRAJECTORY];
        stack_Ball_Ctr_Y[i] = new std::vector<double>[N_TRAJECTORY];
        stack_Ball_Ctr_Rot[i] = new std::vector<double>[N_TRAJECTORY];
        stack_Ball_Ctr_Scale[i] = new std::vector<double>[N_TRAJECTORY];

        stack_Ball_Ctr_Time_elapse_Processing[i] = new std::vector<double>[N_TRAJECTORY];
        stack_Ball_Ctr_Motion_est[i] = new std::vector<int>[N_TRAJECTORY];

        stack_Ball_Ctr_sEMG_MAV_amp[i] = new std::vector<double>[N_TRAJECTORY];
        stack_Ball_Ctr_Time_elapse_TensorRT[i] = new std::vector<double>[N_TRAJECTORY];
        stack_Ball_Ctr_isMotionExerted[i] = new std::vector<int>[N_TRAJECTORY];
        stack_Ball_Ctr_Motion_est_TensorRT_thread[i] = new std::vector<int>[N_TRAJECTORY];
    }

    Flex_raw_offset = new float64[N_FLEX_CH];
    Flex_processed = new float64[N_FLEX_CH];
    Flex_Angle = new float64[N_FLEX_CH];
    for (int i = 0; i < N_FLEX_CH; i++) {
        Flex_raw_offset[i] = 0.0;
        Flex_processed[i] = 0.0;
        Flex_Angle[i] = 0.0;
    }

    SuccessOrNot = new bool*[N_TRIALS];
    BallCtrElapsedTime = new double*[N_TRIALS];
    for (int i = 0; i < N_TRIALS; i++) {
        SuccessOrNot[i] = new bool[N_TRAJECTORY];
        BallCtrElapsedTime[i] = new double[N_TRAJECTORY];
    }

    for (int i = 0; i < N_TRIALS; i++) {
        for (int j = 0; j < N_TRAJECTORY; j++) {
            SuccessOrNot[i][j] = false;
            BallCtrElapsedTime[i][j] = 0.0;
        }
    }
}

void MainWindow::Initialize_NI() {
    AI_sEMG = new NI_AI_sEMG("Dev1/ai0:15", DELSYS_CH_MAX);
    NI_AI_sEMG::InitializeNI();

    AI_Flex = new NI_AI_Flex("Dev2/ai0:4", N_FLEX_CH);
    NI_AI_Flex::InitializeNI();
}

void MainWindow::Initialize_GUI() {
    // 1. LineEdit initialization
    ui->lineEdit_WinSize->setText(QString::number(WIN_SIZE));
    ui->lineEdit_MAVMax->setText(QString::number(GRAPH_MAV_Y_MAX));
    ui->lineEdit_Sbj_Name->setText(NameStr);
    ui->lineEdit_Trial_Idx->setText(QString::number(Trial_idx));
    ui->lineEdit_Traj_Idx->setText(QString::number(Traj_idx));

    ui->lineEdit_Sbj_Name->setEnabled(false);
    ui->lineEdit_Trial_Idx->setEnabled(false);
    ui->lineEdit_Traj_Idx->setEnabled(false);
    ui->lineEdit_WinSize->setEnabled(false);
    ui->lineEdit_MAVMax->setEnabled(false);

    ui->BtnSkipPractice->setEnabled(false);

    ui->radioBtn_Mode_1->setChecked(true);
    ui->radioBtn_Mode_2->setEnabled(true);
    ui->radioBtn_Mode_3->setEnabled(true);
    ui->radioBtn_Mode_4->setEnabled(false);

    ui->radioBtn_Network_1->setEnabled(false);
    ui->radioBtn_Network_2->setEnabled(false);
    ui->radioBtn_Network_3->setEnabled(false);

    PathStr = QString("D:/STDAN_TensorRT/Data/");
    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
    if (m_radioMode == 0)
        ModeStr = QString("_Training");

    if (m_radioMode == 0) {
        SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr;
    }
    ui->lineEdit_SaveFolder->setText(SaveFolderStr);

    if (m_radioSaveOpt == 0)
        ui->radioBtn_SaveOption_1->setChecked(true);
    else if (m_radioSaveOpt == 1)
        ui->radioBtn_SaveOption_2->setChecked(true);
    else if (m_radioSaveOpt == 2)
        ui->radioBtn_SaveOption_3->setChecked(true);
    else if (m_radioSaveOpt == 3)
        ui->radioBtn_SaveOption_4->setChecked(true);

    ui->radioBtn_DataStream_1->setEnabled(true);
    ui->radioBtn_DataStream_2->setEnabled(true);
    if (m_radioStrOpt == 0) {
        ui->radioBtn_DataStream_1->setChecked(true);
        ui->BtnLoad->setEnabled(false);
    }
    else if (m_radioStrOpt == 1) {
        ui->radioBtn_DataStream_2->setChecked(true);
        ui->BtnLoad->setEnabled(true);
    }

    ui->BtnStackSave->setEnabled(false);

    connect(ui->radioBtn_Mode_1, SIGNAL(clicked()), this, SLOT(RadioCtrl_Mode()));
    connect(ui->radioBtn_Mode_2, SIGNAL(clicked()), this, SLOT(RadioCtrl_Mode()));
    connect(ui->radioBtn_Mode_3, SIGNAL(clicked()), this, SLOT(RadioCtrl_Mode()));
    connect(ui->radioBtn_Mode_4, SIGNAL(clicked()), this, SLOT(RadioCtrl_Mode()));

    connect(ui->radioBtn_Network_1, SIGNAL(clicked()), this, SLOT(RadioCtrl_Network()));
    connect(ui->radioBtn_Network_2, SIGNAL(clicked()), this, SLOT(RadioCtrl_Network()));
    connect(ui->radioBtn_Network_3, SIGNAL(clicked()), this, SLOT(RadioCtrl_Network()));

    connect(ui->radioBtn_SaveOption_1, SIGNAL(clicked()), this, SLOT(RadioCtrl_SaveOpt()));
    connect(ui->radioBtn_SaveOption_2, SIGNAL(clicked()), this, SLOT(RadioCtrl_SaveOpt()));
    connect(ui->radioBtn_SaveOption_3, SIGNAL(clicked()), this, SLOT(RadioCtrl_SaveOpt()));
    connect(ui->radioBtn_SaveOption_4, SIGNAL(clicked()), this, SLOT(RadioCtrl_SaveOpt()));

    connect(ui->radioBtn_DataStream_1, SIGNAL(clicked()), this, SLOT(RadioCtrl_StrOpt()));
    connect(ui->radioBtn_DataStream_2, SIGNAL(clicked()), this, SLOT(RadioCtrl_StrOpt()));
}

void MainWindow::Initialize_MenuBar() {
    QMenu *file = new QMenu(this);

    QAction *action1;
    action1 = new QAction(tr("&1. Training"), this);
    connect(action1, &QAction::triggered, this, &MainWindow::OpenTrainingUserForm);
    file->addAction(action1);
    menuBar()->addMenu(file)->setText(tr("&Dialogs"));

    QAction *action2;
    action2 = new QAction(tr("&2. Prediction"), this);
    file->addAction(action2);
    menuBar()->addMenu(file)->setText(tr("&Dialogs"));

    QAction *action3;
    action3 = new QAction(tr("&3. Ball Control"), this);
    file->addAction(action3);
    menuBar()->addMenu(file)->setText(tr("&Dialogs"));

    QAction *action4;
    action4 = new QAction(tr("&4. (Reserved)"), this);
    menuBar()->addMenu(file)->setText(tr("&Dialogs"));
}

void MainWindow::Initialize_StackedWidget(int mode) {
    ui->stackedWdg->insertWidget(0, &trnForm);           // 1
    ui->stackedWdg->insertWidget(1, &unlabeledDAQForm);  // 2
    ui->stackedWdg->insertWidget(2, &testOneForm);  // 2
    ui->stackedWdg->setCurrentIndex(mode);
}

void MainWindow::Initialize_Figures() {
    Figures_motion = new cv::Mat[N_MOTIONS + 1];
    Figures_motion[0] = cv::imread("../TRT_demo/Figures/Motions/Rest.png", cv::IMREAD_UNCHANGED);
    Figures_motion[1] = cv::imread("../TRT_demo/Figures/Motions/Wrist Flexion.png", cv::IMREAD_UNCHANGED);
    Figures_motion[2] = cv::imread("../TRT_demo/Figures/Motions/Wrist Extension.png", cv::IMREAD_UNCHANGED);
    Figures_motion[3] = cv::imread("../TRT_demo/Figures/Motions/Radial Deviation.png", cv::IMREAD_UNCHANGED);
    Figures_motion[4] = cv::imread("../TRT_demo/Figures/Motions/Ulnar Deviation.png", cv::IMREAD_UNCHANGED);
    Figures_motion[5] = cv::imread("../TRT_demo/Figures/Motions/Wrist Pronation.png", cv::IMREAD_UNCHANGED);
    Figures_motion[6] = cv::imread("../TRT_demo/Figures/Motions/Wrist Supination.png", cv::IMREAD_UNCHANGED);
    Figures_motion[7] = cv::imread("../TRT_demo/Figures/Motions/Hand Close.png", cv::IMREAD_UNCHANGED);
    Figures_motion[8] = cv::imread("../TRT_demo/Figures/Motions/Hand Open.png", cv::IMREAD_UNCHANGED);
}

void MainWindow::Initialize_trnForm() {
    GraphInit_sEMG(trnForm.get_UI()->wdg_RTGraph_MAV);
    GraphInit_IMU(trnForm.get_UI()->wdg_RTGraph_IMU);
    GraphInit_Flex(trnForm.get_UI()->wdg_RTGraph_Flex);
    GraphInit_Label(trnForm.get_UI()->wdg_RTGraph_Label, 0);
}

void MainWindow::Initialize_unlabeledDAQForm() {
    GraphInit_sEMG(unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV);
    GraphInit_IMU(unlabeledDAQForm.get_UI()->wdg_RTGraph_IMU);
    GraphInit_Flex(unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex);
    GraphInit_Label(unlabeledDAQForm.get_UI()->wdg_RTGraph_Label, 1);
}

void MainWindow::Initialize_testOneForm() {
    GraphInit_sEMG(testOneForm.get_UI()->wdg_RTGraph_MAV);
    GraphInit_IMU(testOneForm.get_UI()->wdg_RTGraph_IMU);
    GraphInit_Flex(testOneForm.get_UI()->wdg_RTGraph_Flex);
    GraphInit_Label(testOneForm.get_UI()->wdg_RTGraph_Est, 2);
}

void MainWindow::RealTimeDataPlot() {
//    static QTime time(QTime::currentTime());
//    qsrand(QTime::currentTime().msecsSinceStartOfDay());
//    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    double key = m_time; // time elapsed since start of demo, in seconds
    if (m_radioMode == 0) {
        // 1. sEMG MAV
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(0)->addData(key, sEMG_MAV[0]);
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(1)->addData(key, sEMG_MAV[1]);
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(2)->addData(key, sEMG_MAV[2]);
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(3)->addData(key, sEMG_MAV[3]);
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(4)->addData(key, sEMG_MAV[4]);
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(5)->addData(key, sEMG_MAV[5]);
        trnForm.get_UI()->wdg_RTGraph_MAV->graph(6)->addData(key, sEMG_MAV[6]);
        trnForm.get_UI()->wdg_RTGraph_MAV->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        trnForm.get_UI()->wdg_RTGraph_MAV->replot();

        // 3. Flex sensor
        trnForm.get_UI()->wdg_RTGraph_Flex->graph(0)->addData(key, Flex_processed[0]);
        trnForm.get_UI()->wdg_RTGraph_Flex->graph(1)->addData(key, Flex_processed[1]);
        trnForm.get_UI()->wdg_RTGraph_Flex->graph(2)->addData(key, Flex_processed[2]);
        trnForm.get_UI()->wdg_RTGraph_Flex->graph(3)->addData(key, Flex_processed[3]);
        trnForm.get_UI()->wdg_RTGraph_Flex->graph(4)->addData(key, Flex_processed[4]);
        trnForm.get_UI()->wdg_RTGraph_Flex->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        trnForm.get_UI()->wdg_RTGraph_Flex->replot();

        // 4. Motion label
        trnForm.get_UI()->wdg_RTGraph_Label->graph(0)->addData(key, Motion_label);
        trnForm.get_UI()->wdg_RTGraph_Label->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        trnForm.get_UI()->wdg_RTGraph_Label->replot();
    }
    else if (m_radioMode == 1) {
        // 1. sEMG MAV
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(0)->addData(key, sEMG_MAV[0]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(1)->addData(key, sEMG_MAV[1]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(2)->addData(key, sEMG_MAV[2]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(3)->addData(key, sEMG_MAV[3]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(4)->addData(key, sEMG_MAV[4]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(5)->addData(key, sEMG_MAV[5]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->graph(6)->addData(key, sEMG_MAV[6]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        unlabeledDAQForm.get_UI()->wdg_RTGraph_MAV->replot();

        // 3. Flex sensor
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->graph(0)->addData(key, Flex_processed[0]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->graph(1)->addData(key, Flex_processed[1]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->graph(2)->addData(key, Flex_processed[2]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->graph(3)->addData(key, Flex_processed[3]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->graph(4)->addData(key, Flex_processed[4]);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Flex->replot();

        // 4. Motion label
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Label->graph(0)->addData(key, Motion_label);
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Label->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        unlabeledDAQForm.get_UI()->wdg_RTGraph_Label->replot();
    }
    else if (m_radioMode == 2) {
        // 1. sEMG MAV
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(0)->addData(key, sEMG_MAV[0]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(1)->addData(key, sEMG_MAV[1]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(2)->addData(key, sEMG_MAV[2]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(3)->addData(key, sEMG_MAV[3]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(4)->addData(key, sEMG_MAV[4]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(5)->addData(key, sEMG_MAV[5]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->graph(6)->addData(key, sEMG_MAV[6]);
        testOneForm.get_UI()->wdg_RTGraph_MAV->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        testOneForm.get_UI()->wdg_RTGraph_MAV->replot();

        // 3. Flex sensor
        testOneForm.get_UI()->wdg_RTGraph_Flex->graph(0)->addData(key, Flex_processed[0]);
        testOneForm.get_UI()->wdg_RTGraph_Flex->graph(1)->addData(key, Flex_processed[1]);
        testOneForm.get_UI()->wdg_RTGraph_Flex->graph(2)->addData(key, Flex_processed[2]);
        testOneForm.get_UI()->wdg_RTGraph_Flex->graph(3)->addData(key, Flex_processed[3]);
        testOneForm.get_UI()->wdg_RTGraph_Flex->graph(4)->addData(key, Flex_processed[4]);
        testOneForm.get_UI()->wdg_RTGraph_Flex->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        testOneForm.get_UI()->wdg_RTGraph_Flex->replot();

        // 4. Motion estimation
        testOneForm.get_UI()->wdg_RTGraph_Est->graph(0)->addData(key, Motion_est);
        testOneForm.get_UI()->wdg_RTGraph_Est->xAxis->setRange(key, Width_X, Qt::AlignRight); // Size is set to [s] scale
        testOneForm.get_UI()->wdg_RTGraph_Est->replot();
    }

    key_cnt++;
}

void MainWindow::OnTimerControlTimeCallbackFunc() {
    if (m_radioMode == 0 || m_radioMode == 1)
        ui->lineEdit_Time->setText(QString::number(m_time - m_time_last, 'f', 3)); // Time update
    else if (m_radioMode == 2)
        ui->lineEdit_Time->setText(QString::number(m_time - T_trial_start, 'f', 3)); // Time update
}

void MainWindow::OnTimerRTPlotCallbackFunc() {
    // Real-time Graph
    RealTimeDataPlot();
}

void MainWindow::OnTimerFigCallbackFunc() {
    QImage* qimg_motion;
    QPixmap qpix_motion;

    if (m_radioMode == 0) {
        // (Labeling) Figures - Truth motion
        qimg_motion = new QImage((unsigned char *)Figures_motion[Motion_label].data,
                                  Figures_motion[Motion_label].cols, Figures_motion[Motion_label].rows,
                                  QImage::Format_ARGB32);
        qpix_motion = QPixmap::fromImage(*qimg_motion);

        trnForm.get_UI()->Img_Truth->setPixmap(qpix_motion.scaled(trnForm.get_UI()->Img_Truth->width(),
                                  trnForm.get_UI()->Img_Truth->height(),
                                  Qt::IgnoreAspectRatio));
        delete qimg_motion;

        // (TrainingUserForm) Figures - State indicator
        qimg_motion = new QImage((unsigned char *)Figures_motion[State_indicator].data,
                                  Figures_motion[State_indicator].cols, Figures_motion[State_indicator].rows,
                                  QImage::Format_ARGB32);
        qpix_motion = QPixmap::fromImage(*qimg_motion);

        get_VideoPlayer_Training()->get_m_MotionImg()->setPixmap(qpix_motion.scaled(PIXMAP_WIDTH, PIXMAP_HEIGHT,
                                                                                    Qt::IgnoreAspectRatio));
        delete qimg_motion;
    }
    else if (m_radioMode == 1) {
//        // (Labeling) Figures - Truth motion
//        qimg_motion = new QImage((unsigned char *)Figures_motion[Motion_label].data,
//                                  Figures_motion[Motion_label].cols, Figures_motion[Motion_label].rows,
//                                  QImage::Format_ARGB32);
//        qpix_motion = QPixmap::fromImage(*qimg_motion);

//        unlabeledDAQForm.get_UI()->Img_Truth->setPixmap(qpix_motion.scaled(unlabeledDAQForm.get_UI()->Img_Truth->width(),
//                                  unlabeledDAQForm.get_UI()->Img_Truth->height(),
//                                  Qt::IgnoreAspectRatio));
//        delete qimg_motion;

//        // (UnlabeledDAQUserForm) Figures - State indicator
//        qimg_motion = new QImage((unsigned char *)Figures_motion[State_indicator].data,
//                                  Figures_motion[State_indicator].cols, Figures_motion[State_indicator].rows,
//                                  QImage::Format_ARGB32);
//        qpix_motion = QPixmap::fromImage(*qimg_motion);

//        get_VideoPlayer_UnlabeledDAQ()->get_m_MotionImg()->setPixmap(qpix_motion.scaled(PIXMAP_WIDTH, PIXMAP_HEIGHT,
//                                                                                    Qt::IgnoreAspectRatio));
//        delete qimg_motion;
    }
    else if (m_radioMode == 2) {
        // (Labeling) Figures - Estimation
        if ((m_time - T_trial_start) >= T_READY_BALL_CTR) {
            qimg_motion = new QImage((unsigned char *)Figures_motion[Motion_est].data,
                                      Figures_motion[Motion_est].cols, Figures_motion[Motion_est].rows,
                                      QImage::Format_ARGB32);
            qpix_motion = QPixmap::fromImage(*qimg_motion);

            testOneForm.get_UI()->Img_Est->setPixmap(qpix_motion.scaled(testOneForm.get_UI()->Img_Est->width(),
                                      testOneForm.get_UI()->Img_Est->height(),
                                      Qt::IgnoreAspectRatio));
            delete qimg_motion;
        }
    }
}

void MainWindow::Thread_TwinCAT_func() {
    // Setup for real-time processing clock
    if (hMutex = CreateMutexA(NULL, TRUE, "nMutex")) {
        std::cout << "[USER] Creating Mutex Success" << std::endl;
    }
    else {
        std::cout << "[User] Error: Creating Mutex Failed: "
                  << GetLastError() << std::endl;
    }

    if (hMemory = ::CreateFileMappingA(INVALID_HANDLE_VALUE,
                                       NULL, PAGE_READWRITE,
                                       0, sizeof(Shared_Data), "nMemory")) {
        std::cout << "[USER] Creating Shared Memory Success" << std::endl;
    }
    else {
        std::cout << "[User] Error: Creating Shared Memory Failed: "
                  << GetLastError() << std::endl;
    }

    if (pShared_Data = (Shared_Data*)::MapViewOfFile(hMemory,
                                                     FILE_MAP_ALL_ACCESS,
                                                     0, 0, sizeof(Shared_Data))) {
        std::cout << "[USER] Data Address Returned" << std::endl;
    }
    else {
        std::cout << "[User] Error: Address Return Failed: "
                  << GetLastError() << std::endl;
    }

    pShared_Data->bContinue = FALSE;
    if (m_radioSaveOpt == 0)
        pShared_Data->bSaveImmediate = TRUE;
    else
        pShared_Data->bSaveImmediate = FALSE;
    pShared_Data->bProcessEnd = FALSE;
    pShared_Data->bFirst = TRUE;
    pShared_Data->iNextOwner = THREAD_MAIN;
    pShared_Data->time = 0.0;
    pShared_Data->count = 0;

    // (Important) Memory should be initialized
    ZeroMemory(&startupinfo, sizeof(startupinfo));
    ZeroMemory(&processinfo, sizeof(processinfo));

    if (CreateProcessA("D:/TwinCAT/Debug/TwinCAT.exe",
                      "TwinCAT.exe nMemory nMutex", NULL, NULL, FALSE, 0, NULL, NULL, &startupinfo, &processinfo)) {
        std::cout << "[USER] Creating Process Success" << std::endl;
    }
    else {
        std::cout << "[User] Error: Creating Process Failed: "
                  << GetLastError() << std::endl;
    }

    if (m_radioMode == 2) {
        std::thread Thread_TensorRT(&MainWindow::Thread_TensorRT_func, this);
        Thread_TensorRT.detach();
    }

    pShared_Data->iNextOwner = THREAD_TWINCAT;
    ReleaseMutex(hMutex);

    while (!pShared_Data->bProcessEnd) {
        WaitForSingleObject(hMutex, INFINITE);
        while (pShared_Data->iNextOwner != THREAD_MAIN) {
            ReleaseMutex(hMutex);
            WaitForSingleObject(hMutex, INFINITE);
        }

        if (pShared_Data->bFirst) {
            pShared_Data->bContinue = TRUE;
            pShared_Data->bFirst = FALSE;
            pShared_Data->iNextOwner = THREAD_CALLBACK;
            ReleaseMutex(hMutex);

            if (m_radioStrOpt == 0) { // Streaming mode
                if (m_radioMode == 0)
                    emit playTrainingVideo();
                else if (m_radioMode == 1)
                    emit playUnlabeledDAQVideo();
            }
        }
        else if (pShared_Data->bContinue) {
            ////////////////////////////////////////// DAQ //////////////////////////////////////////
            if (m_radioStrOpt == 0) { // Streaming mode
                std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

                // Data processing - Data acquisition
                DAQ_Online();

                // Data processing - Feature extraction
                Feature_Extraction();

                // Data processing - Offset calculation
                Offset_Calculation();

                // Data processing - Flex sensor angle conversion
                Flex_Angle_Calculation();

                // Data processing - Normalization
                Normalize_Features();

                std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
                Time_elapse_Processing = (double)((double)std::chrono::duration_cast
                                                <std::chrono::nanoseconds>(end - start).count() / (double)1000000.0);

                // Motion label & state indicator assignment
                if (m_radioMode == 0 || m_radioMode == 1) {
                    Motion_label = Get_Motion_Label(m_radioMode, m_time - m_time_last);
                    State_indicator = Get_State_Indicator(m_radioMode, m_time - m_time_last);
                }

                // MAV Amplitude calculation
                Calculate_MAV_Amp();

                // Trajectory assignment
                if (m_radioMode == 0) {
                    if ((m_time_cnt - m_time_last_cnt) % 20 == 0) {
                        // Visual cuess
                        if ((m_time_cnt - m_time_last_cnt) < (int)((T_READY + N_MOTIONS * T_CONTRACT +
                                                (N_MOTIONS - 1) * T_REST + T_RESIDUAL) * FS)) {
                            emit setPalmRotX_Cue_Trn(trj_wrist_X[m_time_cnt - m_time_last_cnt]);
                            emit setLowerArmRotY_Cue_Trn(trj_wrist_Y[m_time_cnt - m_time_last_cnt]);
                            emit setPalmRotZ_Cue_Trn(trj_wrist_Z[m_time_cnt - m_time_last_cnt]);

                            emit setThumbRotZ_Cue_Trn(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setIndexRotZ_Cue_Trn(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setMiddleRotZ_Cue_Trn(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setRingRotZ_Cue_Trn(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setLittleRotZ_Cue_Trn(trj_hand[m_time_cnt - m_time_last_cnt]);
                        }

                        // Motion capture
                        emit setThumbRotZ_MoCap_Trn(Flex_Angle[0]);
                        emit setIndexRotZ_MoCap_Trn(Flex_Angle[1]);
                        emit setMiddleRotZ_MoCap_Trn(Flex_Angle[2]);
                        emit setRingRotZ_MoCap_Trn(Flex_Angle[3]);
                        emit setLittleRotZ_MoCap_Trn(Flex_Angle[4]);
                    }
                }
                else if (m_radioMode == 1) {
                    if ((m_time_cnt - m_time_last_cnt) % 20 == 0) {
                        // Visual cue
                        if ((m_time_cnt - m_time_last_cnt) < (int)((T_READY + N_REPEAT * T_CONTRACT
                                                + (N_REPEAT - 1) * T_REST + T_RESIDUAL) * FS)) {
                            emit setPalmRotX_Cue_UnlabeledDAQ(trj_wrist_X[m_time_cnt - m_time_last_cnt]);
                            emit setLowerArmRotY_Cue_UnlabeledDAQ(trj_wrist_Y[m_time_cnt - m_time_last_cnt]);
                            emit setPalmRotZ_Cue_UnlabeledDAQ(trj_wrist_Z[m_time_cnt - m_time_last_cnt]);

                            emit setThumbRotZ_Cue_UnlabeledDAQ(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setIndexRotZ_Cue_UnlabeledDAQ(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setMiddleRotZ_Cue_UnlabeledDAQ(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setRingRotZ_Cue_UnlabeledDAQ(trj_hand[m_time_cnt - m_time_last_cnt]);
                            emit setLittleRotZ_Cue_UnlabeledDAQ(trj_hand[m_time_cnt - m_time_last_cnt]);
                        }

                        // Motion capture
                        emit setThumbRotZ_MoCap_UnlabeledDAQ(Flex_Angle[0]);
                        emit setIndexRotZ_MoCap_UnlabeledDAQ(Flex_Angle[1]);
                        emit setMiddleRotZ_MoCap_UnlabeledDAQ(Flex_Angle[2]);
                        emit setRingRotZ_MoCap_UnlabeledDAQ(Flex_Angle[3]);
                        emit setLittleRotZ_MoCap_UnlabeledDAQ(Flex_Angle[4]);
                    }

                    // Print the motion order
                    for (int n = 0; n < N_REPEAT; n++) {
                        if ((m_time_cnt - m_time_last_cnt) == (int)((T_READY + (T_CONTRACT + T_REST) * n) * FS) + 1) {
                            std::cout << n + 1 << "th motion complete" << std::endl;
                            break;
                        }
                    }
                }
                else if (m_radioMode == 2) {
                    if ((m_time_cnt - m_time_last_cnt) % 20 == 0) {
                        // Motion capture
                        emit setThumbRotZ_MoCap_TestOne(Flex_Angle[0]);
                        emit setIndexRotZ_MoCap_TestOne(Flex_Angle[1]);
                        emit setMiddleRotZ_MoCap_TestOne(Flex_Angle[2]);
                        emit setRingRotZ_MoCap_TestOne(Flex_Angle[3]);
                        emit setLittleRotZ_MoCap_TestOne(Flex_Angle[4]);
                    }
                }
            }
            else if (m_radioStrOpt == 1) { // Load mode
                std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

                // Data processing - Data acquisition (Features are already calculated)
                DAQ_Offline();

                // Data processing - Offset calculation
                Offset_Calculation();

                // Data processing - Normalization
                Normalize_Features();

                std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
                Time_elapse_Processing = (double)((double)std::chrono::duration_cast
                                                <std::chrono::nanoseconds>(end - start).count() / (double)1000000.0);

                // MAV Amplitude calculation
                Calculate_MAV_Amp();

                //////////////// 3D model rendering should be implemented here ////////////////
            }
            Rest_mean_std_Calculation();

            // Estimation flag (For Prediction & Ball Control)
            if (m_time_cnt % 15 == 0)
                isEstimated = false;

            // Ball Control
            if (m_radioMode == 2) {
                if (PracticeOrNot) {
                    player_TestOne.Get_PaintTestOne()->Set_Traj_idx(0);
                    if (!isRun) {
                        isRun = true;
                        T_trial_start = m_time;
                        player_TestOne.Get_PaintTestOne()->Set_T_Trial_Start(T_trial_start);
                    }
                    else {
                        // Success determination
                        player_TestOne.Get_PaintTestOne()->Set_isSuccess(Determine_Success(0));

                        // Time over determination
                        if ((m_time - T_trial_start) >= (T_READY_BALL_CTR + T_MAX_BALL_CTR_PRACTICE)) {
                            isTimeOver = true;
                            player_TestOne.Get_PaintTestOne()->Set_isTimeOver(isTimeOver);
                        }
                    }
                }
                else {
                    if (!isRun) {
                        isRun = true;
                        T_trial_start = m_time;
                        player_TestOne.Get_PaintTestOne()->Set_T_Trial_Start(T_trial_start);
                    }
                    else {
                        if ((m_time - T_trial_start) < T_READY_BALL_CTR) {
                            player_TestOne.Get_PaintTestOne()->Set_isSuccess(false);
                        }
                        else if (T_READY_BALL_CTR <= (m_time - T_trial_start) &&
                                (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR)) {
                            isTimeOver = false;
                            player_TestOne.Get_PaintTestOne()->Set_isTimeOver(isTimeOver);

                            // Success determination
                            isSuccess = Determine_Success(Traj_idx);
                            player_TestOne.Get_PaintTestOne()->Set_isSuccess(isSuccess);
                            if (isSuccess) {
                                if (!isSuccess_time_flag) {
                                    isSuccess_time_flag = true;
                                    T_trial_end = m_time;
                                    player_TestOne.Get_PaintTestOne()->Set_T_Trial_End(T_trial_end);

                                    SuccessOrNot[Trial_idx - 1][Traj_idx - 1] = true;
                                    BallCtrElapsedTime[Trial_idx - 1][Traj_idx - 1] = T_trial_end - T_trial_start - T_READY_BALL_CTR;
                                    std::cout << "Ball Control Elapsed time : " << BallCtrElapsedTime[Trial_idx - 1][Traj_idx - 1] << std::endl;
                                }
                            }
                        }
                        else if ((m_time - T_trial_start) >= (T_READY_BALL_CTR + T_MAX_BALL_CTR) && !isSuccess) {
                            isTimeOver = true;
                            player_TestOne.Get_PaintTestOne()->Set_isTimeOver(isTimeOver);

                            if (!isTimeOver_time_flag) {
                                isTimeOver_time_flag = true;
                                T_trial_end = m_time;
                                player_TestOne.Get_PaintTestOne()->Set_T_Trial_End(T_trial_end);

                                SuccessOrNot[Trial_idx - 1][Traj_idx - 1] = false;
                                BallCtrElapsedTime[Trial_idx - 1][Traj_idx - 1] = T_MAX_BALL_CTR;
                                std::cout << "Ball Control Elapsed time : " << BallCtrElapsedTime[Trial_idx - 1][Traj_idx - 1] << std::endl;
                            }
                        }
                    }
                }
            }

            ///////////////////////////////////////// Stack /////////////////////////////////////////
            if (m_radioMode == 0 || m_radioMode == 1) {
                if (m_radioSaveOpt != 3 && (m_time_cnt - m_time_last_cnt) > (int)(T_OFFSET_END_TRN * 1000))
                    StackData();
            }
            else if (m_radioMode == 2) {
                if (!PracticeOrNot && !isSuccess && !isTimeOver && (m_time - T_trial_start) >= T_READY_BALL_CTR) {
                    StackData();
                }
            }

            /////////////////////////////////////////////////////////////////////////////////////////
            pShared_Data->iNextOwner = THREAD_CALLBACK;
            ReleaseMutex(hMutex);
        }
    }

    pShared_Data->iNextOwner = THREAD_TWINCAT;
    ReleaseMutex(hMutex);

    WaitForSingleObject(hMutex, INFINITE);
    while (pShared_Data->iNextOwner != THREAD_MAIN) {
        ReleaseMutex(hMutex);
        WaitForSingleObject(hMutex, INFINITE);
    }
}

void MainWindow::Thread_TensorRT_func() {
    /////////////////////////////////////// Inference ///////////////////////////////////////
    cv::Mat image_TRT = cv::Mat::ones(4, N_EMG, CV_32F);
    int X_val_inc = INC_MAX_X;
    int Y_val_inc = INC_MAX_Y;
    double Rot_val_inc = INC_MAX_ROT;
    double Scale_val_inc = INC_MAX_SCALE;

    while (!pShared_Data->bProcessEnd) {
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
        if (pShared_Data->bContinue) {
            // 1. Assign image values
            for (int j = 0; j < N_EMG; j++) {
                image_TRT.at<float>(0, j) = sEMG_MAV_norm[j];
                image_TRT.at<float>(1, j) = sEMG_WL_norm[j];
                image_TRT.at<float>(2, j) = sEMG_SSC_norm[j];
                image_TRT.at<float>(3, j) = sEMG_ZC_norm[j];
            }

            // 2. Inference
            if (!isEstimated) {
                // Stack the sEMG MAV amplitude
                if (!PracticeOrNot && T_READY < (m_time - T_trial_start)
                    && (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR)) {
                    stack_Ball_Ctr_sEMG_MAV_amp[Trial_idx - 1][Traj_idx - 1].push_back(sum_sEMG_MAV);
                }

                if (Rest_Mot_Classification() && T_READY < (m_time - T_trial_start)) { // Motion exerted
                    if (!PracticeOrNot && (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR)) {
                        stack_Ball_Ctr_isMotionExerted[Trial_idx - 1][Traj_idx - 1].push_back(1);
                    }

                    if (m_radioNetwork == 0) {
                        if (TRT_Fine_Tune->infer(image_TRT))
                            Motion_est = TRT_Fine_Tune->get_estimation();
                    }
                    else if (m_radioNetwork == 1) {
                        if (TRT_STM->infer(image_TRT))
                            Motion_est = TRT_STM->get_estimation();
                    }
                    else if (m_radioNetwork == 2) {
                        if (TRT_Proposed->infer(image_TRT))
                            Motion_est = TRT_Proposed->get_estimation();
                    }

                    if (!PracticeOrNot && (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR)) {
                        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
                        Time_elapse_TensorRT = (double)((double)std::chrono::duration_cast
                                                         <std::chrono::nanoseconds>(end - start).count() / (double)1000000.0);
                        stack_Ball_Ctr_Time_elapse_TensorRT[Trial_idx - 1][Traj_idx - 1].push_back(Time_elapse_TensorRT);
                    }

                    if (PracticeOrNot) {
                        if (isRun) {
                            if (T_READY_BALL_CTR <= (m_time - T_trial_start)) {
                                // Move the cursor corresponding to the motion estimation
                                if (Motion_est == 1) {
                                    double X_val = player_TestOne.Get_PaintTestOne()->Get_Cur_X();
                                    if (X_val - X_val_inc >= 0)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_X(X_val - X_val_inc);
                                }
                                else if (Motion_est == 2) {
                                    double X_val = player_TestOne.Get_PaintTestOne()->Get_Cur_X();
                                    if (X_val + X_val_inc <= 1150)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_X(X_val + X_val_inc);
                                }
                                else if (Motion_est == 3) {
                                    double Y_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Y();
                                    if (Y_val - Y_val_inc >= 0)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_Y(Y_val - Y_val_inc);
                                }
                                else if (Motion_est == 4) {
                                    double Y_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Y();
                                    if (Y_val + Y_val_inc <= SCREEN_HEIGHT)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_Y(Y_val + Y_val_inc);
                                }
                                else if (Motion_est == 5) {
                                    double Rot_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Rot();
                                    if (Rot_val + Rot_val_inc <= 150)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(Rot_val + Rot_val_inc);
                                }
                                else if (Motion_est == 6) {
                                    double Rot_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Rot();
                                    if (Rot_val - Rot_val_inc >= -150)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(Rot_val - Rot_val_inc);
                                }
                                else if (Motion_est == 7) {
                                    double Scale_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Scale();
                                    if (Scale_val - Scale_val_inc >= 0.3)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(Scale_val - Scale_val_inc);
                                }
                                else if (Motion_est == 8) {
                                    double Scale_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Scale();
                                    if (Scale_val + Scale_val_inc <= 2.5)
                                        player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(Scale_val + Scale_val_inc);
                                }
                            }
                        }
                    }
                    else {
                        if (isRun) {
                            if (T_READY_BALL_CTR <= (m_time - T_trial_start) &&
                                    (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR)) {
                                if (!isSuccess) {
                                    // Move the cursor corresponding to the motion estimation
                                    if (Motion_est == 1) {
                                        double X_val = player_TestOne.Get_PaintTestOne()->Get_Cur_X();
                                        if (X_val - X_val_inc >= 0)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_X(X_val - X_val_inc);
                                    }
                                    else if (Motion_est == 2) {
                                        double X_val = player_TestOne.Get_PaintTestOne()->Get_Cur_X();
                                        if (X_val + X_val_inc <= 1150)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_X(X_val + X_val_inc);
                                    }
                                    else if (Motion_est == 3) {
                                        double Y_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Y();
                                        if (Y_val - Y_val_inc >= 0)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_Y(Y_val - Y_val_inc);
                                    }
                                    else if (Motion_est == 4) {
                                        double Y_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Y();
                                        if (Y_val + Y_val_inc <= SCREEN_HEIGHT)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_Y(Y_val + Y_val_inc);
                                    }
                                    else if (Motion_est == 5) {
                                        double Rot_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Rot();
                                        if (Rot_val + Rot_val_inc <= 150)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(Rot_val + Rot_val_inc);
                                    }
                                    else if (Motion_est == 6) {
                                        double Rot_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Rot();
                                        if (Rot_val - Rot_val_inc >= -150)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(Rot_val - Rot_val_inc);
                                    }
                                    else if (Motion_est == 7) {
                                        double Scale_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Scale();
                                        if (Scale_val - Scale_val_inc >= 0.3)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(Scale_val - Scale_val_inc);
                                    }
                                    else if (Motion_est == 8) {
                                        double Scale_val = player_TestOne.Get_PaintTestOne()->Get_Cur_Scale();
                                        if (Scale_val + Scale_val_inc <= 2.5)
                                            player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(Scale_val + Scale_val_inc);
                                    }
                                }
                            }
                        }
                    }
                }
                else { // Rest state
                    if (!PracticeOrNot && T_READY < (m_time - T_trial_start)
                        && (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR))
                        stack_Ball_Ctr_isMotionExerted[Trial_idx - 1][Traj_idx - 1].push_back(0);
                    Motion_est = 0;
                }

                isEstimated = true;
                if (!PracticeOrNot && T_READY < (m_time - T_trial_start)
                    && (m_time - T_trial_start) < (T_READY_BALL_CTR + T_MAX_BALL_CTR))
                    stack_Ball_Ctr_Motion_est_TensorRT_thread[Trial_idx - 1][Traj_idx - 1].push_back(Motion_est);
            }
        }
    }
}

void MainWindow::fillPortsInfo() {
    portName = QString("No Port");
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        if (info.manufacturer() == QString("Silicon Labs")) {
            portName = info.portName();
            break;
        }
    }
}

/////////////////////////////////// sEMG feature extraction ///////////////////////////////////
void MainWindow::Calculate_MAV() {
    double sEMG_MAV_tmp = 0.0;
    if (m_radioMode == 0 || m_radioMode == 1) {
        if (stack_sEMG_raw[0].size() <= Win_size) {
            for (int i = 0; i < N_EMG; i++) {
                sEMG_MAV_tmp = 0.0;
                for (int j = 0; j < stack_sEMG_raw[0].size(); j++)
                    sEMG_MAV_tmp += abs(stack_sEMG_raw[i][j]);

                sEMG_MAV[i] = sEMG_MAV_tmp / (double)Win_size;
            }
        }
        else {
            int idx_size = stack_sEMG_raw[0].size();
            for (int i = 0; i < N_EMG; i++) {
                sEMG_MAV_tmp = 0.0;
                for (int j = idx_size - Win_size; j < idx_size; j++)
                    sEMG_MAV_tmp += abs(stack_sEMG_raw[i][j]);

                sEMG_MAV[i] = sEMG_MAV_tmp / (double)Win_size;
            }
        }
    }
    else if (m_radioMode == 2) {
        if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() <= Win_size) {
            for (int i = 0; i < N_EMG; i++) {
                sEMG_MAV_tmp = 0.0;
                for (int j = 0; j < stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size(); j++)
                    sEMG_MAV_tmp += abs(stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][j]);

                sEMG_MAV[i] = sEMG_MAV_tmp / (double)Win_size;
            }
        }
        else {
            int idx_size = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size();
            for (int i = 0; i < N_EMG; i++) {
                sEMG_MAV_tmp = 0.0;
                for (int j = idx_size - Win_size; j < idx_size; j++)
                    sEMG_MAV_tmp += abs(stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][j]);

                sEMG_MAV[i] = sEMG_MAV_tmp / (double)Win_size;
            }
        }
    }
}

void MainWindow::Calculate_WL() {
    double sEMG_WL_tmp = 0.0;
    if (m_radioMode == 0 || m_radioMode == 1) {
        if (stack_sEMG_raw[0].size() == 1) {
            for (int i = 0; i < N_EMG; i++)
                sEMG_WL[i] = 0.0;
        }
        else if (1 < stack_sEMG_raw[0].size() && stack_sEMG_raw[0].size() <= Win_size) {
            int idx_size = stack_sEMG_raw[0].size();
            for (int i = 0; i < N_EMG; i++) {
                sEMG_WL_tmp = 0.0;
                for (int j = 1; j < idx_size; j++) {
                    sEMG_WL_tmp += abs(stack_sEMG_raw[i][j] - stack_sEMG_raw[i][j - 1]);
                }

                sEMG_WL[i] = sEMG_WL_tmp;
            }
        }
        else {
            int idx_size = stack_sEMG_raw[0].size();
            for (int i = 0; i < N_EMG; i++) {
                sEMG_WL_tmp = 0.0;
                for (int j = idx_size - Win_size + 1; j < idx_size; j++)
                    sEMG_WL_tmp += abs(stack_sEMG_raw[i][j] - stack_sEMG_raw[i][j - 1]);

                sEMG_WL[i] = sEMG_WL_tmp;
            }
        }
    }
    else if (m_radioMode == 2) {
        if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() == 1) {
            for (int i = 0; i < N_EMG; i++)
                sEMG_WL[i] = 0.0;
        }
        else if (1 < stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() &&
                 stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() <= Win_size) {
            int idx_size = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size();
            for (int i = 0; i < N_EMG; i++) {
                sEMG_WL_tmp = 0.0;
                for (int j = 1; j < idx_size; j++) {
                    sEMG_WL_tmp += abs(stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][j] -
                                       stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][j - 1]);
                }
                sEMG_WL[i] = sEMG_WL_tmp;
            }
        }
        else {
            int idx_size = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size();
            for (int i = 0; i < N_EMG; i++) {
                sEMG_WL_tmp = 0.0;
                for (int j = idx_size - Win_size + 1; j < idx_size; j++) {
                    sEMG_WL_tmp += abs(stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][j] -
                                       stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][j - 1]);
                }
                sEMG_WL[i] = sEMG_WL_tmp;
            }
        }
    }
}

void MainWindow::Calculate_SSC() {
    double sEMG_Slope_1 = 0.0;
    double sEMG_Slope_2 = 0.0;

    if (m_radioMode == 0 || m_radioMode == 1) {
        if (stack_sEMG_raw[0].size() == 1 || stack_sEMG_raw[0].size() == 2) {
            for (int i = 0; i < N_EMG; i++)
                sEMG_SSC[i] = 0.0;
        }
        else if (2 < stack_sEMG_raw[0].size() && stack_sEMG_raw[0].size() <= Win_size) {
            int idx_cur =  stack_sEMG_raw[0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                sEMG_Slope_1 = stack_sEMG_raw[i][idx_cur] - stack_sEMG_raw[i][idx_cur - 1];
                sEMG_Slope_2 = stack_sEMG_raw[i][idx_cur - 1] - stack_sEMG_raw[i][idx_cur - 2];
                if (sEMG_Slope_1 * sEMG_Slope_2 < 0)
                    sEMG_SSC[i] += 1;
            }
        }
        else {
            int idx_cur = stack_sEMG_raw[0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                sEMG_Slope_1 = stack_sEMG_raw[i][idx_cur - Win_size + 2] - stack_sEMG_raw[i][idx_cur - Win_size + 1];
                sEMG_Slope_2 = stack_sEMG_raw[i][idx_cur - Win_size + 1] - stack_sEMG_raw[i][idx_cur - Win_size];
                if (sEMG_Slope_1 * sEMG_Slope_2 < 0)
                    if (sEMG_SSC[i] > 0)
                        sEMG_SSC[i] -= 1;

                sEMG_Slope_1 = stack_sEMG_raw[i][idx_cur] - stack_sEMG_raw[i][idx_cur - 1];
                sEMG_Slope_2 = stack_sEMG_raw[i][idx_cur - 1] - stack_sEMG_raw[i][idx_cur - 2];
                if (sEMG_Slope_1 * sEMG_Slope_2 < 0)
                    sEMG_SSC[i] += 1;
            }
        }
    }
    else if (m_radioMode == 2) {
        if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() == 1 ||
            stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() == 2) {
            for (int i = 0; i < N_EMG; i++)
                sEMG_SSC[i] = 0.0;
        }
        else if (2 < stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() &&
                 stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() <= Win_size) {
            int idx_cur =  stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                sEMG_Slope_1 = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur] -
                               stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 1];
                sEMG_Slope_2 = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 1] -
                               stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 2];
                if (sEMG_Slope_1 * sEMG_Slope_2 < 0)
                    sEMG_SSC[i] += 1;
            }
        }
        else {
            int idx_cur = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                sEMG_Slope_1 = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - Win_size + 2] -
                               stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - Win_size + 1];
                sEMG_Slope_2 = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - Win_size + 1] -
                               stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - Win_size];
                if (sEMG_Slope_1 * sEMG_Slope_2 < 0)
                    if (sEMG_SSC[i] > 0)
                        sEMG_SSC[i] -= 1;

                sEMG_Slope_1 = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur] -
                               stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 1];
                sEMG_Slope_2 = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 1] -
                               stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 2];
                if (sEMG_Slope_1 * sEMG_Slope_2 < 0)
                    sEMG_SSC[i] += 1;
            }
        }
    }
}

void MainWindow::Calculate_ZC() {
    if (m_radioMode == 0 || m_radioMode == 1) {
        if (stack_sEMG_raw[0].size() == 1) {
            for (int i = 0; i < N_EMG; i++)
                sEMG_ZC[i] = 0.0;
        }
        else if (1 < stack_sEMG_raw[0].size() && stack_sEMG_raw[0].size() <= Win_size) {
            int idx_cur = stack_sEMG_raw[0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                if (stack_sEMG_raw[i][idx_cur] * stack_sEMG_raw[i][idx_cur - 1] < 0)
                    sEMG_ZC[i] += 1;
            }
        }
        else {
            int idx_cur = stack_sEMG_raw[0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                if (stack_sEMG_raw[i][idx_cur - Win_size + 1] * stack_sEMG_raw[i][idx_cur - Win_size] < 0)
                    if (sEMG_ZC[i] > 0)
                        sEMG_ZC[i] -= 1;

                if (stack_sEMG_raw[i][idx_cur] * stack_sEMG_raw[i][idx_cur - 1] < 0)
                    sEMG_ZC[i] += 1;
            }
        }
    }
    else if (m_radioMode == 2) {
        if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() == 1) {
            for (int i = 0; i < N_EMG; i++)
                sEMG_ZC[i] = 0.0;
        }
        else if (1 < stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() &&
                 stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() <= Win_size) {
            int idx_cur = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur] *
                        stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 1] < 0)
                    sEMG_ZC[i] += 1;
            }
        }
        else {
            int idx_cur = stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][0].size() - 1;
            for (int i = 0; i < N_EMG; i++) {
                if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - Win_size + 1] *
                        stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - Win_size] < 0)
                    if (sEMG_ZC[i] > 0)
                        sEMG_ZC[i] -= 1;

                if (stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur] *
                        stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i][idx_cur - 1] < 0)
                    sEMG_ZC[i] += 1;
            }
        }
    }
}

void MainWindow::Normalize_MAV() {
    for (int i = 0; i < N_EMG; i++) {
        sEMG_MAV_norm[i] = sEMG_MAV[i] / MAV_MAX_VAL;
        if (sEMG_MAV_norm[i] >= MAV_MAX_VAL)
            sEMG_MAV_norm[i] = MAV_MAX_VAL;
    }
}

void MainWindow::Normalize_WL() {
    for (int i = 0; i < N_EMG; i++) {
        sEMG_WL_norm[i] = sEMG_WL[i] / WL_MAX_VAL;
        if (sEMG_WL_norm[i] >= WL_MAX_VAL)
            sEMG_WL_norm[i] = WL_MAX_VAL;
    }
}

void MainWindow::Normalize_SSC() {
    for (int i = 0; i < N_EMG; i++)
        sEMG_SSC_norm[i] = sEMG_SSC[i] / (double)Win_size;
}

void MainWindow::Normalize_ZC() {
    for (int i = 0; i < N_EMG; i++)
        sEMG_ZC_norm[i] = sEMG_ZC[i] / (double)Win_size;
}

void MainWindow::Calculate_MAV_Amp() {
    sum_sEMG_MAV = 0.0;
    for (int i = 0; i < N_EMG; i++)
        sum_sEMG_MAV += sEMG_MAV[i];
}

//////////////////////////////////////////// DAQ ////////////////////////////////////////////
void MainWindow::DAQ_Online() {
    m_time = pShared_Data->time;
    m_time_cnt = pShared_Data->count;
    player_TestOne.Get_PaintTestOne()->Set_time(m_time);

    AI_sEMG->ReadOneStep();
    sEMG_raw = AI_sEMG->Get_m_ReadValue();
    for (int i = 0; i < N_EMG; i++) {
        if (m_radioMode == 0 || m_radioMode == 1)
            stack_sEMG_raw[i].push_back(sEMG_raw[sEMG_CH[i]]);
        else if (m_radioMode == 2)
            stack_Ball_Ctr_sEMG_raw[Trial_idx - 1][Traj_idx - 1][i].push_back(sEMG_raw[sEMG_CH[i]]);
    }

    AI_Flex->ReadOneStep();
    Flex_raw = AI_Flex->Get_m_ReadValue();
}

void MainWindow::DAQ_Offline() {
    m_time = pShared_Data->time;
    m_time_cnt = pShared_Data->count;
    player_TestOne.Get_PaintTestOne()->Set_time(m_time);

    if (m_time_cnt <= stack_sEMG_MAV_load[0].size()) {
        Motion_label = stack_Motion_label_load[m_time_cnt - 1];
        for (int i = 0; i < N_EMG; i++) {
            sEMG_MAV[i] = stack_sEMG_MAV_load[i][m_time_cnt - 1];
            sEMG_WL[i] = stack_sEMG_WL_load[i][m_time_cnt - 1];
            sEMG_SSC[i] = stack_sEMG_SSC_load[i][m_time_cnt - 1];
            sEMG_ZC[i] = stack_sEMG_ZC_load[i][m_time_cnt - 1];
        }
    }
}

void MainWindow::Feature_Extraction() {
    Calculate_MAV();
    Calculate_WL();
    Calculate_SSC();
    Calculate_ZC();
}

void MainWindow::Normalize_Features() {
    Normalize_MAV();
    Normalize_WL();
    Normalize_SSC();
    Normalize_ZC();
}

void MainWindow::Offset_Calculation() {
    // Offset Calculation
    if (int(T_OFFSET_START_TRN * 1000) <= (m_time_cnt - m_time_last_cnt) &&
            (m_time_cnt - m_time_last_cnt) < int(T_OFFSET_END_TRN * 1000)) {
        for (int i = 0 ; i < N_EMG; i++) {
            sEMG_MAV_offset[i] += sEMG_MAV[i];
            sEMG_WL_offset[i] += sEMG_WL[i];
        }

        for (int i = 0 ; i < N_FLEX_CH; i++) {
            Flex_raw_offset[i] += Flex_raw[i];
        }

        if ((m_time_cnt - m_time_last_cnt) == int(T_OFFSET_END_TRN * 1000) - 1) {
            for (int i = 0 ; i < N_EMG; i++) {
                sEMG_MAV_offset[i] /= (double)(int(T_OFFSET_END_TRN * 1000) -
                                                int(T_OFFSET_START_TRN * 1000));
                sEMG_WL_offset[i] /= (double)(int(T_OFFSET_END_TRN * 1000) -
                                                int(T_OFFSET_START_TRN * 1000));
            }

            for (int i = 0 ; i < N_FLEX_CH; i++) {
                Flex_raw_offset[i] /= (double)(int(T_OFFSET_END_TRN * 1000) -
                                               int(T_OFFSET_START_TRN * 1000));
            }
        }
    }

    // Offset Application
    if (int(T_OFFSET_END_TRN * 1000) <= (m_time_cnt - m_time_last_cnt)) {
        for (int i = 0; i < N_EMG; i++) {
            sEMG_MAV[i] = std::abs(sEMG_MAV[i] - sEMG_MAV_offset[i]);
            sEMG_WL[i] = std::abs(sEMG_WL[i] - sEMG_WL_offset[i]);
        }

        for (int i = 0; i < N_FLEX_CH; i++) {
            Flex_processed[i] = Flex_raw[i] - Flex_raw_offset[i];
        }
    }
}

void MainWindow::Rest_mean_std_Calculation() {
    if (m_radioMode == 0 || m_radioMode == 1) {
        if (int(T_REST_START_TRN * 1000) <= (m_time_cnt - m_time_last_cnt) &&
                (m_time_cnt - m_time_last_cnt) < int(T_REST_END_TRN * 1000)) {
            for (int i = 0 ; i < N_EMG; i++) {
                sEMG_MAV_rest_mean[i] += sEMG_MAV[i];
                sEMG_MAV_rest_square_mean[i] += std::pow(sEMG_MAV[i], 2.0);
                sEMG_WL_rest_mean[i] += sEMG_WL[i];
                sEMG_WL_rest_square_mean[i] += std::pow(sEMG_WL[i], 2.0);
            }

            if ((m_time_cnt - m_time_last_cnt) == int(T_REST_END_TRN * 1000) - 1) {
                for (int i = 0 ; i < N_EMG; i++) {
                    sEMG_MAV_rest_mean[i] /= (double)(int(T_REST_END_TRN * 1000) -
                                                      int(T_REST_START_TRN * 1000));
                    sEMG_MAV_rest_square_mean[i] /= (double)(int(T_REST_END_TRN * 1000) -
                                                             int(T_REST_START_TRN * 1000));
                    sEMG_MAV_rest_std[i] = std::sqrt(sEMG_MAV_rest_square_mean[i] -
                                                     std::pow(sEMG_MAV_rest_mean[i], 2.0));

                    sum_sEMG_MAV_rest_mean += sEMG_MAV_rest_mean[i];
                    sum_sEMG_MAV_rest_std += sEMG_MAV_rest_std[i];

                    sEMG_WL_rest_mean[i] /= (double)(int(T_REST_END_TRN * 1000) -
                                                     int(T_REST_START_TRN * 1000));
                    sEMG_WL_rest_square_mean[i] /= (double)(int(T_REST_END_TRN * 1000) -
                                                            int(T_REST_START_TRN * 1000));
                    sEMG_WL_rest_std[i] = std::sqrt(sEMG_WL_rest_square_mean[i] -
                                                    std::pow(sEMG_WL_rest_mean[i], 2.0));

                    sum_sEMG_WL_rest_mean += sEMG_WL_rest_mean[i];
                    sum_sEMG_WL_rest_std += sEMG_WL_rest_std[i];
                }

                rest_thres = sum_sEMG_MAV_rest_mean + STD_THRES_SCALE * sum_sEMG_MAV_rest_std;
            }
        }
    }
    else if (m_radioMode == 2) {
        if (int(T_REST_START_BALL_CTR * 1000) <= (m_time_cnt - int(T_trial_start * FS)) &&
            (m_time_cnt - int(T_trial_start * FS)) < int(T_REST_END_BALL_CTR * FS)) {
            for (int i = 0 ; i < N_EMG; i++) {
                sEMG_MAV_rest_mean[i] += sEMG_MAV[i];
                sEMG_MAV_rest_square_mean[i] += std::pow(sEMG_MAV[i], 2.0);
                sEMG_WL_rest_mean[i] += sEMG_WL[i];
                sEMG_WL_rest_square_mean[i] += std::pow(sEMG_WL[i], 2.0);
            }

            if ((m_time_cnt - int(T_trial_start * FS)) == int(T_REST_END_BALL_CTR * 1000) - 1) {
                for (int i = 0 ; i < N_EMG; i++) {
                    sEMG_MAV_rest_mean[i] /= (double)(int(T_REST_END_BALL_CTR * 1000) -
                                                       int(T_REST_START_BALL_CTR * 1000));
                    sEMG_MAV_rest_square_mean[i] /= (double)(int(T_REST_END_BALL_CTR * 1000) -
                                                              int(T_REST_START_BALL_CTR * 1000));
                    sEMG_MAV_rest_std[i] = std::sqrt(sEMG_MAV_rest_square_mean[i] -
                                                     std::pow(sEMG_MAV_rest_mean[i], 2.0));

                    sum_sEMG_MAV_rest_mean += sEMG_MAV_rest_mean[i];
                    sum_sEMG_MAV_rest_std += sEMG_MAV_rest_std[i];

                    sEMG_WL_rest_mean[i] /= (double)(int(T_REST_END_BALL_CTR * 1000) -
                                                      int(T_REST_START_BALL_CTR * 1000));
                    sEMG_WL_rest_square_mean[i] /= (double)(int(T_REST_END_BALL_CTR * 1000) -
                                                             int(T_REST_START_BALL_CTR * 1000));
                    sEMG_WL_rest_std[i] = std::sqrt(sEMG_WL_rest_square_mean[i] -
                                                    std::pow(sEMG_WL_rest_mean[i], 2.0));

                    sum_sEMG_WL_rest_mean += sEMG_WL_rest_mean[i];
                    sum_sEMG_WL_rest_std += sEMG_WL_rest_std[i];
                }

                rest_thres = sum_sEMG_MAV_rest_mean + STD_THRES_SCALE * sum_sEMG_MAV_rest_std;
            }
        }
    }
}

void MainWindow::Flex_Angle_Calculation() {
    if (int(T_OFFSET_END_TRN * 1000) <= (m_time_cnt - m_time_last_cnt)) {
        double thumb_amp = 0.8;
        double index_amp = 1.0;
        double middle_amp = 1.2;
        double ring_amp = 1.0;
        double little_amp = 1.2;

        Flex_Angle[0] = Flex_processed[0] / thumb_amp * 70.0 - 10.0;
        Flex_Angle[1] = Flex_processed[1] / index_amp * 80.0 - 10.0;
        Flex_Angle[2] = Flex_processed[2] / middle_amp * 80.0 - 10.0;
        Flex_Angle[3] = Flex_processed[3] / ring_amp * 80.0 - 10.0;
        Flex_Angle[4] = Flex_processed[4] / little_amp * 80.0 - 10.0;
    }
}

bool MainWindow::Rest_Mot_Classification() {
    if (sum_sEMG_MAV >= rest_thres) {
        return true;
    }
    else {
        return false;
    }
}

bool MainWindow::Determine_Success(int traj_idx) {
    double End_X, End_Y, End_Rot, End_Scale;
    double cur_X, cur_Y, cur_Rot, cur_Scale;
    if (traj_idx == 0) {
        End_X = player_TestOne.Get_PaintTestOne()->Get_End_X_Practice();
        End_Y = player_TestOne.Get_PaintTestOne()->Get_End_Y_Practice();
        End_Rot = player_TestOne.Get_PaintTestOne()->Get_End_Rot_Practice();
        End_Scale = player_TestOne.Get_PaintTestOne()->Get_End_Scale_Practice();
    }
    else {
        End_X = player_TestOne.Get_PaintTestOne()->Get_End_X(traj_idx);
        End_Y = player_TestOne.Get_PaintTestOne()->Get_End_Y(traj_idx);
        End_Rot = player_TestOne.Get_PaintTestOne()->Get_End_Rot(traj_idx);
        End_Scale = player_TestOne.Get_PaintTestOne()->Get_End_Scale(traj_idx);
    }
    cur_X = player_TestOne.Get_PaintTestOne()->Get_Cur_X();
    cur_Y = player_TestOne.Get_PaintTestOne()->Get_Cur_Y();
    cur_Rot = player_TestOne.Get_PaintTestOne()->Get_Cur_Rot();
    cur_Scale = player_TestOne.Get_PaintTestOne()->Get_Cur_Scale();

    if ((std::fabs(cur_X - End_X) <= THRES_POS) &&
        (std::fabs(cur_Y - End_Y) <= THRES_POS) &&
        (std::fabs(cur_Rot - End_Rot) <= THRES_ANG) &&
        (std::fabs(cur_Scale - End_Scale) <= THRES_SCALE))
        return true;
    else
        return false;
}

void MainWindow::StackData() {
    if (m_radioMode == 0 || m_radioMode == 1) {
        // 1. sEMG variables
        for (int i = 0; i < N_EMG; i++) {
            stack_sEMG_MAV[i].push_back(sEMG_MAV[i]);
            stack_sEMG_WL[i].push_back(sEMG_WL[i]);
            stack_sEMG_SSC[i].push_back(sEMG_SSC[i]);
            stack_sEMG_ZC[i].push_back(sEMG_ZC[i]);
        }

        // 3. Flex sensor variables
        for (int i = 0; i < N_FLEX_CH; i++) {
            stack_Flex_raw[i].push_back(Flex_raw[i]);
            stack_Flex_processed[i].push_back(Flex_processed[i]);
            stack_Flex_Angle[i].push_back(Flex_Angle[i]);
        }

        // 4. Motion label
        stack_Motion_label.push_back(Motion_label);

        // 5. Time elapse for processing
        stack_Time_elapse_Processing.push_back(Time_elapse_Processing);
    }
    else if (m_radioMode == 2) {
        // 1. sEMG variables
        for (int i = 0; i < N_EMG; i++) {
            stack_Ball_Ctr_sEMG_MAV[Trial_idx - 1][Traj_idx - 1][i].push_back(sEMG_MAV[i]);
            stack_Ball_Ctr_sEMG_WL[Trial_idx - 1][Traj_idx - 1][i].push_back(sEMG_WL[i]);
            stack_Ball_Ctr_sEMG_SSC[Trial_idx - 1][Traj_idx - 1][i].push_back(sEMG_SSC[i]);
            stack_Ball_Ctr_sEMG_ZC[Trial_idx - 1][Traj_idx - 1][i].push_back(sEMG_ZC[i]);
        }

        // 3. Flex sensor variables
        for (int i = 0; i < N_FLEX_CH; i++) {
            stack_Ball_Ctr_Flex_raw[Trial_idx - 1][Traj_idx - 1][i].push_back(Flex_raw[i]);
            stack_Ball_Ctr_Flex_processed[Trial_idx - 1][Traj_idx - 1][i].push_back(Flex_processed[i]);
            stack_Ball_Ctr_Flex_Angle[Trial_idx - 1][Traj_idx - 1][i].push_back(Flex_Angle[i]);
        }

        // 4. Ball control histories
        stack_Ball_Ctr_X[Trial_idx - 1][Traj_idx - 1].
                push_back(player_TestOne.Get_PaintTestOne()->Get_Cur_X());
        stack_Ball_Ctr_Y[Trial_idx - 1][Traj_idx - 1].
                push_back(player_TestOne.Get_PaintTestOne()->Get_Cur_Y());
        stack_Ball_Ctr_Rot[Trial_idx - 1][Traj_idx - 1].
                push_back(player_TestOne.Get_PaintTestOne()->Get_Cur_Rot());
        stack_Ball_Ctr_Scale[Trial_idx - 1][Traj_idx - 1].
                push_back(player_TestOne.Get_PaintTestOne()->Get_Cur_Scale());

        // 5. Time elapse for processing
        stack_Ball_Ctr_Time_elapse_Processing[Trial_idx - 1][Traj_idx - 1].push_back(Time_elapse_Processing);

        // 6. Motion estimation
        stack_Ball_Ctr_Motion_est[Trial_idx - 1][Traj_idx - 1].push_back(Motion_est);
    }
}

void MainWindow::PopUpData(unsigned int N_delete) {
    for (int time_idx = 0; time_idx < N_delete; time_idx++) {
        for (int i = 0; i < N_EMG; i++) {
            stack_sEMG_raw[i].pop_back();
            stack_sEMG_MAV[i].pop_back();
            stack_sEMG_WL[i].pop_back();
            stack_sEMG_SSC[i].pop_back();
            stack_sEMG_ZC[i].pop_back();
        }

        for (int i = 0; i < N_FLEX_CH; i++) {
            stack_Flex_raw[i].pop_back();
            stack_Flex_processed[i].pop_back();
            stack_Flex_Angle[i].pop_back();
        }

        stack_Motion_label.pop_back();
        stack_Time_elapse_Processing.pop_back();
    }
}

void MainWindow::SaveData_Training() {
    std::cout << "Save Start!" << std::endl;

    // 0. Make Save Folder
    if (!QDir(SaveFolderStr).exists())
        QDir().mkdir(SaveFolderStr);

    // 1. sEMG raw.txt
    QString filename = "/sEMG_raw.txt";
    QFile file_sEMG_raw(SaveFolderStr + filename);
    file_sEMG_raw.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_raw);
    for (int i = 0; i < stack_sEMG_raw[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_raw[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "1. sEMG raw save complete" << std::endl;

    // 2. sEMG MAV.txt
    filename = "/sEMG_MAV.txt";
    QFile file_sEMG_MAV(SaveFolderStr + filename);
    file_sEMG_MAV.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_MAV);
    for (int i = 0; i < stack_sEMG_MAV[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_MAV[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "2. sEMG MAV save complete" << std::endl;

    // 3. sEMG WL.txt
    filename = "/sEMG_WL.txt";
    QFile file_sEMG_WL(SaveFolderStr + filename);
    file_sEMG_WL.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_WL);
    for (int i = 0; i < stack_sEMG_WL[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_WL[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "3. sEMG WL save complete" << std::endl;

    // 4. sEMG SSC.txt
    filename = "/sEMG_SSC.txt";
    QFile file_sEMG_SSC(SaveFolderStr + filename);
    file_sEMG_SSC.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_SSC);
    for (int i = 0; i < stack_sEMG_SSC[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_SSC[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "4. sEMG SSC save complete" << std::endl;

    // 5. sEMG ZC.txt
    filename = "/sEMG_ZC.txt";
    QFile file_sEMG_ZC(SaveFolderStr + filename);
    file_sEMG_ZC.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_ZC);
    for (int i = 0; i < stack_sEMG_ZC[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_ZC[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "5. sEMG ZC save complete" << std::endl;

    // 10. Raw flex sensor.txt
    filename = "/Flex_raw.txt";
    QFile file_Flex_raw(SaveFolderStr + filename);
    file_Flex_raw.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Flex_raw);
    for (int i = 0; i < stack_Flex_raw[0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Flex_raw[j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "10. Raw flex sensor save complete" << std::endl;

    // 11. Offset-processed flex sensor.txt
    filename = "/Flex_processed.txt";
    QFile file_Flex_processed(SaveFolderStr + filename);
    file_Flex_processed.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Flex_processed);
    for (int i = 0; i < stack_Flex_raw[0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Flex_processed[j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "11. Processed flex sensor save complete" << std::endl;

    // 12. Angle flex sensor.txt
    filename = "/Flex_angle.txt";
    QFile file_Flex_angle(SaveFolderStr + filename);
    file_Flex_angle.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Flex_angle);
    for (int i = 0; i < stack_Flex_Angle[0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Flex_Angle[j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "12. Angle flex sensor save complete" << std::endl;

    // 13. Motion label.txt
    filename = "/Motion_label.txt";
    QFile file_Motion_label(SaveFolderStr + filename);
    file_Motion_label.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Motion_label);
    for (int i = 0; i < stack_Motion_label.size(); i++) {
        SaveOut << QString::number(stack_Motion_label[i]);
        SaveOut << QString("\n");
    }
    std::cout << "13. Motion label save complete" << std::endl;

    // 14. Preprocessing time elapse
    filename = "/Time_elapse_Processing.txt";
    QFile file_Time_elapse_processing(SaveFolderStr + filename);
    file_Time_elapse_processing.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Time_elapse_processing);
    for (int i = 0; i < stack_Time_elapse_Processing.size(); i++) {
        SaveOut << QString::number(stack_Time_elapse_Processing[i]);
        SaveOut << QString("\n");
    }
    std::cout << "14. Time elapse for TwinCAT save complete" << std::endl;

    std::cout << "################ save complete ################" << std::endl;
}

void MainWindow::SaveData_UnlabeledDAQ() {
    std::cout << "Save Start!" << std::endl;

    // 0. Make Save Folder
    if (!QDir(SaveFolderStr).exists())
        QDir().mkdir(SaveFolderStr);

    // 1. sEMG raw.txt
    QString filename = "/sEMG_raw.txt";
    QFile file_sEMG_raw(SaveFolderStr + filename);
    file_sEMG_raw.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_raw);
    for (int i = 0; i < stack_sEMG_raw[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_raw[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "1. sEMG raw save complete" << std::endl;

    // 2. sEMG MAV.txt
    filename = "/sEMG_MAV.txt";
    QFile file_sEMG_MAV(SaveFolderStr + filename);
    file_sEMG_MAV.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_MAV);
    for (int i = 0; i < stack_sEMG_MAV[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_MAV[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "2. sEMG MAV save complete" << std::endl;

    // 3. sEMG WL.txt
    filename = "/sEMG_WL.txt";
    QFile file_sEMG_WL(SaveFolderStr + filename);
    file_sEMG_WL.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_WL);
    for (int i = 0; i < stack_sEMG_WL[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_WL[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "3. sEMG WL save complete" << std::endl;

    // 4. sEMG SSC.txt
    filename = "/sEMG_SSC.txt";
    QFile file_sEMG_SSC(SaveFolderStr + filename);
    file_sEMG_SSC.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_SSC);
    for (int i = 0; i < stack_sEMG_SSC[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_SSC[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "4. sEMG SSC save complete" << std::endl;

    // 5. sEMG ZC.txt
    filename = "/sEMG_ZC.txt";
    QFile file_sEMG_ZC(SaveFolderStr + filename);
    file_sEMG_ZC.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_ZC);
    for (int i = 0; i < stack_sEMG_ZC[0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_sEMG_ZC[j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "5. sEMG ZC save complete" << std::endl;

    // 10. Raw flex sensor.txt
    filename = "/Flex_raw.txt";
    QFile file_Flex_raw(SaveFolderStr + filename);
    file_Flex_raw.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Flex_raw);
    for (int i = 0; i < stack_Flex_raw[0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Flex_raw[j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "10. Raw flex sensor save complete" << std::endl;

    // 11. Offset-processed flex sensor.txt
    filename = "/Flex_processed.txt";
    QFile file_Flex_processed(SaveFolderStr + filename);
    file_Flex_processed.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Flex_processed);
    for (int i = 0; i < stack_Flex_raw[0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Flex_processed[j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "11. Processed flex sensor save complete" << std::endl;

    // 12. Angle flex sensor.txt
    filename = "/Flex_angle.txt";
    QFile file_Flex_angle(SaveFolderStr + filename);
    file_Flex_angle.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Flex_angle);
    for (int i = 0; i < stack_Flex_Angle[0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Flex_Angle[j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "12. Angle flex sensor save complete" << std::endl;

    // 13. Motion label.txt
    filename = "/Motion_label.txt";
    QFile file_Motion_label(SaveFolderStr + filename);
    file_Motion_label.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Motion_label);
    for (int i = 0; i < stack_Motion_label.size(); i++) {
        SaveOut << QString::number(stack_Motion_label[i]);
        SaveOut << QString("\n");
    }
    std::cout << "13. Motion label save complete" << std::endl;

    // 14. Preprocessing time elapse
    filename = "/Time_elapse_Processing.txt";
    QFile file_Time_elapse_processing(SaveFolderStr + filename);
    file_Time_elapse_processing.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Time_elapse_processing);
    for (int i = 0; i < stack_Time_elapse_Processing.size(); i++) {
        SaveOut << QString::number(stack_Time_elapse_Processing[i]);
        SaveOut << QString("\n");
    }
    std::cout << "14. Time elapse for TwinCAT save complete" << std::endl;

    std::cout << "################ save complete ################" << std::endl;
}

void MainWindow::SaveData_BallControl(int trial_idx, int traj_idx) {
    std::cout << QString::number(trial_idx).toStdString() << "th trial, "
              << QString::number(traj_idx).toStdString() << "th trajectory) "
              << "Save Start!" << std::endl;

    // 0. Make Save Folder
    if (!PracticeOrNot) {
        if (!QDir(SaveFolderStr).exists())
            QDir().mkdir(SaveFolderStr);
    }

    // 1. sEMG_raw.txt
    QString filename = "/sEMG_raw.txt";
    QFile file_Ball_Ctr_sEMG_raw(SaveFolderStr + filename);
    file_Ball_Ctr_sEMG_raw.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_sEMG_raw);
    for (int i = 0; i < stack_Ball_Ctr_sEMG_raw[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_sEMG_raw[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "1. sEMG raw save complete" << std::endl;

    // 2. sEMG_MAV.txt
    filename = "/sEMG_MAV.txt";
    QFile file_Ball_Ctr_sEMG_MAV(SaveFolderStr + filename);
    file_Ball_Ctr_sEMG_MAV.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_sEMG_MAV);
    for (int i = 0; i < stack_Ball_Ctr_sEMG_MAV[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_sEMG_MAV[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "2. sEMG MAV save complete" << std::endl;

    // 3. sEMG MAV amplitude
    filename = "/sEMG_MAV_amp.txt";
    QFile file_sEMG_MAV_amp(SaveFolderStr + filename);
    file_sEMG_MAV_amp.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_MAV_amp);
    for (int i = 0; i < stack_Ball_Ctr_sEMG_MAV_amp[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_sEMG_MAV_amp[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "3. sEMG MAV amplitude save complete" << std::endl;

    // 4. Flags for motion exertion
    filename = "/isMotionExerted.txt";
    QFile file_isMotionExerted(SaveFolderStr + filename);
    file_isMotionExerted.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_isMotionExerted);
    for (int i = 0; i < stack_Ball_Ctr_isMotionExerted[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_isMotionExerted[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "4. Flags for motion exertion save complete" << std::endl;

    // 5. sEMG_WL.txt
    filename = "/sEMG_WL.txt";
    QFile file_Ball_Ctr_sEMG_WL(SaveFolderStr + filename);
    file_Ball_Ctr_sEMG_WL.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_sEMG_WL);
    for (int i = 0; i < stack_Ball_Ctr_sEMG_WL[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_sEMG_WL[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "5. sEMG WL save complete" << std::endl;

    // 6. sEMG_SSC.txt
    filename = "/sEMG_SSC.txt";
    QFile file_Ball_Ctr_sEMG_SSC(SaveFolderStr + filename);
    file_Ball_Ctr_sEMG_SSC.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_sEMG_SSC);
    for (int i = 0; i < stack_Ball_Ctr_sEMG_SSC[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_sEMG_SSC[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "6. sEMG SSC save complete" << std::endl;

    // 7. sEMG_ZC.txt
    filename = "/sEMG_ZC.txt";
    QFile file_Ball_Ctr_sEMG_ZC(SaveFolderStr + filename);
    file_Ball_Ctr_sEMG_ZC.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_sEMG_ZC);
    for (int i = 0; i < stack_Ball_Ctr_sEMG_ZC[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_EMG; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_sEMG_ZC[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_EMG - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "7. sEMG ZC save complete" << std::endl;

    // 8. IMU orientation
    filename = "/IMU_Ori.txt";
    QFile file_Ball_Ctr_IMU_Ori(SaveFolderStr + filename);
    file_Ball_Ctr_IMU_Ori.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_IMU_Ori);
    for (int i = 0; i < stack_Ball_Ctr_IMU_ori[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_IMU_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_IMU_ori[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_IMU_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "8. IMU orientation save complete" << std::endl;

    // 9. IMU gyroscope
    filename = "/IMU_Gyro.txt";
    QFile file_Ball_Ctr_IMU_Gyro(SaveFolderStr + filename);
    file_Ball_Ctr_IMU_Gyro.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_IMU_Gyro);
    for (int i = 0; i < stack_Ball_Ctr_IMU_gyro[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_IMU_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_IMU_gyro[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_IMU_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "9. IMU gyroscope save complete" << std::endl;

    // 10. IMU accelerometer
    filename = "/IMU_Acc.txt";
    QFile file_Ball_Ctr_IMU_Acc(SaveFolderStr + filename);
    file_Ball_Ctr_IMU_Acc.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_IMU_Acc);
    for (int i = 0; i < stack_Ball_Ctr_IMU_acc[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_IMU_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_IMU_acc[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_IMU_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "10. IMU accelerometer save complete" << std::endl;

    // 11. IMU magnetometer
    filename = "/IMU_Mag.txt";
    QFile file_Ball_Ctr_IMU_Mag(SaveFolderStr + filename);
    file_Ball_Ctr_IMU_Mag.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_IMU_Mag);
    for (int i = 0; i < stack_Ball_Ctr_IMU_mag[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_IMU_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_IMU_mag[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_IMU_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "11. IMU magnetometer save complete" << std::endl;

    // 12. Raw flex sensor.txt
    filename = "/Flex_raw.txt";
    QFile file_Ball_Ctr_Flex_raw(SaveFolderStr + filename);
    file_Ball_Ctr_Flex_raw.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Flex_raw);
    for (int i = 0; i < stack_Ball_Ctr_Flex_raw[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_Flex_raw[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "12. Raw flex sensor save complete" << std::endl;

    // 13. Offset-processed flex sensor.txt
    filename = "/Flex_processed.txt";
    QFile file_Ball_Ctr_Flex_processed(SaveFolderStr + filename);
    file_Ball_Ctr_Flex_processed.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Flex_processed);
    for (int i = 0; i < stack_Ball_Ctr_Flex_raw[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_Flex_processed[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "13. Processed flex sensor save complete" << std::endl;

    // 14. Angle flex sensor.txt
    filename = "/Flex_angle.txt";
    QFile file_Ball_Ctr_Flex_angle(SaveFolderStr + filename);
    file_Ball_Ctr_Flex_angle.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Flex_angle);
    for (int i = 0; i < stack_Ball_Ctr_Flex_Angle[trial_idx - 1][traj_idx - 1][0].size(); i++) {
        for (int j = 0; j < N_FLEX_CH; j++) {
            SaveOut << QString::number(stack_Ball_Ctr_Flex_Angle[trial_idx - 1][traj_idx - 1][j][i]);
            if (j != N_FLEX_CH - 1)
                SaveOut << QString(" ");
        }
        SaveOut << QString("\n");
    }
    std::cout << "14. Angle flex sensor save complete" << std::endl;

    // 15. Motion estimation from TwinCAT Thread
    filename = "/Motion_est_TwinCAT.txt";
    QFile file_Ball_Ctr_Motion_est(SaveFolderStr + filename);
    file_Ball_Ctr_Motion_est.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Motion_est);
    for (int i = 0; i < stack_Ball_Ctr_Motion_est[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Motion_est[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "15. Motion estimation (in TwinCAT thread) save complete" << std::endl;

    // 16. Motion estimation from TensorRT Thread
    filename = "/Motion_est_TensorRT.txt";
    QFile file_Ball_Ctr_Motion_est_TensorRT_thread(SaveFolderStr + filename);
    file_Ball_Ctr_Motion_est_TensorRT_thread.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Motion_est_TensorRT_thread);
    for (int i = 0; i < stack_Ball_Ctr_Motion_est_TensorRT_thread[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Motion_est_TensorRT_thread[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "16. Motion estimation (in TensorRT thread) save complete" << std::endl;

    // 17. Preprocessing time elapse
    filename = "/Time_elapse_Processing.txt";
    QFile file_Ball_Ctr_Time_elapse_processing(SaveFolderStr + filename);
    file_Ball_Ctr_Time_elapse_processing.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Time_elapse_processing);
    for (int i = 0; i < stack_Ball_Ctr_Time_elapse_Processing[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Time_elapse_Processing[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "17. Time elapse for TwinCAT save complete" << std::endl;

    // 18. TensorRT time elapse
    filename = "/Time_elapse_TensorRT.txt";
    QFile file_Ball_Ctr_Time_elapse_TensorRT(SaveFolderStr + filename);
    file_Ball_Ctr_Time_elapse_TensorRT.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Time_elapse_TensorRT);
    for (int i = 0; i < stack_Ball_Ctr_Time_elapse_TensorRT[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Time_elapse_TensorRT[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "18. Time elapse for TensorRT save complete" << std::endl;

    // 19. Ball Control - X pos.
    filename = "/Ball_X.txt";
    QFile file_Ball_Ctr_X(SaveFolderStr + filename);
    file_Ball_Ctr_X.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_X);
    for (int i = 0; i < stack_Ball_Ctr_X[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_X[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "19. Ball control X history save complete" << std::endl;

    // 20. Ball Control - Y pos.
    filename = "/Ball_Y.txt";
    QFile file_Ball_Ctr_Y(SaveFolderStr + filename);
    file_Ball_Ctr_Y.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Y);
    for (int i = 0; i < stack_Ball_Ctr_Y[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Y[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "20. Ball control Y history save complete" << std::endl;

    // 21. Ball Control - Rotation
    filename = "/Ball_Rot.txt";
    QFile file_Ball_Ctr_Rot(SaveFolderStr + filename);
    file_Ball_Ctr_Rot.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Rot);
    for (int i = 0; i < stack_Ball_Ctr_Rot[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Rot[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "21. Ball control Rot history save complete" << std::endl;

    // 22. Ball Control - Scale
    filename = "/Ball_Scale.txt";
    QFile file_Ball_Ctr_Scale(SaveFolderStr + filename);
    file_Ball_Ctr_Scale.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Scale);
    for (int i = 0; i < stack_Ball_Ctr_Scale[trial_idx - 1][traj_idx - 1].size(); i++) {
        SaveOut << QString::number(stack_Ball_Ctr_Scale[trial_idx - 1][traj_idx - 1][i]);
        SaveOut << QString("\n");
    }
    std::cout << "22. Ball control Scale history save complete" << std::endl;

    // 23. Save success and elapsed time
    filename = "/Ball_Success_Time_pos.txt";
    QFile file_Ball_Ctr_Success_Time(SaveFolderStr + filename);
    file_Ball_Ctr_Success_Time.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_Ball_Ctr_Success_Time);
    if (SuccessOrNot[trial_idx - 1][traj_idx - 1])
        SaveOut << "1";
    else
        SaveOut << "0";

    SaveOut << QString("\n");
    SaveOut << QString::number(BallCtrElapsedTime[trial_idx - 1][traj_idx - 1]);
    std::cout << "23. Ball control success and elapsed time save complete" << std::endl;

    // 24. MAV - Rest threshold
    filename = "/sEMG_MAV_thres_info.txt";
    QFile file_sEMG_MAV_thres_info(SaveFolderStr + filename);
    file_sEMG_MAV_thres_info.open(QIODevice::WriteOnly | QIODevice::Text);
    SaveOut.setDevice(&file_sEMG_MAV_thres_info);
    SaveOut << QString("1) Sum of mean sEMG MAV in rest period: ");
    SaveOut << QString::number(sum_sEMG_MAV_rest_mean);
    SaveOut << QString("\n");
    SaveOut << QString("2) Sum of standard deviation of sEMG MAV in rest period: ");
    SaveOut << QString::number(sum_sEMG_MAV_rest_std);
    SaveOut << QString("\n");
    SaveOut << QString("3) Standard deviation threshold scale: ");
    SaveOut << QString::number(STD_THRES_SCALE);
    SaveOut << QString("\n");
    SaveOut << QString("4) sEMG MAV threshold ((1) + (2) * (3)): ");
    SaveOut << QString::number(rest_thres);
    SaveOut << QString("\n");
    std::cout << "24. sEMG MAV rest threshold information save complete" << std::endl;

    std::cout << "################ save complete ################" << std::endl;
}

void MainWindow::ClearData_Training() {
    for (int i = 0; i < N_EMG; i++) {
        stack_sEMG_raw[i].clear();
        stack_sEMG_MAV[i].clear();
        stack_sEMG_WL[i].clear();
        stack_sEMG_SSC[i].clear();
        stack_sEMG_ZC[i].clear();
    }

    for (int i = 0; i < N_FLEX_CH; i++) {
        stack_Flex_raw[i].clear();
        stack_Flex_processed[i].clear();
        stack_Flex_Angle[i].clear();
    }

    stack_Motion_label.clear();
    stack_Time_elapse_Processing.clear();
}

void MainWindow::ClearData_UnlabeledDAQ() {
    for (int i = 0; i < N_EMG; i++) {
        stack_sEMG_raw[i].clear();
        stack_sEMG_MAV[i].clear();
        stack_sEMG_WL[i].clear();
        stack_sEMG_SSC[i].clear();
        stack_sEMG_ZC[i].clear();
    }

    for (int i = 0; i < N_FLEX_CH; i++) {
        stack_Flex_raw[i].clear();
        stack_Flex_processed[i].clear();
        stack_Flex_Angle[i].clear();
    }

    stack_Motion_label.clear();
    stack_Time_elapse_Processing.clear();
}

void MainWindow::ClearData_BallControl(int trial_idx, int traj_idx) {
    for (int i = 0; i < N_EMG; i++) {
        stack_Ball_Ctr_sEMG_raw[trial_idx - 1][traj_idx - 1][i].clear();
        stack_Ball_Ctr_sEMG_MAV[trial_idx - 1][traj_idx - 1][i].clear();
        stack_Ball_Ctr_sEMG_WL[trial_idx - 1][traj_idx - 1][i].clear();
        stack_Ball_Ctr_sEMG_SSC[trial_idx - 1][traj_idx - 1][i].clear();
        stack_Ball_Ctr_sEMG_ZC[trial_idx - 1][traj_idx - 1][i].clear();
    }
    stack_Ball_Ctr_sEMG_MAV_amp[trial_idx - 1][traj_idx - 1].clear();

    for (int i = 0; i < N_FLEX_CH; i++) {
        stack_Ball_Ctr_Flex_raw[trial_idx - 1][traj_idx - 1][i].clear();
        stack_Ball_Ctr_Flex_processed[trial_idx - 1][traj_idx - 1][i].clear();
        stack_Ball_Ctr_Flex_Angle[trial_idx - 1][traj_idx - 1][i].clear();
    }

    stack_Ball_Ctr_Motion_est[trial_idx - 1][traj_idx - 1].clear();
    stack_Ball_Ctr_Motion_est_TensorRT_thread[trial_idx - 1][traj_idx - 1].clear();
    stack_Ball_Ctr_Time_elapse_Processing[trial_idx - 1][traj_idx - 1].clear();
    stack_Ball_Ctr_Time_elapse_TensorRT[trial_idx - 1][traj_idx - 1].clear();

    stack_Ball_Ctr_X[trial_idx - 1][traj_idx - 1].clear();
    stack_Ball_Ctr_Y[trial_idx - 1][traj_idx - 1].clear();
    stack_Ball_Ctr_Rot[trial_idx - 1][traj_idx - 1].clear();
    stack_Ball_Ctr_Scale[trial_idx - 1][traj_idx - 1].clear();

    stack_Ball_Ctr_isMotionExerted[trial_idx - 1][traj_idx - 1].clear();
}

void MainWindow::on_BtnSwitch_clicked() {
    if (!m_flag_Switch) { //Turn OFF -> Turn ON
        if (!isTwinCATStarted) {
            connect(&m_pTimer_ControlTime, SIGNAL(timeout()), this, SLOT(OnTimerControlTimeCallbackFunc()));
            m_pTimer_ControlTime.start(20);

            connect(&m_pTimer_plot, SIGNAL(timeout()), this, SLOT(OnTimerRTPlotCallbackFunc()));
            m_pTimer_plot.start(20);

            connect(&m_pTimer_fig, SIGNAL(timeout()), this, SLOT(OnTimerFigCallbackFunc()));
            m_pTimer_fig.start(20);

            std::thread Thread_TwinCAT(&MainWindow::Thread_TwinCAT_func, this);
            Thread_TwinCAT.detach();
        }
        else if (isTwinCATStarted && m_radioSaveOpt == 1) { // 정지했다가 다시 시작하는 경우 (저장 ㅇ)
            mtx.lock();
            pShared_Data->bContinue = TRUE;
            mtx.unlock();

            m_pTimer_plot.start(20);
            if (m_radioMode == 0)
                player_Training->get_QMediaPlayer()->play();
            else if (m_radioMode == 1)
                player_UnlabeledDAQ->get_QMediaPlayer()->play();
        }
        else { // 중간에 정지했다가 다시 시작하는 경우 (저장 X)
            mtx.lock();
            pShared_Data->bContinue = TRUE;
            mtx.unlock();

            m_pTimer_plot.start(20);
            if (m_radioMode == 0)
                player_Training->get_QMediaPlayer()->play();
            else if (m_radioMode == 1)
                player_UnlabeledDAQ->get_QMediaPlayer()->play();
        }

        isTwinCATStarted = true;
        ui->BtnSwitch->setText("Stop");
        m_flag_Switch = true;

        ui->lineEdit_Trial_Idx->setEnabled(false);
        ui->lineEdit_Traj_Idx->setEnabled(false);

        ui->BtnDelete->setEnabled(false);
        ui->BtnStackSave->setEnabled(false);

        ui->radioBtn_Mode_1->setEnabled(false);
        ui->radioBtn_Mode_2->setEnabled(false);
        ui->radioBtn_Mode_3->setEnabled(false);
        ui->radioBtn_Mode_4->setEnabled(false);

        ui->radioBtn_Network_1->setEnabled(false);
        ui->radioBtn_Network_2->setEnabled(false);
        ui->radioBtn_Network_3->setEnabled(false);

        ui->radioBtn_SaveOption_1->setEnabled(false);
        ui->radioBtn_SaveOption_2->setEnabled(false);
        ui->radioBtn_SaveOption_3->setEnabled(false);
        ui->radioBtn_SaveOption_4->setEnabled(false);

        ui->radioBtn_DataStream_1->setEnabled(false);
        ui->radioBtn_DataStream_2->setEnabled(false);
        ui->BtnLoad->setEnabled(false);

        if (m_radioMode == 2 && PracticeOrNot)
            ui->BtnSwitch->setEnabled(false);
    }
    else { // Turn ON -> Turn OFF
        if (m_radioSaveOpt != 0) {
            ui->BtnSwitch->setText("Start");
            ui->BtnSwitch->setEnabled(true);
        }
        else {
            ui->BtnSwitch->setText("Exp. \nEnd");
            ui->BtnSwitch->setEnabled(false);
        }

        mtx.lock();
        pShared_Data->bContinue = FALSE;
        mtx.unlock();
        m_flag_Switch = FALSE;
        m_pTimer_plot.stop();

        if (m_radioMode == 0)
            player_Training->get_QMediaPlayer()->pause();
        else if (m_radioMode == 1)
            player_UnlabeledDAQ->get_QMediaPlayer()->pause();

        if (m_radioSaveOpt == 0 || m_radioSaveOpt == 1) {
            // Save the data (Only for immediate quit)
            if (m_radioMode == 0)
                SaveData_Training();
            else if (m_radioMode == 1)
                SaveData_UnlabeledDAQ();
            else if (m_radioMode == 2)
                SaveData_BallControl(Trial_idx, Traj_idx);

            // Clear existing data (Only for immediate quit)
            if (m_radioSaveOpt == 1) {
                if (m_radioMode == 0)
                    ClearData_Training();
                else if (m_radioMode == 1)
                    ClearData_UnlabeledDAQ();
                else if (m_radioMode == 2)
                    ClearData_BallControl(Trial_idx, Traj_idx);
            }
        }

        if (m_radioSaveOpt != 0) {
            if (m_radioSaveOpt == 2) {
                ui->BtnStackSave->setEnabled(true);

                if (m_radioMode == 0 || m_radioMode == 1)
                    ui->BtnDelete->setEnabled(true);
            }

            ui->radioBtn_SaveOption_1->setEnabled(true);
            ui->radioBtn_SaveOption_2->setEnabled(true);
            ui->radioBtn_SaveOption_3->setEnabled(true);
            ui->radioBtn_SaveOption_4->setEnabled(true);
        }
    }
}

void MainWindow::on_BtnDelete_clicked() {
    int residue = (pShared_Data->count - m_time_last_cnt) % 1000;
    m_time_last_cnt += (1000 + residue);
    m_time_last += (1000 + residue) / 1000.0;
    PopUpData(1000 + residue);

    QImage* qimg_motion;
    QPixmap qpix_motion;

    Motion_label = Get_Motion_Label(m_radioMode, m_time - m_time_last);
    qimg_motion = new QImage((unsigned char *)Figures_motion[Motion_label].data,
                              Figures_motion[Motion_label].cols, Figures_motion[Motion_label].rows,
                              QImage::Format_ARGB32);
    qpix_motion = QPixmap::fromImage(*qimg_motion);

    if (m_radioMode == 0) {
        //////////////////////////////////////// TrainingForm ////////////////////////////////////////
        trnForm.get_UI()->Img_Truth->setPixmap(qpix_motion.scaled(trnForm.get_UI()->Img_Truth->width(),
                                  trnForm.get_UI()->Img_Truth->height(),
                                  Qt::IgnoreAspectRatio));
        delete qimg_motion;

        /////////////////////////////////////// TrainingUserForm ///////////////////////////////////////
        // (TrainingUserForm) Figures - State indicator
        State_indicator = Get_State_Indicator(m_radioMode, m_time - m_time_last);
        qimg_motion = new QImage((unsigned char *)Figures_motion[State_indicator].data,
                                                  Figures_motion[State_indicator].cols, Figures_motion[State_indicator].rows,
                                                  QImage::Format_ARGB32);
        qpix_motion = QPixmap::fromImage(*qimg_motion);

        // Video
        player_Training->positionChange(m_time_cnt - m_time_last_cnt);

        // 3D modeling - Shoulder
        player_Training->get_Model()->set_Upper_arm_RotX(trj_shd_X[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Upper_arm_Axis(trj_shd_axis[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Upper_arm_RotY(trj_shd_Y[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Upper_arm_RotZ(trj_shd_Z[m_time_cnt - m_time_last_cnt]);

        // 3D modeling - Elbow
        player_Training->get_Model()->set_Lower_arm_RotX(trj_elb_X[m_time_cnt - m_time_last_cnt]);

        // 3D modeling - Wrist
        player_Training->get_Model()->set_Palm_RotX(trj_wrist_X[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Lower_arm_RotY(trj_wrist_Y[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Palm_RotZ(trj_wrist_Z[m_time_cnt - m_time_last_cnt]);

        // 3D modeling - Hand
        player_Training->get_Model()->set_Thumb_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Index_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Middle_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Ring_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_Training->get_Model()->set_Little_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
    }
    else if (m_radioMode == 1) {
        //////////////////////////////////////// TrainingForm ////////////////////////////////////////
        // (TrainingForm)
        unlabeledDAQForm.get_UI()->Img_Truth->setPixmap(qpix_motion.scaled(unlabeledDAQForm.get_UI()->Img_Truth->width(),
                                  unlabeledDAQForm.get_UI()->Img_Truth->height(),
                                  Qt::IgnoreAspectRatio));
        delete qimg_motion;

        /////////////////////////////////////// TrainingUserForm ///////////////////////////////////////
        // (TrainingUserForm) Figures - State indicator
        State_indicator = Get_State_Indicator(m_radioMode, m_time - m_time_last);
        qimg_motion = new QImage((unsigned char *)Figures_motion[State_indicator].data,
                                                  Figures_motion[State_indicator].cols, Figures_motion[State_indicator].rows,
                                                  QImage::Format_ARGB32);
        qpix_motion = QPixmap::fromImage(*qimg_motion);

        // Video
        player_UnlabeledDAQ->positionChange(m_time_cnt - m_time_last_cnt);

        // 3D modeling - Shoulder
        player_UnlabeledDAQ->get_Model()->set_Upper_arm_RotX(trj_shd_X[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Upper_arm_Axis(trj_shd_axis[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Upper_arm_RotY(trj_shd_Y[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Upper_arm_RotZ(trj_shd_Z[m_time_cnt - m_time_last_cnt]);

        // 3D modeling - Elbow
        player_UnlabeledDAQ->get_Model()->set_Lower_arm_RotX(trj_elb_X[m_time_cnt - m_time_last_cnt]);

        // 3D modeling - Wrist
        player_UnlabeledDAQ->get_Model()->set_Palm_RotX(trj_wrist_X[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Lower_arm_RotY(trj_wrist_Y[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Palm_RotZ(trj_wrist_Z[m_time_cnt - m_time_last_cnt]);

        // 3D modeling - Hand
        player_UnlabeledDAQ->get_Model()->set_Thumb_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Index_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Middle_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Ring_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
        player_UnlabeledDAQ->get_Model()->set_Little_RotZ(trj_hand[m_time_cnt - m_time_last_cnt]);
    }
}

void MainWindow::on_BtnStackSave_clicked() {
    if (m_radioMode == 0)
        SaveData_Training();
    else if (m_radioMode == 1)
        SaveData_UnlabeledDAQ();
    else if (m_radioMode == 2)
        SaveData_BallControl(Trial_idx, Traj_idx);

    // 1. Next ball control trajectory & corresponding save folder setup
    if (m_radioMode == 2) {
        isSuccess_time_flag = false;
        isSuccess = false;
        player_TestOne.Get_PaintTestOne()->Set_isSuccess(isSuccess);

        isRun = false;
        isTimeOver_time_flag = false;
        isTimeOver = false;
        player_TestOne.Get_PaintTestOne()->Set_isTimeOver(isTimeOver);

        T_trial_start = m_time;
        player_TestOne.Get_PaintTestOne()->Set_T_Trial_Start(T_trial_start);

        if (Trial_idx < N_TRIALS) {
            if (Traj_idx < N_TRAJECTORY) {
                Traj_idx++;
                player_TestOne.Get_PaintTestOne()->Set_Traj_idx(Traj_idx);
                ui->lineEdit_Traj_Idx->setText(QString::number(Traj_idx));
            }
            else {
                Trial_idx++;
                player_TestOne.Get_PaintTestOne()->Set_Trial_idx(Trial_idx);
                ui->lineEdit_Trial_Idx->setText(QString::number(Trial_idx));

                Traj_idx = 1;
                player_TestOne.Get_PaintTestOne()->Set_Traj_idx(Traj_idx);
                ui->lineEdit_Traj_Idx->setText(QString::number(Traj_idx));
            }
        }
        else if (Trial_idx == N_TRIALS && Traj_idx < N_TRAJECTORY){
            Traj_idx++;
            player_TestOne.Get_PaintTestOne()->Set_Traj_idx(Traj_idx);
            ui->lineEdit_Traj_Idx->setText(QString::number(Traj_idx));
        }

        TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
        SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr + NetworkStr +
                            "_Trial" + QString::number(Trial_idx) + "_Traj" + QString::number(Traj_idx);
        ui->lineEdit_SaveFolder->setText(SaveFolderStr);

        player_TestOne.Get_PaintTestOne()->Set_Cur_X(player_TestOne.Get_PaintTestOne()->Get_Start_X(Traj_idx));
        player_TestOne.Get_PaintTestOne()->Set_Cur_Y(player_TestOne.Get_PaintTestOne()->Get_Start_Y(Traj_idx));
        player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(player_TestOne.Get_PaintTestOne()->Get_Start_Rot(Traj_idx));
        player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(player_TestOne.Get_PaintTestOne()->Get_Start_Scale(Traj_idx));

        ui->lineEdit_Trial_Idx->setEnabled(true);
        ui->lineEdit_Traj_Idx->setEnabled(true);

        m_time_last = m_time;
        m_time_last_cnt = m_time_cnt;
    }

    // 2. Rest mean value initialization
    for (int i = 0 ; i < N_EMG; i++) {
        sEMG_MAV_rest_mean[i] = 0.0;
        sEMG_MAV_rest_square_mean[i] = 0.0;
        sEMG_WL_rest_mean[i] = 0.0;
        sEMG_WL_rest_square_mean[i] = 0.0;
    }
    sum_sEMG_MAV_rest_mean = 0.0;
    sum_sEMG_MAV_rest_std = 0.0;
    sum_sEMG_WL_rest_mean = 0.0;
    sum_sEMG_WL_rest_std = 0.0;

    // 3. Clear data for ball control session
    if (m_radioMode == 0)
        ClearData_Training();
    else if (m_radioMode == 1)
        ClearData_UnlabeledDAQ();
    else if (m_radioMode == 2)
        ClearData_BallControl(Trial_idx, Traj_idx);

    // 4. Enable / Disable Button
    ui->BtnStackSave->setEnabled(false);
}

void MainWindow::on_BtnSkipPractice_clicked() {
    if (PracticeOrNot)
        ui->BtnSwitch->setEnabled(true);

    PracticeOrNot = false;
    player_TestOne.Get_PaintTestOne()->Set_PracticeOrNot(PracticeOrNot);
    player_TestOne.Get_PaintTestOne()->Set_Trial_idx(Trial_idx);
    player_TestOne.Get_PaintTestOne()->Set_Traj_idx(Traj_idx);

    player_TestOne.Get_PaintTestOne()->Set_Cur_X(player_TestOne.Get_PaintTestOne()->Get_Start_X(Traj_idx));
    player_TestOne.Get_PaintTestOne()->Set_Cur_Y(player_TestOne.Get_PaintTestOne()->Get_Start_Y(Traj_idx));
    player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(player_TestOne.Get_PaintTestOne()->Get_Start_Rot(Traj_idx));
    player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(player_TestOne.Get_PaintTestOne()->Get_Start_Scale(Traj_idx));

    T_trial_start = m_time;
    m_time_last = m_time;
    m_time_last_cnt = m_time_cnt;
    player_TestOne.Get_PaintTestOne()->Set_T_Trial_Start(T_trial_start);

    if (isTwinCATStarted)
        on_BtnSwitch_clicked();
    ui->BtnSkipPractice->setEnabled(false);
    ui->lineEdit_Trial_Idx->setEnabled(true);
    ui->lineEdit_Traj_Idx->setEnabled(true);

    isRun = false;
    isTimeOver = false;
    player_TestOne.Get_PaintTestOne()->Set_isTimeOver(isTimeOver);

    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
    SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr + NetworkStr
                        + "_Trial" + QString::number(Trial_idx) + "_Traj" + QString::number(Traj_idx);
    ui->lineEdit_SaveFolder->setText(SaveFolderStr);
}

void MainWindow::on_BtnLoad_clicked() {
    QStringList dir = QFileDialog::getOpenFileNames(this, "Select the files",
                                                    "../TRT_demo/LoadData",
                                                    tr("text files(*.txt)"));

    if (dir.size() != 5) {
        QMessageBox::warning(this, "Warning", "Select only 5 text files!");
        return;
    }

    stack_sEMG_MAV_load = new std::vector<double>[N_EMG];
    stack_sEMG_WL_load = new std::vector<double>[N_EMG];
    stack_sEMG_SSC_load = new std::vector<double>[N_EMG];
    stack_sEMG_ZC_load = new std::vector<double>[N_EMG];

    // 1. Motion label
    QFile file_label(dir[0]);
    if (!file_label.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file_label.errorString());
    }

    QTextStream in_label(&file_label);
    while(!in_label.atEnd()) {
        QString line = in_label.readLine();
        stack_Motion_label_load.push_back(line.toDouble());
    }
    std::cout << "Motion label) Data Loaded" << std::endl;

    // 2. sEMG MAV
    QFile file_MAV(dir[1]);
    if (!file_MAV.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file_MAV.errorString());
    }

    QTextStream in_MAV(&file_MAV);
    while(!in_MAV.atEnd()) {
        QString line = in_MAV.readLine();
        QStringList fields = line.split(" ");
        for (int j = 0; j < N_EMG; j++)
            stack_sEMG_MAV_load[j].push_back(fields[j].toDouble());
    }
    std::cout << "sEMG MAV) Data Loaded" << std::endl;

    // 3. sEMG SSC
    QFile file_SSC(dir[2]);
    if (!file_SSC.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file_SSC.errorString());
    }

    QTextStream in_SSC(&file_SSC);
    while(!in_SSC.atEnd()) {
        QString line = in_SSC.readLine();
        QStringList fields = line.split(" ");
        for (int j = 0; j < N_EMG; j++)
            stack_sEMG_SSC_load[j].push_back(fields[j].toDouble());
    }
    std::cout << "sEMG SSC) Data Loaded" << std::endl;

    // 4. sEMG WL
    QFile file_WL(dir[3]);
    if (!file_WL.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file_WL.errorString());
    }

    QTextStream in_WL(&file_WL);
    while(!in_WL.atEnd()) {
        QString line = in_WL.readLine();
        QStringList fields = line.split(" ");
        for (int j = 0; j < N_EMG; j++)
            stack_sEMG_WL_load[j].push_back(fields[j].toDouble());
    }
    std::cout << "sEMG WL) Data Loaded" << std::endl;

    // 5. sEMG ZC
    QFile file_ZC(dir[4]);
    if (!file_ZC.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file_ZC.errorString());
    }

    QTextStream in_ZC(&file_ZC);
    while(!in_ZC.atEnd()) {
        QString line = in_ZC.readLine();
        QStringList fields = line.split(" ");
        for (int j = 0; j < N_EMG; j++)
            stack_sEMG_ZC_load[j].push_back(fields[j].toDouble());
    }
    std::cout << "sEMG ZC) Data Loaded" << std::endl;

    isDataLoaded = true;
    ui->BtnLoad->setEnabled(false);
}

void MainWindow::on_lineEdit_Sbj_Name_textEdited(const QString &arg1) {
    NameStr = arg1;
    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
    if (m_radioMode == 0) {
        SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr;
    }

    ui->lineEdit_SaveFolder->setText(SaveFolderStr);
}

void MainWindow::on_lineEdit_Trial_Idx_textEdited(const QString &arg1) {
    Trial_idx = arg1.toInt();
    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
    SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr + NetworkStr
                        + "_Trial" + QString::number(Trial_idx) + "_Traj" + QString::number(Traj_idx);
    ui->lineEdit_SaveFolder->setText(SaveFolderStr);
}

void MainWindow::on_lineEdit_Traj_Idx_textEdited(const QString &arg1) {
    Traj_idx = arg1.toInt();
    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
    SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr + NetworkStr
                        + "_Trial" + QString::number(Trial_idx) + "_Traj" + QString::number(Traj_idx);
    ui->lineEdit_SaveFolder->setText(SaveFolderStr);

    if (m_radioMode == 2 && !PracticeOrNot) {
        player_TestOne.Get_PaintTestOne()->Set_Traj_idx(Traj_idx);
        player_TestOne.Get_PaintTestOne()->Set_Cur_X(player_TestOne.Get_PaintTestOne()->Get_Start_X(Traj_idx));
        player_TestOne.Get_PaintTestOne()->Set_Cur_Y(player_TestOne.Get_PaintTestOne()->Get_Start_Y(Traj_idx));
        player_TestOne.Get_PaintTestOne()->Set_Cur_Rot(player_TestOne.Get_PaintTestOne()->Get_Start_Rot(Traj_idx));
        player_TestOne.Get_PaintTestOne()->Set_Cur_Scale(player_TestOne.Get_PaintTestOne()->Get_Start_Scale(Traj_idx));
    }
}

void MainWindow::on_lineEdit_WinSize_textEdited(const QString &arg1) {
    Win_size = arg1.toInt();
}

void MainWindow::on_lineEdit_MAVMax_textEdited(const QString &arg1) {
    Graph_MAV_max = arg1.toDouble();
}

void MainWindow::RadioCtrl_Mode() {
    if (ui->radioBtn_Mode_1->isChecked()) { // Training
        Close_Dialog();
        player_Training->show();
        m_radioMode = 0;

        ui->stackedWdg->setCurrentIndex(m_radioMode);
        ui->lineEdit_Trial_Idx->setEnabled(false);
        ui->lineEdit_Traj_Idx->setEnabled(false);

        ui->radioBtn_Network_1->setEnabled(false);
        ui->radioBtn_Network_2->setEnabled(false);
        ui->radioBtn_Network_3->setEnabled(false);

        ui->radioBtn_DataStream_1->setEnabled(true);
        ui->radioBtn_DataStream_2->setEnabled(false);

        ui->BtnSwitch->setEnabled(true);
        ui->BtnDelete->setEnabled(true);
        ui->BtnSkipPractice->setEnabled(false);

        TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
        ModeStr = "_Training";
        SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr;
        ui->lineEdit_SaveFolder->setText(SaveFolderStr);

        Set_Motion_Trajectory(m_radioMode);
    }
    else if (ui->radioBtn_Mode_2->isChecked()) {
        Close_Dialog();
        player_UnlabeledDAQ->show();
        m_radioMode = 1;

        ui->stackedWdg->setCurrentIndex(m_radioMode);
        ui->lineEdit_Trial_Idx->setEnabled(false);
        ui->lineEdit_Traj_Idx->setEnabled(false);

        ui->radioBtn_Network_1->setEnabled(false);
        ui->radioBtn_Network_2->setEnabled(false);
        ui->radioBtn_Network_3->setEnabled(false);

        ui->radioBtn_DataStream_1->setEnabled(true);
        ui->radioBtn_DataStream_2->setEnabled(false);

        ui->BtnSwitch->setEnabled(true);
        ui->BtnDelete->setEnabled(true);
        ui->BtnSkipPractice->setEnabled(false);

        TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
        ModeStr = "_Unlabeled_DAQ";
        SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr;
        ui->lineEdit_SaveFolder->setText(SaveFolderStr);

        // Set the randomized rate of motions
        bool resmp_required = true;
        int N_min = 0;
        int N_max = 0;
        while (resmp_required) {
            // Probability sampling
            std::srand(static_cast<unsigned int>(std::time(0)));
            double sum_Motion_prob = 0.0;
            for (int M = 0; M < N_MOTIONS; M++) {
                Motion_prob[M] = (double)(PROB_MIN +
                                    (PROB_MAX - PROB_MIN) * ((double)std::rand() / (double)RAND_MAX));
                sum_Motion_prob += Motion_prob[M];

                Target_motion_N[M] = 0;
            }

            // Normalization to convert into probability
            for (int M = 0; M < N_MOTIONS; M++) {
                Motion_prob[M] /= sum_Motion_prob;
            }

            // Target motion sampling
            int mot_idx = 0;
            for (int i = 0; i < N_REPEAT; i++) {
                double rand_smp = (double)std::rand() / (double)(RAND_MAX + 1);
                double M_prob_sum = 0.0;
                for (int M = 0; M < N_MOTIONS; M++) {
                    M_prob_sum += Motion_prob[M];

                    if (rand_smp < M_prob_sum) {
                        mot_idx = M + 1;
                        break;
                    }

                    if (M == N_MOTIONS - 1) {
                        mot_idx = M + 1;
                    }
                }

                Target_motion_N[mot_idx - 1] += 1;
                Target_motion_list[i] = mot_idx;
            }

            // The minimal & maximal number of target motions
            N_min = Target_motion_N[0];
            N_max = Target_motion_N[0];
            for (int M = 1; M < N_MOTIONS; M++) {
                if (Target_motion_N[M] < N_min)
                    N_min = Target_motion_N[M];

                if (Target_motion_N[M] > N_max)
                    N_max = Target_motion_N[M];
            }

            if (N_min >= N_MOTION_MIN && N_max <= N_MOTION_MAX)
                resmp_required = false;
        }

        std::cout.precision(3);
        std::cout << "\nNormalized motion-wise probbability: ";
        for (int M = 0; M < N_MOTIONS; M++) {
            if (M == N_MOTIONS - 1)
                std::cout << Motion_prob[M] * 100 << "%";
            else
                std::cout << Motion_prob[M] * 100 << "%, ";
        }
        std::cout << std::endl;

        std::cout << "The number of each target motion: ";
        for (int M = 0; M < N_MOTIONS; M++) {
            if (M == N_MOTIONS - 1)
                std::cout << Target_motion_N[M];
            else
                std::cout << Target_motion_N[M] << ", ";
        }
        std::cout << " (Min: " << N_min << ", Max: " << N_max << ")" << std::endl;
        std::cout.precision(6);

        std::cout << "Target motion list: ";
        for (int i = 0; i < N_REPEAT; i++) {
            std::cout << Target_motion_list[i] << " ";
        }
        std::cout << std::endl;

        Set_Motion_Trajectory(m_radioMode);
    }
    else if (ui->radioBtn_Mode_3->isChecked()) {
        Close_Dialog();
        player_TestOne.show();
        m_radioMode = 2;

        ui->stackedWdg->setCurrentIndex(m_radioMode);
        ui->lineEdit_Trial_Idx->setEnabled(false);
        ui->lineEdit_Traj_Idx->setEnabled(false);

        ui->radioBtn_Network_1->setEnabled(true);
        ui->radioBtn_Network_2->setEnabled(true);
        ui->radioBtn_Network_3->setEnabled(true);

        if (!ui->radioBtn_Network_1->isChecked() &&
            !ui->radioBtn_Network_2->isChecked() &&
            !ui->radioBtn_Network_3->isChecked()) {
            ui->radioBtn_DataStream_1->setEnabled(false);
            ui->radioBtn_DataStream_2->setEnabled(false);
        }

        if (!ui->radioBtn_Network_1->isChecked() &&
            !ui->radioBtn_Network_2->isChecked() &&
            !ui->radioBtn_Network_3->isChecked())
            ui->BtnSwitch->setEnabled(false);
        ui->BtnDelete->setEnabled(false);
        ui->BtnSkipPractice->setEnabled(true);

        TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
        ModeStr = "_Ball_Control";
        SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr + NetworkStr
                            + "_Trial" + QString::number(Trial_idx) + "_Traj" + QString::number(Traj_idx);
        ui->lineEdit_SaveFolder->setText(SaveFolderStr);
    }
}

void MainWindow::RadioCtrl_Network() {
    if (ui->radioBtn_Network_1->isChecked()) { // No network loaded
        m_radioNetwork = 0;
        NetworkStr = "_Fine_Tune";
        if (TRT_Fine_Tune == nullptr)
            TRT_Fine_Tune = new TensorRT_module("FT_C_sbj_T12_sbj_S3_iter50.uff");    // For CNN comparison
    }
    else if (ui->radioBtn_Network_2->isChecked()) { // No network loaded
        m_radioNetwork = 1;
        NetworkStr = "_STM";
        if (TRT_STM == nullptr)
            TRT_STM = new TensorRT_module("STM_C_sbj_T12_sbj_S3_beta_0.20_gamma_0.00_iter50.uff");  // For WDGRL comparison
    }
    else if (ui->radioBtn_Network_3->isChecked()) { // No network loaded
        m_radioNetwork = 2;
        NetworkStr = "_Proposed";
        if (TRT_Proposed == nullptr)
            TRT_Proposed = new TensorRT_module("STDAN_C_sbj_T12_sbj_S3_acpt_thres_0.4_iter10.uff");  // For WDGRL comparison
    }

    ui->BtnSwitch->setEnabled(true);
    ui->radioBtn_DataStream_1->setEnabled(true);
    ui->radioBtn_DataStream_2->setEnabled(true);

    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
    SaveFolderStr = PathStr + TimeStr + NameStr + ModeStr + NetworkStr
                            + "_Trial" + QString::number(Trial_idx) + "_Traj" + QString::number(Traj_idx);
    ui->lineEdit_SaveFolder->setText(SaveFolderStr);
}

void MainWindow::RadioCtrl_SaveOpt() {
    if (isTwinCATStarted) {
        mtx.lock();
        if (ui->radioBtn_SaveOption_1->isChecked()) {
            pShared_Data->bSaveImmediate = true;
            ui->BtnStackSave->setEnabled(false);
            ui->BtnDelete->setEnabled(false);
        }
        else if (ui->radioBtn_SaveOption_2->isChecked()) {
            pShared_Data->bSaveImmediate = false;
            ui->BtnStackSave->setEnabled(false);
            ui->BtnDelete->setEnabled(false);
        }
        else if (ui->radioBtn_SaveOption_3->isChecked()) {
            pShared_Data->bSaveImmediate = false;
            ui->BtnStackSave->setEnabled(true);
            ui->BtnDelete->setEnabled(true);
        }
        else if (ui->radioBtn_SaveOption_4->isChecked()) {
            pShared_Data->bSaveImmediate = false;
            ui->BtnStackSave->setEnabled(false);
            ui->BtnDelete->setEnabled(false);
        }
        mtx.unlock();
    }
    else {
        if (ui->radioBtn_SaveOption_1->isChecked()) {
            m_radioSaveOpt = 0;
            ui->BtnStackSave->setEnabled(false);
        }
        else if (ui->radioBtn_SaveOption_2->isChecked()) {
            m_radioSaveOpt = 1;
            ui->BtnStackSave->setEnabled(false);
        }
        else if (ui->radioBtn_SaveOption_3->isChecked()) {
            m_radioSaveOpt = 2;
            ui->BtnStackSave->setEnabled(true);
        }
        else if (ui->radioBtn_SaveOption_4->isChecked()) {
            m_radioSaveOpt = 3;
            ui->BtnStackSave->setEnabled(false);
        }
    }
}

void MainWindow::RadioCtrl_StrOpt() {
    if (ui->radioBtn_DataStream_1->isChecked()) {
        m_radioStrOpt = 0;
        ui->BtnLoad->setEnabled(false);
    }
    else if (ui->radioBtn_DataStream_2->isChecked()) {
        m_radioStrOpt = 1;
        ui->BtnLoad->setEnabled(true);
    }
}

void MainWindow::Close_Dialog() {
    if (m_radioMode == 0)
        player_Training->close();
    else if (m_radioMode == 1)
        player_UnlabeledDAQ->close();
    else if (m_radioMode == 2)
        player_TestOne.close();
}

//////////////////////////// 'Get' functions ////////////////////////////////////////////
TrainingUserForm* MainWindow::get_VideoPlayer_Training() {
    return player_Training;
}

UnlabeledDAQUserForm* MainWindow::get_VideoPlayer_UnlabeledDAQ() {
    return player_UnlabeledDAQ;
}

//////////////////////////// 'Set' functions ////////////////////////////////////////////
void MainWindow::Set_TimeStr() {
    TimeStr = QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");
}

int MainWindow::Get_Motion_Label(int mode, double time) {
    int mot_idx = 0;
    double T_start = 0.0; // Starting time for this trial
    double T_end = 0.0;   // Starting time for this trial

    if (mode == 0) {        // Training
        for (int M = 0; M < N_MOTIONS; M++) {
            T_start = T_READY + T_REST * M + T_CONTRACT * M + T_MARGIN_START_TRN;
            T_end = T_READY + T_REST * M + T_CONTRACT * (M + 1) - T_MARGIN_END_TRN;

            if (T_start <= time && time < T_end) {
                mot_idx = M + 1;
                break;
            }
        }
    }
    else if (mode == 1) {   // Unlabeled DAQ
        for (int i = 0; i < N_REPEAT; i++) {
            T_start = T_READY + T_REST * i + T_CONTRACT * i + T_MARGIN_START_UNLABELED_DAQ;
            T_end = T_READY + T_REST * i + T_CONTRACT * (i + 1) - T_MARGIN_END_UNLABELED_DAQ;

            if (T_start <= time && time < T_end) {
                mot_idx = Target_motion_list[i];
                break;
            }
        }
    }

    return mot_idx;
}

int MainWindow::Get_State_Indicator(int mode, double time) {
    int state_idx = 0;
    double T_start = 0.0; // Starting time for this trial
    double T_end = 0.0;   // Starting time for this trial

    if (mode == 0) {        // Training
        for (int M = 0; M < N_MOTIONS; M++) {
            T_start = T_READY + T_REST * M + T_CONTRACT * M;
            T_end = T_READY + T_REST * M + T_CONTRACT * (M + 1);

            if (T_start <= time && time < T_end) {
                state_idx = M + 1;
                break;
            }
        }
    }
    else if (mode == 1) {   // Unlabeled DAQ
        for (int i = 0; i < N_REPEAT; i++) {
            T_start = T_READY + T_REST * i + T_CONTRACT * i;
            T_end = T_READY + T_REST * i + T_CONTRACT * (i + 1);

            if (T_start <= time && time < T_end) {
                state_idx = Target_motion_list[i];
                break;
            }
        }
    }

    return state_idx;
}

void MainWindow::Set_Motion_Trajectory(int mode) {
    // Clear the previous trajectories
    trj_shd_X.clear();
    trj_shd_Y.clear();
    trj_shd_Z.clear();
    trj_elb_X.clear();
    trj_wrist_X.clear();
    trj_wrist_Y.clear();
    trj_wrist_Z.clear();
    trj_hand.clear();

    std::vector<double> temp1; temp1.swap(trj_shd_X);
    std::vector<double> temp2; temp2.swap(trj_shd_Y);
    std::vector<double> temp3; temp3.swap(trj_shd_Z);
    std::vector<double> temp4; temp4.swap(trj_elb_X);
    std::vector<double> temp5; temp5.swap(trj_wrist_X);
    std::vector<double> temp6; temp6.swap(trj_wrist_Y);
    std::vector<double> temp7; temp7.swap(trj_wrist_Z);
    std::vector<double> temp8; temp8.swap(trj_hand);

    if (mode == 0) {        // Training
        // Fill up the vector
        for (int i = 0; i < (int)((T_READY + N_MOTIONS * T_CONTRACT + (N_MOTIONS - 1) * T_REST + T_RESIDUAL) * FS); i++) {
            trj_shd_X.push_back(0.0);
            trj_shd_axis.push_back(0.0);
            trj_shd_Y.push_back(0.0);
            trj_shd_Z.push_back(0.0);
            trj_elb_X.push_back(90.0);
            trj_wrist_X.push_back(10.0);
            trj_wrist_Y.push_back(0.0);
            trj_wrist_Z.push_back(15.0);
            trj_hand.push_back(-10.0);
        }
        // std::cout << trj_shd_X.size() << std::endl;

        //////////////////////////////////// Hand & Wrist Motions ////////////////////////////////////
        for (int M = 0; M < N_MOTIONS; M++) {
            double T_start_contract = T_READY + T_REST * M + T_CONTRACT * M;
            double T_end_contract   = T_READY + T_REST * M + T_CONTRACT * M + T_MARGIN_START;
            double T_start_release  = T_READY + T_REST * M + T_CONTRACT * (M + 1);
            double T_end_release    = T_READY + T_REST * M + T_CONTRACT * (M + 1) + T_MARGIN_END;
            // std::cout << T_start_contract << " " << T_end_contract << " " << T_start_release << " " << T_end_release << std::endl;

            // Contraction
            for (int t = (int)(T_start_contract * FS); t < (int)(T_end_contract * FS); t++) {
                if (M == 0) {               // Wrist Flexion
                    trj_wrist_Z[t] = 15.0 - WF_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 1) {          // Wrist Extension
                    trj_wrist_Z[t] = 15.0 + WE_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 2) {          // Radial Deviation
                    trj_wrist_X[t] = 10.0 + RD_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 3) {          // Ulnar Deviation
                    trj_wrist_X[t] = 10.0 - UD_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 4) {          // Radial Deviation
                    trj_wrist_Y[t] = 0.0 + WP_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 5) {          // Ulnar Deviation
                    trj_wrist_Y[t] = 0.0 - WS_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 6) {          // Radial Deviation
                    trj_hand[t] = -10.0 - HC_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (M == 7) {          // Ulnar Deviation
                    trj_hand[t] = -10.0 + HO_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
            }

            // Steady state
            for (int t = (int)(T_end_contract * FS); t < (int)(T_start_release * FS); t++) {
                if (M == 0) {               // Wrist Flexion
                    trj_wrist_Z[t] = 15.0 - WF_ROM;
                }
                else if (M == 1) {          // Wrist Extension
                    trj_wrist_Z[t] = 15.0 + WE_ROM;
                }
                else if (M == 2) {          // Radial Deviation
                    trj_wrist_X[t] = 10.0 + RD_ROM;
                }
                else if (M == 3) {          // Ulnar Deviation
                    trj_wrist_X[t] = 10.0 - UD_ROM;
                }
                else if (M == 4) {          // Radial Deviation
                    trj_wrist_Y[t] = 0.0 + WP_ROM;
                }
                else if (M == 5) {          // Ulnar Deviation
                    trj_wrist_Y[t] = 0.0 - WS_ROM;
                }
                else if (M == 6) {          // Radial Deviation
                    trj_hand[t] = -10.0 - HC_ROM;
                }
                else if (M == 7) {          // Ulnar Deviation
                    trj_hand[t] = -10.0 + HO_ROM;
                }
            }

            // Release
            for (int t = (int)(T_start_release * FS); t < (int)(T_end_release * FS); t++) {
                if (M == 0) {               // Wrist Flexion
                    trj_wrist_Z[t] = 15.0 - WF_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 1) {          // Wrist Extension
                    trj_wrist_Z[t] = 15.0 + WE_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 2) {          // Radial Deviation
                    trj_wrist_X[t] = 10.0 + RD_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 3) {          // Ulnar Deviation
                    trj_wrist_X[t] = 10.0 - UD_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 4) {          // Radial Deviation
                    trj_wrist_Y[t] = 0.0 + WP_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 5) {          // Ulnar Deviation
                    trj_wrist_Y[t] = 0.0 - WS_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 6) {          // Radial Deviation
                    trj_hand[t] = -10.0 - HC_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (M == 7) {          // Ulnar Deviation
                    trj_hand[t] = -10.0 + HO_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
            }
        }
    }
    else if (mode == 1) {   // Unlabeled DAQ
        // Fill up the vector
        for (int i = 0; i < (int)((T_READY + N_REPEAT * T_CONTRACT
                                  + (N_REPEAT - 1) * T_REST + T_RESIDUAL) * FS); i++) {
            trj_shd_X.push_back(0.0);
            trj_shd_axis.push_back(0.0);
            trj_shd_Y.push_back(0.0);
            trj_shd_Z.push_back(0.0);
            trj_elb_X.push_back(90.0);
            trj_wrist_X.push_back(10.0);
            trj_wrist_Y.push_back(0.0);
            trj_wrist_Z.push_back(15.0);
            trj_hand.push_back(-10.0);
        }
        // std::cout << trj_shd_X.size() << std::endl;

        //////////////////////////////////// Hand & Wrist Motions ////////////////////////////////////
        for (int i = 0; i < N_REPEAT; i++) {
            double T_start_contract = T_READY + T_REST * i + T_CONTRACT * i;
            double T_end_contract   = T_READY + T_REST * i + T_CONTRACT * i + T_MARGIN_START;
            double T_start_release  = T_READY + T_REST * i + T_CONTRACT * (i + 1);
            double T_end_release    = T_READY + T_REST * i + T_CONTRACT * (i + 1) + T_MARGIN_END;
            // std::cout << T_start_contract << " " << T_end_contract << " " << T_start_release << " " << T_end_release << std::endl;

            // Contraction
            for (int t = (int)(T_start_contract * FS); t < (int)(T_end_contract * FS); t++) {
                if (Target_motion_list[i] == 1) {               // Wrist Flexion
                    trj_wrist_Z[t] = 15.0 - WF_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 2) {          // Wrist Extension
                    trj_wrist_Z[t] = 15.0 + WE_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 3) {          // Radial Deviation
                    trj_wrist_X[t] = 10.0 + RD_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 4) {          // Ulnar Deviation
                    trj_wrist_X[t] = 10.0 - UD_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 5) {          // Radial Deviation
                    trj_wrist_Y[t] = 0.0 + WP_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 6) {          // Ulnar Deviation
                    trj_wrist_Y[t] = 0.0 - WS_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 7) {          // Radial Deviation
                    trj_hand[t] = -10.0 - HC_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
                else if (Target_motion_list[i] == 8) {          // Ulnar Deviation
                    trj_hand[t] = -10.0 + HO_ROM * (float)((t - (int)(T_start_contract * FS) + 1) * TS)
                                                                / (float)(T_MARGIN_START);
                }
            }

            // Steady state
            for (int t = (int)(T_end_contract * FS); t < (int)(T_start_release * FS); t++) {
                if (Target_motion_list[i] == 1) {               // Wrist Flexion
                    trj_wrist_Z[t] = 15.0 - WF_ROM;
                }
                else if (Target_motion_list[i] == 2) {          // Wrist Extension
                    trj_wrist_Z[t] = 15.0 + WE_ROM;
                }
                else if (Target_motion_list[i] == 3) {          // Radial Deviation
                    trj_wrist_X[t] = 10.0 + RD_ROM;
                }
                else if (Target_motion_list[i] == 4) {          // Ulnar Deviation
                    trj_wrist_X[t] = 10.0 - UD_ROM;
                }
                else if (Target_motion_list[i] == 5) {          // Radial Deviation
                    trj_wrist_Y[t] = 0.0 + WP_ROM;
                }
                else if (Target_motion_list[i] == 6) {          // Ulnar Deviation
                    trj_wrist_Y[t] = 0.0 - WS_ROM;
                }
                else if (Target_motion_list[i] == 7) {          // Radial Deviation
                    trj_hand[t] = -10.0 - HC_ROM;
                }
                else if (Target_motion_list[i] == 8) {          // Ulnar Deviation
                    trj_hand[t] = -10.0 + HO_ROM;
                }
            }

            // Release
            for (int t = (int)(T_start_release * FS); t < (int)(T_end_release * FS); t++) {
                if (Target_motion_list[i] == 1) {               // Wrist Flexion
                    trj_wrist_Z[t] = 15.0 - WF_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 2) {          // Wrist Extension
                    trj_wrist_Z[t] = 15.0 + WE_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 3) {          // Radial Deviation
                    trj_wrist_X[t] = 10.0 + RD_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 4) {          // Ulnar Deviation
                    trj_wrist_X[t] = 10.0 - UD_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 5) {          // Radial Deviation
                    trj_wrist_Y[t] = 0.0 + WP_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 6) {          // Ulnar Deviation
                    trj_wrist_Y[t] = 0.0 - WS_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 7) {          // Radial Deviation
                    trj_hand[t] = -10.0 - HC_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
                else if (Target_motion_list[i] == 8) {          // Ulnar Deviation
                    trj_hand[t] = -10.0 + HO_ROM * (float)(((int)(T_end_release * FS) - t + 1) * TS)
                                                                / (float)(T_MARGIN_END);
                }
            }
        }
    }
}
