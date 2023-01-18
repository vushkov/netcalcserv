#ifndef MYDISCONNECTED_H
#define MYDISCONNECTED_H

#include <QObject>

class MyDisconnected : public QObject
{
    Q_OBJECT
public:
    explicit MyDisconnected(QObject *parent = nullptr);

signals:

public slots:
    void myDist(QString ip);
};

#endif // MYDISCONNECTED_H
