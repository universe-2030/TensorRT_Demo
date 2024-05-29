#include "dadialog.h"

DADialog::DADialog(QWidget *parent)
    : QWidget(parent) {
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x000000))); // Set background color

    // Import 3D rendering QWidget to ui QWidget
    m_3DRender = new QWidget(this);
    w_hand = m_3DRender->width();
    h_hand = m_3DRender->height();
    Container_hand = QWidget::createWindowContainer(view,
                                                    m_3DRender,
                                                    Qt::Widget);
    Container_hand->resize(w_hand, h_hand);
//    QSize screenSize = view->screen()->size();
//    container->setMinimumSize(QSize(200, 100));
//    container->setMaximumSize(screenSize);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera setup
    Qt3DRender::QCamera *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(20.0f, 2.0f, 0.0f));
    cameraEntity->setUpVector(QVector3D(-1.0, 1.713, 0));
    cameraEntity->setViewCenter(QVector3D(0, -2.0f, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // For camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Scenemodifier
    SceneModifier *modifier = new SceneModifier(rootEntity);

    // Set root object of the scene
    view->setRootEntity(rootEntity);
}

DADialog::~DADialog() {

}
