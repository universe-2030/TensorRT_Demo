#include "painttestone.h"
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QDebug>

PaintTestOne::PaintTestOne() {
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    //! Initialization
    Initialize_Variables();
    Initialize_DAVariables();
    Initialize_Cursor(Traj_idx);

    //! Brush assignment
        background = QBrush(QColor(220, 220, 255));
        arrowBrush = QBrush(Qt::SolidPattern);
        startPolyBrush = QBrush(Qt::SolidPattern);
        startPolyBrush.setColor(QColor(255, 0, 0, 150));
        endPolyBrush = QBrush(Qt::SolidPattern);
        endPolyBrush.setColor(QColor(0, 0, 255, 150));
        curPolyBrush = QBrush(Qt::SolidPattern);
        curPolyBrush.setColor(QColor(0, 0, 0, 120));
    //!

    //! Pen assignment
        linePen = QPen(Qt::black);
        linePen.setWidth(5);
        linePen.setStyle(Qt::DashLine);
        arrowPen = QPen(Qt::black);
        arrowPen.setWidth(3);
        arrowPen.setStyle(Qt::SolidLine);
        startPolyPen = QPen(QColor(255, 0, 0, 150)); // R, G, B, alpha
        endPolyPen = QPen(QColor(0, 0, 255, 150));
        curPolyPen = QPen(QColor(0, 0, 0, 120));
        textPen = QPen(Qt::black);
        cntdownPen = QPen(QColor(0, 0, 255));
        timerPen = QPen(QColor(0, 0, 0));
        successPen = QPen(QColor(0, 0, 255));
        timeoverPen = QPen(QColor(255, 0, 0));
    //!

    //! Font assignment
        textFont.setPixelSize(20);
        textFont.setBold(true);
        cntdownFont.setPixelSize(300);
        cntdownFont.setBold(true);
        timerFont.setPixelSize(200);
        timerFont.setBold(true);
        successFont.setPixelSize(75);
        successFont.setBold(true);
        timeoverFont.setPixelSize(75);
        timeoverFont.setBold(true);
    //!

    //! Time variable assignment
}

void PaintTestOne::Initialize_Cursor(int traj_idx) {
    if (traj_idx == 0) {
        Cur_X = Get_Start_X_Practice();
        Cur_Y = Get_Start_Y_Practice();
        Cur_Scale = Get_Start_Scale_Practice();
    }
    else {
        Cur_X = Get_Start_X(traj_idx);
        Cur_Y = Get_Start_Y(traj_idx);
        Cur_Scale = Get_Start_Scale(traj_idx);
    }
}

void PaintTestOne::Initialize_Variables() {
    // Trajectory - practice
    Start_X_Practice = 800;
    Start_Y_Practice = 400;
    Start_Scale_Practice = 1.2;
}

void PaintTestOne::Initialize_DAVariables() {
    Start_X = new double[N_TRAJ_TEST];
    Start_Y = new double[N_TRAJ_TEST];
    Start_Scale = new double[N_TRAJ_TEST];

    End_X = new double[N_TRAJ_TEST];
    End_Y = new double[N_TRAJ_TEST];
    End_Scale = new double[N_TRAJ_TEST];

    // 1. Trajectory - test 1
    Start_X[0] = 100;
    Start_Y[0] = 100;
    Start_Scale[0] = 1.2;

    End_X[0] = 350;
    End_Y[0] = 300;
    End_Scale[0] = 1.9;
}

void PaintTestOne::paint(QPainter *painter, QPaintEvent *event) {
    painter->fillRect(event->rect(), background);

    // 1. Count-down
    if ((time - T_trial_start) <= T_READY_BALL_CTR)
        CountDown(painter);

    // 2. Trajectory rendering
    if (!PracticeOrNot) {
        Traj_test(painter);
    }

    // 3. Current state rendering
    Current(painter);
}

// Template
void PaintTestOne::CountDown(QPainter *painter) {
    //! Time notification
        painter->setPen(cntdownPen);
        painter->setFont(cntdownFont);

        painter->translate(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        QString temp = QString::fromStdString(std::to_string((int)((T_READY_BALL_CTR + 0.999)
                                                                        - (time - T_trial_start))));
        painter->drawText(QRect(-150, -150, 300, 300),
                          Qt::AlignCenter, temp);
        painter->translate(-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2);
    //!
}

void PaintTestOne::Traj_test(QPainter *painter) {
    int traj_idx = 0;

    //! 0. Trajectory point assignment
        Traj_X_1 = 100;
        Traj_Y_1 = 700;

        Traj_X_2 = 850;
        Traj_Y_2 = 700;

        Traj_X_3 = 850;
        Traj_Y_3 = 300;
    //!

    //! 1. 'Start' location
        painter->setPen(textPen);
        painter->setFont(textFont);
        painter->translate(Start_X[traj_idx], Start_Y[traj_idx]);
        painter->drawText(QRect(-25, -25, 50, 50),
                          Qt::AlignCenter, QStringLiteral("Start"));
        painter->translate(-Start_X[traj_idx], -Start_Y[traj_idx]);
    //!

    //! 2. Initial orientation
        painter->setPen(startPolyPen);
        painter->setBrush(startPolyBrush);

        // Translate & Rotate
        painter->translate(Start_X[traj_idx], Start_Y[traj_idx]);
        QRect rect1(-30 * Start_Scale[traj_idx], -30 * Start_Scale[traj_idx],
                    60 * Start_Scale[traj_idx], 60 * Start_Scale[traj_idx]);
        painter->drawEllipse(rect1);
        painter->translate(-Start_X[traj_idx], -Start_Y[traj_idx]);
    //!

    //! 3. 1st trajectory
        painter->setPen(linePen);
        painter->drawLine(Start_X[traj_idx], Start_Y[traj_idx] - 20, Traj_X_1, Traj_Y_1);

        painter->setPen(arrowPen);
        painter->setBrush(arrowBrush);
        QPoint arrows_1st[3] = {
                QPoint(Traj_X_1, Traj_Y_1 + TRIANGLE_HEIGHT / 2),
                QPoint(Traj_X_1 - TRIANGLE_WIDTH / 2, Traj_Y_1 - TRIANGLE_HEIGHT / 2),
                QPoint(Traj_X_1 + TRIANGLE_WIDTH / 2, Traj_Y_1 - TRIANGLE_HEIGHT / 2)
            };
        painter->drawPolygon(arrows_1st, 3, Qt::WindingFill);
    //!

    //! 4. 2nd trajectory
        painter->setPen(linePen);
        painter->drawLine(Traj_X_1, Traj_Y_1, Traj_X_2, Traj_Y_2);

        painter->setPen(arrowPen);
        painter->setBrush(arrowBrush);
        QPoint arrows_2nd[3] = {
                QPoint(Traj_X_2 + TRIANGLE_WIDTH / 2, Traj_Y_2),
                QPoint(Traj_X_2 - TRIANGLE_WIDTH / 2, Traj_Y_2 - TRIANGLE_HEIGHT / 2),
                QPoint(Traj_X_2 - TRIANGLE_WIDTH / 2, Traj_Y_2 + TRIANGLE_HEIGHT / 2)
            };
        painter->drawPolygon(arrows_2nd, 3, Qt::WindingFill);
    //!

    //! 5. 3rd trajectory
        painter->setPen(linePen);
        painter->drawLine(Traj_X_2, Traj_Y_2, Traj_X_3, Traj_Y_3);

        painter->setPen(arrowPen);
        painter->setBrush(arrowBrush);
        QPoint arrows_3rd[3] = {
                QPoint(Traj_X_3, Traj_Y_3 - TRIANGLE_HEIGHT / 2),
                QPoint(Traj_X_3 - TRIANGLE_WIDTH / 2, Traj_Y_3 + TRIANGLE_HEIGHT / 2),
                QPoint(Traj_X_3 + TRIANGLE_WIDTH / 2, Traj_Y_3 + TRIANGLE_HEIGHT / 2)
            };
        painter->drawPolygon(arrows_3rd, 3, Qt::WindingFill);
    //!

    //! 6. 4th trajectory
        painter->setPen(linePen);
        painter->drawLine(Traj_X_3, Traj_Y_3, End_X[traj_idx], End_Y[traj_idx]);

        painter->setPen(arrowPen);
        painter->setBrush(arrowBrush);
        QPoint arrows_4th[3] = {
                QPoint(End_X[traj_idx] - TRIANGLE_WIDTH / 2, End_Y[traj_idx]),
                QPoint(End_X[traj_idx] + TRIANGLE_WIDTH / 2, End_Y[traj_idx] - TRIANGLE_HEIGHT / 2),
                QPoint(End_X[traj_idx] + TRIANGLE_WIDTH / 2, End_Y[traj_idx] + TRIANGLE_HEIGHT / 2)
            };
        painter->drawPolygon(arrows_4th, 3, Qt::WindingFill);
    //!

    //! 7. Final orientation
        painter->setPen(endPolyPen);
        painter->setBrush(endPolyBrush);

        // Translate & Rotate
        painter->translate(End_X[traj_idx], End_Y[traj_idx]);
        QRect rect = QRect(-30 * End_Scale[traj_idx], -30 * End_Scale[traj_idx],
                    60 * End_Scale[traj_idx], 60 * End_Scale[traj_idx]);
        painter->drawEllipse(rect);
        painter->translate(-End_X[traj_idx], -End_Y[traj_idx]);
    //!

    //! 8. 'End' location
        painter->setPen(textPen);
        painter->setFont(textFont);
        painter->translate(End_X[traj_idx], End_Y[traj_idx]);
        painter->drawText(QRect(-25, -25, 50, 50), Qt::AlignCenter, QStringLiteral("End"));
        painter->translate(-End_X[traj_idx], -End_Y[traj_idx]);
    //!
}

void PaintTestOne::Current(QPainter *painter) {
    //! Current location & orientation
        painter->setPen(curPolyPen);
        painter->setBrush(curPolyBrush);

        // Translate & Rotate
        painter->translate(Cur_X, Cur_Y);
        QRect rect1(-30 * Cur_Scale, -30 * Cur_Scale,
                    60 * Cur_Scale, 60 * Cur_Scale);
        painter->drawEllipse(rect1);
        painter->translate(-Cur_X, -Cur_Y);
    //!
}

//////////////////////////////////////// 'Get' functions ////////////////////////////////////////
double PaintTestOne::Get_Cur_Scale() {
    return Cur_Scale;
}

double PaintTestOne::Get_Cur_X() {
    return Cur_X;
}

double PaintTestOne::Get_Cur_Y() {
    return Cur_Y;
}

double PaintTestOne::Get_Start_Scale_Practice() {
    return Start_Scale_Practice;
}

double PaintTestOne::Get_Start_X_Practice() {
    return Start_X_Practice;
}

double PaintTestOne::Get_Start_Y_Practice() {
    return Start_Y_Practice;
}

double PaintTestOne::Get_Start_Scale(int traj_idx) {
    return Start_Scale[traj_idx - 1];
}

double PaintTestOne::Get_Start_X(int traj_idx) {
    return Start_X[traj_idx - 1];
}

double PaintTestOne::Get_Start_Y(int traj_idx) {
    return Start_Y[traj_idx - 1];
}

double PaintTestOne::Get_End_Scale(int traj_idx) {
    return End_Scale[traj_idx - 1];
}

double PaintTestOne::Get_End_X(int traj_idx) {
    return End_X[traj_idx - 1];
}

double PaintTestOne::Get_End_Y(int traj_idx) {
    return End_Y[traj_idx - 1];
}

//////////////////////////////////////// 'Set' functions ////////////////////////////////////////
void PaintTestOne::Set_time(double m_time) {
    time = m_time;
}

void PaintTestOne::Set_T_Trial_Start(double m_time) {
    T_trial_start = m_time;
}

void PaintTestOne::Set_T_Trial_End(double m_time) {
    T_trial_end = m_time;
}

void PaintTestOne::Set_Cur_Scale(double m_Cur_Scale) {
    Cur_Scale = m_Cur_Scale;
}

void PaintTestOne::Set_Cur_X(double m_Cur_X) {
    Cur_X = m_Cur_X;
}

void PaintTestOne::Set_Cur_Y(double m_Cur_Y) {
    Cur_Y = m_Cur_Y;
}

void PaintTestOne::Set_Trial_idx(int trial_idx) {
    Trial_idx = trial_idx;
}

void PaintTestOne::Set_Traj_idx(int traj_idx) {
    Traj_idx = traj_idx;
}

void PaintTestOne::Set_PracticeOrNot(bool _PracticeOrNot) {
    PracticeOrNot = _PracticeOrNot;
}

void PaintTestOne::Set_isRun(bool _isRun) {
    isRun = _isRun;
}
