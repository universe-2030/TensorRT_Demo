#ifndef OPENGLTESTONE_H
#define OPENGLTESTONE_H

#include <QOpenGLWidget>

class PaintTestOne;

class OpenGLTestOne : public QOpenGLWidget {
    Q_OBJECT

public:
    OpenGLTestOne(PaintTestOne *painttestone, QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    PaintTestOne *painttestone;
};

#endif // OPENGLTESTONE_H
