#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "emuthread.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    EmuThread* emu = EmuThread::instance();
    emu->start();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
