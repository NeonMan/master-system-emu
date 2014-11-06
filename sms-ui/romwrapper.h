#ifndef ROMWRAPPER_H
#define ROMWRAPPER_H

#include <QObject>
#include <stdint.h>
#include <QtQml>

class RomWrapper : public QObject
{
    Q_OBJECT
public:
    explicit RomWrapper(QObject *parent = 0);
    Q_INVOKABLE void     tick();
    Q_INVOKABLE uint8_t* get_rom();
    Q_INVOKABLE void     set_rom(uint8_t* buffer, unsigned int size);
    Q_INVOKABLE uint8_t  get_slot(uint8_t slot_number);

    ///QML Callback function (singleton)
    static QJSValue singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

signals:

public slots:

};

#endif // ROMWRAPPER_H
