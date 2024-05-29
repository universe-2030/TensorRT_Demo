#ifndef UNLABELEDDAQUSERFORM_H
#define UNLABELEDDAQUSERFORM_H

#include <QWidget>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaService>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QString>
#include <QPushButton>

// Qt headers for 3D rendering
#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

// API headers
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>

// Custom headers
#include "scenemodifier.h"

#define N_MOTIONS 8
#define PIXMAP_WIDTH 400
#define PIXMAP_HEIGHT 400

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QSlider;
class QStatusBar;
class QVideoProbe;
class QVideoWidget;
class QAudioProbe;
QT_END_NAMESPACE

class PlaylistModel;
class HistogramWidget;

namespace Ui {
class UnlabeledDAQUserForm;
}

class UnlabeledDAQUserForm : public QWidget {
    Q_OBJECT

public:
    explicit UnlabeledDAQUserForm(QWidget *parent = nullptr);
    ~UnlabeledDAQUserForm();

    // 3D arm model
    SceneModifier* get_Model();

    // Video player
    bool isPlayerAvailable() const;
    void addToPlaylist(const QList<QUrl> &urls);
    void setCustomAudioRole(const QString &role);

    QMediaPlayer *m_player = nullptr;
    QMediaPlayer *get_QMediaPlayer();
    QMediaPlaylist *get_QMediaPlaylist();
    void positionChange(qint64 progress);

    // 'Get' functions
    QLabel *get_m_MotionImg();

    // 'set' functions

signals:
    void fullScreenChanged(bool fullScreen);

private slots:
    // Video player
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();
    void previousClicked();

    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);

    void displayErrorMessage();
    void showColorDialog();

private:
    Ui::UnlabeledDAQUserForm *ui;

    // Motion image
    cv::Mat* Figures_motion;

    // 3D arm model
    SceneModifier *modifier;
    QWidget* Container_hand;

    // Video player
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);

    QMediaPlaylist *m_playlist = nullptr;
    QVideoWidget *m_videoWidget = nullptr;
    QLabel *m_coverLabel = nullptr;
    QSlider *m_slider = nullptr;
    QLabel *m_labelDuration = nullptr;
    QPushButton *m_fullScreenButton = nullptr;
    QPushButton *m_colorButton = nullptr;
    QDialog *m_colorDialog = nullptr;
    QLabel *m_statusLabel = nullptr;
    QStatusBar *m_statusBar = nullptr;

    QLabel *m_labelHistogram = nullptr;
    HistogramWidget *m_videoHistogram = nullptr;
    HistogramWidget *m_audioHistogram = nullptr;
    QVideoProbe *m_videoProbe = nullptr;
    QAudioProbe *m_audioProbe = nullptr;

    PlaylistModel *m_playlistModel = nullptr;
    QAbstractItemView *m_playlistView = nullptr;
    QString m_trackInfo;
    QString m_statusInfo;
    qint64 m_duration;

    QLabel *m_MotionImg;
};

#endif // UNLABELEDDAQUSERFORM_H
