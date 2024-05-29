#ifndef TRAININGFORM_H
#define TRAININGFORM_H

#include <QWidget>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

namespace Ui {
class TrainingForm;
}

class TrainingForm : public QWidget {
    Q_OBJECT

public:
    explicit TrainingForm(QWidget *parent = nullptr);
    ~TrainingForm();

    Ui::TrainingForm *get_UI();

private:
    Ui::TrainingForm *ui;
};

#endif // TRAININGFORM_H
