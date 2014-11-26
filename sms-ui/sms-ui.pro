TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    emuthread.cpp

RESOURCES += qml.qrc

#Emulator core includes
INCLUDEPATH += ../io/ ../peripherial/ ../psg/ ../rom/
INCLUDEPATH += ../sdsc/ ../vdp/ ../z80/ ../ram/ ../

#Static libraries
LIBS += -L"../build-mingw/"
LIBS += -lz80 -lsmsio -lsmsrom

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    emuthread.h