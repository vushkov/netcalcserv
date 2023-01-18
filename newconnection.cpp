#include <QTextStream>
#include <QTcpSocket>
#include "newconnection.h"
#include "timestamp.h"

QTcpSocket *socket;

MyNewConnection::MyNewConnection(QObject *parent) : QObject(parent)
{
}

void MyNewConnection::myNewConn(QTcpServer *conn)
    {
    // Получаем сокет вновь подключенного клиента
          socket = conn->nextPendingConnection();
          socket->peerAddress();


    // Выводим в лог информацию о новом подключении
          QTextStream(stdout) << getTimeStamp() << " New connection: " << socket->peerAddress().toString() << "\n";
    };
