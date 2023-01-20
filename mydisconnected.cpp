#include "mydisconnected.h"
#include "timestamp.h"
#include <QTextStream>

MyDisconnected::MyDisconnected(QObject *parent) : QObject(parent) {}

void MyDisconnected::myDist(QString ip)
{
    // Пишем в лог о разорванном соединении
    QTextStream(stdout) << getTimeStamp() << " > Disconnected: " << ip << "\n";
}
