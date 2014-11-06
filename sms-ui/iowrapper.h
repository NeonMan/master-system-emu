#ifndef IOWRAPPER_H
#define IOWRAPPER_H

#include <QObject>
#include <stdint.h>

class IoWrapper : public QObject
{
    Q_OBJECT
public:
    explicit IoWrapper(QObject *parent = 0);
    Q_INVOKABLE void tick();
    Q_INVOKABLE uint8_t get_state();
    Q_INVOKABLE void set_state(uint8_t b);
signals:

public slots:

};

#endif // IOWRAPPER_H
