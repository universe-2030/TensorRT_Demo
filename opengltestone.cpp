#include "opengltestone.h"
#include "painttestone.h"

#include <QPainter>
#include <QTimer>

OpenGLTestOne::OpenGLTestOne(PaintTestOne *painttestone, QWidget *parent)
    : QOpenGLWidget(parent), painttestone(painttestone) {
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setAutoFillBackground(false);
}

void OpenGLTestOne::animate() {
    update();
}

void OpenGLTestOne::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painttestone->paint(&painter, event);
    painter.end();
}
