TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    romwrapper.cpp

RESOURCES += qml.qrc

#Emulator core includes
INCLUDEPATH += ../io/ ../peripherial/ ../psg/ ../rom/
INCLUDEPATH += ../sdsc/ ../vdp/ ../z80/ ../ram/ ../

#Static libraries
LIBS += -L"../build-mingw/" -lio -lperipherial -lpsg
LIBS += -lrom -lsdsc -lvdp -lz80 -lram

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    romwrapper.h
