#include "mydisconnected.h"
#include <QTextStream>
#include "timestamp.h"

MyDisconnected::MyDisconnected(QObject *parent) : QObject(parent)
{

}

void MyDisconnected::myDist(QString ip)
{
QTextStream(stdout) << getTimeStamp() << " Disconnected: " << ip << "\n";
}
