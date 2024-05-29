#ifndef PAINTTESTONE_H
#define PAINTTESTONE_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QString>
#include <QDialog>

#include <iostream>
#include <string>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 800

#define TRIANGLE_WIDTH 20
#define TRIANGLE_HEIGHT 20

#define N_TRAJ_PRACTICE 1
#define N_TRAJ_TEST 1

#define T_READY_BALL_CTR 5.0
#define T_MAX_BALL_CTR_PRACTICE 75.0
#define T_MAX_BALL_CTR 75.0
#define T_DURATION_SUCCESS 5.0

#define N_POS 5
#define N_TRAJECTORY 2

class PaintTestOne : public QObject{
    Q_OBJECT

public:
    PaintTestOne();

public:
    void paint(QPainter *painter, QPaintEvent *event);

    // Initialization
    void Initialize_Cursor(int trial_idx);
    void Initialize_Variables();
    void Initialize_DAVariables();

    // Template
    void CountDown(QPainter *painter); // Countdown for 5 seconds
    void Traj_test(QPainter *painter); // Trajectory - test

    // Current state
    void Current(QPainter *painter);

    // 'Get' Functions
    double Get_time();
    double Get_Cur_Scale();
    double Get_Cur_X();
    double Get_Cur_Y();

    double Get_Start_Scale_Practice();
    double Get_Start_X_Practice();
    double Get_Start_Y_Practice();

    double Get_Start_Scale(int trial_idx);
    double Get_Start_X(int trial_idx);
    double Get_Start_Y(int trial_idx);

    double Get_End_Scale(int trial_idx);
    double Get_End_X(int trial_idx);
    double Get_End_Y(int trial_idx);

    // 'Set' Functions
    void Set_time(double m_time);
    void Set_Cur_Scale(double Cur_Scale);
    void Set_Cur_X(double Cur_X);
    void Set_Cur_Y(double Cur_Y);

    void Set_Trial_idx(int trial_idx);
    void Set_Traj_idx(int traj_idx);
    void Set_PracticeOrNot(bool _PracticeOrNot);
    void Set_isSuccess(bool _isSuccess);
    void Set_isTimeOver(bool _isTimeOver);
    void Set_isRun(bool _isRun);

    void Set_T_Trial_Start(double m_time);
    void Set_T_Trial_End(double m_time);

private:
    // Limb pos & Trial idx
    int Trial_idx = 1;
    int Traj_idx = 0;

    // Boolean representing for success
    bool PracticeOrNot = true;
    bool isSuccess = false;
    bool isTimeOver = false;
    bool isRun = false;

    // Variables
    double time = 0.0;
    double T_trial_start = 0.0;
    double T_trial_end = 0.0;

    // Painting variables
    QBrush background;
    QBrush arrowBrush;
    QBrush startPolyBrush;
    QBrush endPolyBrush;
    QBrush curPolyBrush;

    QFont textFont;
    QFont cntdownFont;
    QFont timerFont;
    QFont successFont;
    QFont timeoverFont;

    QPen linePen;
    QPen arrowPen;
    QPen startPolyPen;
    QPen endPolyPen;
    QPen curPolyPen;
    QPen textPen;
    QPen cntdownPen;
    QPen timerPen;
    QPen successPen;
    QPen timeoverPen;

    // Trajectory variables
    double Start_Scale_Practice;
    double Start_X_Practice;
    double Start_Y_Practice;

    double* Start_Scale;
    double* Start_X;
    double* Start_Y;

    double Traj_X_1;
    double Traj_Y_1;

    double Traj_X_2;
    double Traj_Y_2;

    double Traj_X_3;
    double Traj_Y_3;

    double Traj_X_4;
    double Traj_Y_4;

    double Traj_X_5;
    double Traj_Y_5;

    double Traj_X_6;
    double Traj_Y_6;

    double Traj_X_7;
    double Traj_Y_7;

    double* End_Rot;
    double* End_Scale;
    double* End_X;
    double* End_Y;

    // Current state variables
    double Cur_Scale;
    double Cur_X;
    double Cur_Y;
};

#endif // PAINTTESTONE_H
