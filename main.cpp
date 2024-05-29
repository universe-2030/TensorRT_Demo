#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;

    QCoreApplication::setApplicationName("Player Example");
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser_Training, parser_UnlabeledDAQ;
    QCommandLineOption customAudioRoleOption("custom-audio-role",
                                             "Set a custom audio role for the player.",
                                             "role");

    // 1. Training Video Dialog
    parser_Training.setApplicationDescription("Qt MultiMedia Player Example");
    parser_Training.addHelpOption();
    parser_Training.addVersionOption();
    parser_Training.addOption(customAudioRoleOption);
    parser_Training.process(a);

    if (parser_Training.isSet(customAudioRoleOption))
        w.get_VideoPlayer_Training()->setCustomAudioRole(parser_Training.value(customAudioRoleOption));

    if (!parser_Training.positionalArguments().isEmpty() &&
            w.get_VideoPlayer_Training()->isPlayerAvailable()) {
        QList<QUrl> urls;
        for (auto &a: parser_Training.positionalArguments())
            urls.append(QUrl::fromUserInput(a, QDir::currentPath(), QUrl::AssumeLocalFile));
        w.get_VideoPlayer_Training()->addToPlaylist(urls);
    }

    // 2. Prediction Video Dialog
    parser_UnlabeledDAQ.setApplicationDescription("Qt MultiMedia Player Example");
    parser_UnlabeledDAQ.addHelpOption();
    parser_UnlabeledDAQ.addVersionOption();
    parser_UnlabeledDAQ.addOption(customAudioRoleOption);
    parser_UnlabeledDAQ.process(a);

    if (parser_UnlabeledDAQ.isSet(customAudioRoleOption))
        w.get_VideoPlayer_UnlabeledDAQ()->setCustomAudioRole(parser_UnlabeledDAQ.value(customAudioRoleOption));

    if (!parser_UnlabeledDAQ.positionalArguments().isEmpty() &&
            w.get_VideoPlayer_UnlabeledDAQ()->isPlayerAvailable()) {
        QList<QUrl> urls;
        for (auto &a: parser_UnlabeledDAQ.positionalArguments())
            urls.append(QUrl::fromUserInput(a, QDir::currentPath(), QUrl::AssumeLocalFile));
        w.get_VideoPlayer_UnlabeledDAQ()->addToPlaylist(urls);
    }

    QDesktopWidget widget;
    QRect screenGeometry = widget.availableGeometry(widget.primaryScreen()); //.screenGeometry(0)
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    w.move((width - w.width()) / 2.0,
           (height - w.height()) / 2.0 - 15);

    w.show();
    return a.exec();
}
