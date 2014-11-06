#ifndef Z80WRAPPER_H
#define Z80WRAPPER_H

#include <QObject>

class Z80Wrapper : public QObject
{
    Q_OBJECT
public:
    explicit Z80Wrapper(QObject *parent = 0);
    Q_INVOKABLE void tick();
signals:

public slots:

};

#endif // Z80WRAPPER_H
