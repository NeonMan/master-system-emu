#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "romwrapper.h"
#include "iowrapper.h"
#include "z80wrapper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //Register ROM singleton
    qmlRegisterSingletonType("emu.rom", 1, 0, "Test", RomWrapper::singleton_provider);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
