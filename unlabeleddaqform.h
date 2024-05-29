#ifndef UNLABELEDDAQFORM_H
#define UNLABELEDDAQFORM_H

#include <QWidget>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

namespace Ui {
class UnlabeledDAQForm;
}

class UnlabeledDAQForm : public QWidget {
    Q_OBJECT

public:
    explicit UnlabeledDAQForm(QWidget *parent = nullptr);
    ~UnlabeledDAQForm();

    Ui::UnlabeledDAQForm *get_UI();

private:
    Ui::UnlabeledDAQForm *ui;
};

#endif // UNLABELEDDAQFORM_H
