#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include "testoneuserform.h"

void TestOneUserForm::keyPressEvent(QKeyEvent* event) {
    // Virtual controller to check the ball control session
    double X_val_inc = 5;
    double Y_val_inc = 5;
    double Rot_val_inc = 0.5;
    double Scale_val_inc = 0.01;
    double X_val, Y_val, Rot_val, Scale_val;

    switch(event->key()) {
        case Qt::Key_Left:
            X_val = Get_PaintTestOne()->Get_Cur_X();
            Get_PaintTestOne()->Set_Cur_X(X_val - X_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_X() << std::endl;
            break;
        case Qt::Key_Right:
            X_val = Get_PaintTestOne()->Get_Cur_X();
            Get_PaintTestOne()->Set_Cur_X(X_val + X_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_X() << std::endl;
            break;
        case Qt::Key_Up:
            Y_val = Get_PaintTestOne()->Get_Cur_Y();
            Get_PaintTestOne()->Set_Cur_Y(Y_val - Y_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_Y() << std::endl;
            break;
        case Qt::Key_Down:
            Y_val = Get_PaintTestOne()->Get_Cur_Y();
            Get_PaintTestOne()->Set_Cur_Y(Y_val + Y_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_Y() << std::endl;
            break;
        case Qt::Key_A:
            Rot_val = Get_PaintTestOne()->Get_Cur_Rot();
            Get_PaintTestOne()->Set_Cur_Rot(Rot_val + Rot_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_Rot() << std::endl;
            break;
        case Qt::Key_D:
            Rot_val = Get_PaintTestOne()->Get_Cur_Rot();
            Get_PaintTestOne()->Set_Cur_Rot(Rot_val - Rot_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_Rot() << std::endl;
            break;
        case Qt::Key_W:
            Scale_val = Get_PaintTestOne()->Get_Cur_Scale();
            Get_PaintTestOne()->Set_Cur_Scale(Scale_val + Scale_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_Scale() << std::endl;
            break;
        case Qt::Key_S:
            Scale_val = Get_PaintTestOne()->Get_Cur_Scale();
            Get_PaintTestOne()->Set_Cur_Scale(Scale_val - Scale_val_inc);
//            std::cout << Get_PaintTestOne()->Get_Cur_Scale() << std::endl;
            break;
    }
}

TestOneUserForm::TestOneUserForm(QWidget *parent)
    : QWidget(parent) {
    painttestone = new PaintTestOne;

    // Move and resize the window
    QDesktopWidget widget;
    QRect screenGeometry = widget.availableGeometry(widget.primaryScreen());
    int width = screenGeometry.width();
    int height = screenGeometry.height();
    this->move((width - 1700) / 2.0 + 1920 + 40,
               (height - 850) / 2.0 - 15);

    OpenGLTestOne *opengltestone = new OpenGLTestOne(painttestone, this);
    QLabel *openglLabel = new QLabel(tr("Ball control session"));
    openglLabel->setAlignment(Qt::AlignHCenter);
    QFont font = openglLabel->font();
    font.setPointSize(20);
    font.setBold(true);
    openglLabel->setFont(font);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(opengltestone, 5);
    layout->addWidget(openglLabel, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, opengltestone, &OpenGLTestOne::animate);
    timer->start(30);
}

TestOneUserForm::~TestOneUserForm() {}

PaintTestOne* TestOneUserForm::Get_PaintTestOne() {
    return painttestone;
}
