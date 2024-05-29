#ifndef DADIALOG_H
#define DADIALOG_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

#include "scenemodifier.h"

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QStatusBar;
class QVideoProbe;
class QVideoWidget;
class QAudioProbe;
QT_END_NAMESPACE

class DADialog : public QWidget {
    Q_OBJECT

public:
    explicit DADialog(QWidget *parent = nullptr);
    ~DADialog();

private:
    QWidget *m_3DRender = nullptr;

    // Variables for hand-rendering Qwidget
    int h_hand;
    int w_hand;
    QWidget* Container_hand;
};

#endif // DADIALOG_H
