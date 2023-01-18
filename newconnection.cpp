#include <QTextStream>
#include <QTcpSocket>
#include <QString>
#include "newconnection.h"
#include "timestamp.h"
#include "mydisconnected.h"



MyNewConnection::MyNewConnection(QObject *parent) : QObject(parent)
{
}

void MyNewConnection::myNewConn(QTcpServer *conn)
    {
    // Получаем сокет вновь подключенного клиента
          QTcpSocket *socket = conn->nextPendingConnection();
          QString ipString = socket->peerAddress().toString();


    // Выводим в лог информацию о новом подключении
          QTextStream(stdout) << getTimeStamp() << " New connection: " << ipString << "\n";

          MyDisconnected *myDisconnObj = new MyDisconnected;

          QObject::connect(socket, &QTcpSocket::disconnected, [=]{ myDisconnObj->myDist(ipString); });
    };
