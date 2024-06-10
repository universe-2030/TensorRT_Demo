#ifndef TESTONEFORM_H
#define TESTONEFORM_H

#include <QWidget>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

namespace Ui {
class TestOneForm;
}

class TestOneForm : public QWidget {
    Q_OBJECT

public:
    explicit TestOneForm(QWidget *parent = nullptr);
    ~TestOneForm();

    Ui::TestOneForm *get_UI();

private:
    Ui::TestOneForm *ui;
};

#endif // TESTONEFORM_H
