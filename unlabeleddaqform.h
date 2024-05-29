#ifndef UNLABELEDDAQFORM_H
#define UNLABELEDDAQFORM_H

#include <QWidget>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

// Custom headers
#include "scenemodifier.h"

namespace Ui {
class UnlabeledDAQForm;
}

class UnlabeledDAQForm : public QWidget {
    Q_OBJECT

public:
    explicit UnlabeledDAQForm(QWidget *parent = nullptr);
    ~UnlabeledDAQForm();

    Ui::UnlabeledDAQForm *get_UI();

    // 3D arm model
    SceneModifier* get_Model();

private:
    Ui::UnlabeledDAQForm *ui;

    // 3D arm model
    SceneModifier *modifier;
    QWidget* Container_hand;
};

#endif // UNLABELEDDAQFORM_H
