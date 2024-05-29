#include "traininguserform.h"
#include "ui_traininguserform.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "histogramwidget.h"
#include "videowidget.h"

TrainingUserForm::TrainingUserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingUserForm) {
    ui->setupUi(this);

    // Figure container assignment
    Figures_motion = new cv::Mat[N_MOTIONS + 1];
    Figures_motion[0] = cv::imread("../TRT_demo/Figures/Motions/Rest.png", cv::IMREAD_UNCHANGED);
    Figures_motion[1] = cv::imread("../TRT_demo/Figures/Motions/Wrist Flexion.png", cv::IMREAD_UNCHANGED);
    Figures_motion[2] = cv::imread("../TRT_demo/Figures/Motions/Wrist Extension.png", cv::IMREAD_UNCHANGED);
    Figures_motion[3] = cv::imread("../TRT_demo/Figures/Motions/Radial Deviation.png", cv::IMREAD_UNCHANGED);
    Figures_motion[4] = cv::imread("../TRT_demo/Figures/Motions/Ulnar Deviation.png", cv::IMREAD_UNCHANGED);
    Figures_motion[5] = cv::imread("../TRT_demo/Figures/Motions/Wrist Pronation.png", cv::IMREAD_UNCHANGED);
    Figures_motion[6] = cv::imread("../TRT_demo/Figures/Motions/Wrist Supination.png", cv::IMREAD_UNCHANGED);
    Figures_motion[7] = cv::imread("../TRT_demo/Figures/Motions/Hand Close.png", cv::IMREAD_UNCHANGED);
    Figures_motion[8] = cv::imread("../TRT_demo/Figures/Motions/Hand Open.png", cv::IMREAD_UNCHANGED);

    /////////////////////////////////// OpenGL widget ///////////////////////////////////
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x000000))); // Background Color
    Container_hand = QWidget::createWindowContainer(view,
                                                    ui->wdg_Upper_limb,
                                                    Qt::Widget);

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera setup
    Qt3DRender::QCamera *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(12.5f, 6.25f, 6.0f));      // Position (12.5, 6.25, 6.0)
    cameraEntity->setUpVector(QVector3D(0.5f, 2.2f, 0.3f));        // Up Vector (0.5, 2.2, 0.3)
    cameraEntity->setViewCenter(QVector3D(0.5f, 0.0f, -1.5f));    // View Center (0.5, 0.0, -1.5)

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

    /////////////////////////////////// Image widget ///////////////////////////////////
    m_MotionImg = new QLabel(this);

    /////////////////////////////////// Video widget ///////////////////////////////////
    // Video player
    m_player = new QMediaPlayer(this);
    m_player->setAudioRole(QAudio::VideoRole);
    qInfo() << "Supported audio roles:";
    for (QAudio::Role role : m_player->supportedAudioRoles())
        qInfo() << "    " << role;
    // owned by PlaylistModel
    m_playlist = new QMediaPlaylist();
    m_player->setPlaylist(m_playlist);

    connect(m_player, &QMediaPlayer::durationChanged, this, &TrainingUserForm::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &TrainingUserForm::positionChanged);
    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &TrainingUserForm::metaDataChanged);
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &TrainingUserForm::playlistPositionChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &TrainingUserForm::statusChanged);
    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &TrainingUserForm::bufferingProgress);
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &TrainingUserForm::displayErrorMessage);
    connect(m_player, &QMediaPlayer::stateChanged, this, &TrainingUserForm::stateChanged);

    m_videoWidget = new VideoWidget(this);
    m_player->setVideoOutput(m_videoWidget);

    m_playlistModel = new PlaylistModel(this);
    m_playlistModel->setPlaylist(m_playlist);

    // Arm - layout
    QBoxLayout *armlayout = new QHBoxLayout;
    armlayout->addWidget(Container_hand);

    QFont TextFont("Times", 15, QFont::Bold);

    // Video - layout
    QBoxLayout *videolayout = new QVBoxLayout;
    QLabel *videolabel = new QLabel(this);
    videolabel->setText("Timer");
    videolabel->setAlignment(Qt::AlignHCenter);
    videolabel->setFont(TextFont);
    videolayout->addWidget(m_videoWidget, 10);
    videolayout->addWidget(videolabel, 1);

    // Image - layout
    QBoxLayout *imagelayout = new QVBoxLayout;
    QLabel *imagelabel = new QLabel(this);
    imagelabel->setText("Motion");
    imagelabel->setAlignment(Qt::AlignHCenter);
    imagelabel->setFont(TextFont);
    imagelayout->addWidget(m_MotionImg, 10);
    imagelayout->addWidget(imagelabel, 1);
    imagelayout->setAlignment(Qt::AlignHCenter);

    QBoxLayout *ImageVideoLayout = new QVBoxLayout;
    ImageVideoLayout->addLayout(videolayout, 1);
    ImageVideoLayout->addLayout(imagelayout, 1);

    QBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(armlayout, 3);
    layout->addLayout(ImageVideoLayout, 1);
    setLayout(layout);

    // Motion image setup
    QImage* qimg_motion;
    QPixmap qpix_motion;
    qimg_motion = new QImage((unsigned char *)Figures_motion[0].data,
                              Figures_motion[0].cols, Figures_motion[0].rows,
                              QImage::Format_ARGB32);
    qpix_motion = QPixmap::fromImage(*qimg_motion);

    m_MotionImg->setPixmap(qpix_motion.scaled(PIXMAP_WIDTH, PIXMAP_HEIGHT,
                                              Qt::IgnoreAspectRatio));
    delete qimg_motion;

    // Load videos
    QString url_1 = "file:///D:/STDAN_TensorRT/Video/";
    QString url_2 = "Training.avi";
    QUrl url1(url_1 + url_2);

    QList<QUrl> url_list;
    url_list.append(url1);
    addToPlaylist(url_list);

    metaDataChanged();
}

TrainingUserForm::~TrainingUserForm() {
    delete ui;
}

SceneModifier* TrainingUserForm::get_Model() {
    return modifier;
}

QMediaPlayer* TrainingUserForm::get_QMediaPlayer() {
    return m_player;
}

QMediaPlaylist* TrainingUserForm::get_QMediaPlaylist() {
    return m_playlist;
}

void TrainingUserForm::positionChange(qint64 progress) {
    updateDurationInfo(progress / 1000);
    m_player->setPosition(progress);
}

bool TrainingUserForm::isPlayerAvailable() const {
    return m_player->isAvailable();
}

void TrainingUserForm::open() {
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = m_player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        addToPlaylist(fileDialog.selectedUrls());
}

static bool isPlaylist(const QUrl &url) { // Check for ".m3u" playlists.
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void TrainingUserForm::addToPlaylist(const QList<QUrl> &urls) {
    for (auto &url: urls) {
        if (isPlaylist(url))
            m_playlist->load(url);
        else
            m_playlist->addMedia(url);
    }
}

void TrainingUserForm::setCustomAudioRole(const QString &role) {
    m_player->setCustomAudioRole(role);
}

void TrainingUserForm::durationChanged(qint64 duration) {
    m_duration = duration / 1000;
}

void TrainingUserForm::positionChanged(qint64 progress) {
    updateDurationInfo(progress / 1000);
}

void TrainingUserForm::metaDataChanged() {
    if (m_player->isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                .arg(m_player->metaData(QMediaMetaData::AlbumArtist).toString())
                .arg(m_player->metaData(QMediaMetaData::Title).toString()));

        if (m_coverLabel) {
            QUrl url = m_player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            m_coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

void TrainingUserForm::previousClicked() {
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if (m_player->position() <= 5000)
        m_playlist->previous();
    else
        m_player->setPosition(0);
}

void TrainingUserForm::jump(const QModelIndex &index) {
    if (index.isValid()) {
        m_playlist->setCurrentIndex(index.row());
        m_player->play();
    }
}

void TrainingUserForm::playlistPositionChanged(int currentItem) {

}

void TrainingUserForm::seek(int seconds) {
    m_player->setPosition(seconds * 1000);
}

void TrainingUserForm::statusChanged(QMediaPlayer::MediaStatus status) {
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(tr("Buffering %1%").arg(m_player->bufferStatus()));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Stalled %1%").arg(m_player->bufferStatus()));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void TrainingUserForm::stateChanged(QMediaPlayer::State state) {

}

void TrainingUserForm::handleCursor(QMediaPlayer::MediaStatus status) {
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void TrainingUserForm::bufferingProgress(int progress) {
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
        setStatusInfo(tr("Stalled %1%").arg(progress));
    else
        setStatusInfo(tr("Buffering %1%").arg(progress));
}

void TrainingUserForm::setTrackInfo(const QString &info) {
    m_trackInfo = info;

    if (m_statusBar) {
        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
        if (!m_statusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
        else
            setWindowTitle(m_trackInfo);
    }
}

void TrainingUserForm::setStatusInfo(const QString &info) {
    m_statusInfo = info;

    if (m_statusBar) {
        m_statusBar->showMessage(m_trackInfo);
        m_statusLabel->setText(m_statusInfo);
    } else {
        if (!m_statusInfo.isEmpty())
            setWindowTitle(QString("%1 | %2").arg(m_trackInfo).arg(m_statusInfo));
        else
            setWindowTitle(m_trackInfo);
    }
}

void TrainingUserForm::displayErrorMessage() {
    setStatusInfo(m_player->errorString());
}

void TrainingUserForm::updateDurationInfo(qint64 currentInfo) {
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
}

void TrainingUserForm::showColorDialog() {
    if (!m_colorDialog) {
        QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
        brightnessSlider->setRange(-100, 100);
        brightnessSlider->setValue(m_videoWidget->brightness());
        connect(brightnessSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setBrightness);
        connect(m_videoWidget, &QVideoWidget::brightnessChanged, brightnessSlider, &QSlider::setValue);

        QSlider *contrastSlider = new QSlider(Qt::Horizontal);
        contrastSlider->setRange(-100, 100);
        contrastSlider->setValue(m_videoWidget->contrast());
        connect(contrastSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setContrast);
        connect(m_videoWidget, &QVideoWidget::contrastChanged, contrastSlider, &QSlider::setValue);

        QSlider *hueSlider = new QSlider(Qt::Horizontal);
        hueSlider->setRange(-100, 100);
        hueSlider->setValue(m_videoWidget->hue());
        connect(hueSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setHue);
        connect(m_videoWidget, &QVideoWidget::hueChanged, hueSlider, &QSlider::setValue);

        QSlider *saturationSlider = new QSlider(Qt::Horizontal);
        saturationSlider->setRange(-100, 100);
        saturationSlider->setValue(m_videoWidget->saturation());
        connect(saturationSlider, &QSlider::sliderMoved, m_videoWidget, &QVideoWidget::setSaturation);
        connect(m_videoWidget, &QVideoWidget::saturationChanged, saturationSlider, &QSlider::setValue);

        QFormLayout *layout = new QFormLayout;
        layout->addRow(tr("Brightness"), brightnessSlider);
        layout->addRow(tr("Contrast"), contrastSlider);
        layout->addRow(tr("Hue"), hueSlider);
        layout->addRow(tr("Saturation"), saturationSlider);

        QPushButton *button = new QPushButton(tr("Close"));
        layout->addRow(button);

        m_colorDialog = new QDialog(this);
        m_colorDialog->setWindowTitle(tr("Color Options"));
        m_colorDialog->setLayout(layout);

        connect(button, &QPushButton::clicked, m_colorDialog, &QDialog::close);
    }
    m_colorDialog->show();
}

QLabel *TrainingUserForm::get_m_MotionImg() {
    return m_MotionImg;
}
