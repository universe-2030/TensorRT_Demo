#ifndef TRAININGFORM_H
#define TRAININGFORM_H

#include <QWidget>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

// Custom headers
#include "scenemodifier.h"

namespace Ui {
class TrainingForm;
}

class TrainingForm : public QWidget {
    Q_OBJECT

public:
    explicit TrainingForm(QWidget *parent = nullptr);
    ~TrainingForm();

    Ui::TrainingForm *get_UI();

    // 3D arm model
    SceneModifier* get_Model();

private:
    Ui::TrainingForm *ui;

    // 3D arm model
    SceneModifier *modifier;
    QWidget* Container_hand;
};

#endif // TRAININGFORM_H
