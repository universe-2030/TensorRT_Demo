#ifndef TESTONEFORM_H
#define TESTONEFORM_H

#include <QWidget>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

// Custom headers
#include "scenemodifier.h"

namespace Ui {
class TestOneForm;
}

class TestOneForm : public QWidget {
    Q_OBJECT

public:
    explicit TestOneForm(QWidget *parent = nullptr);
    ~TestOneForm();

    Ui::TestOneForm *get_UI();

    // 3D arm model
    SceneModifier* get_Model();

private:
    Ui::TestOneForm *ui;

    // 3D arm model
    SceneModifier *modifier;
    QWidget* Container_hand;
};

#endif // TESTONEFORM_H
