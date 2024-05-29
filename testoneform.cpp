#include "testoneform.h"
#include "ui_testoneform.h"

TestOneForm::TestOneForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestOneForm) {
    ui->setupUi(this);

    /////////////////////////////////// OpenGL widget ///////////////////////////////////
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x000000))); // Background Color
    Container_hand = QWidget::createWindowContainer(view,
                                                    ui->wdg_Upper_limb,
                                                    Qt::Widget);
    double w_hand = ui->wdg_Upper_limb->width();
    double h_hand = ui->wdg_Upper_limb->height();
    Container_hand->resize(w_hand, h_hand);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera setup
    Qt3DRender::QCamera *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(12.5f, 6.25f, 6.0f));      // Position (12.5, 6.25, 6.0)
    cameraEntity->setUpVector(QVector3D(0.5f, 2.2f, 0.3f));        // Up Vector (0.5, 2.2, 0.3)
    cameraEntity->setViewCenter(QVector3D(0.5f, 0.0f, -1.5f));     // View Center (0.5, 0.0, -1.5)

    // Light setup
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.5);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // Interactive camera control
    Qt3DExtras::QFirstPersonCameraController *camController =
                    new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Scenemodifier
    modifier = new SceneModifier(rootEntity);

    // Set root object of the scene
    view->setRootEntity(rootEntity);
}

TestOneForm::~TestOneForm() {
    delete ui;
}

Ui::TestOneForm* TestOneForm::get_UI() {
    return ui;
}

SceneModifier* TestOneForm::get_Model() {
    return modifier;
}
