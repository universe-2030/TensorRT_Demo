#ifndef TESTONEUSERFORM_H
#define TESTONEUSERFORM_H

#include <QWidget>
#include <QDesktopWidget>
#include <QKeyEvent>

#include <iostream>
#include "painttestone.h"
#include "opengltestone.h"

namespace Ui {
class TestOneUserForm;
}

class TestOneUserForm : public QWidget {
    Q_OBJECT

protected:
    void keyPressEvent(QKeyEvent* event);

public:
    explicit TestOneUserForm(QWidget *parent = nullptr);
    ~TestOneUserForm();

    PaintTestOne* Get_PaintTestOne();

private:
    PaintTestOne* painttestone;
};

#endif // TESTONEUSERFORM_H
