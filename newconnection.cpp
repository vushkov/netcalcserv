#include "newconnection.h"
#include "mydisconnected.h"
#include "timestamp.h"
#include <QString>
#include <QTcpSocket>
#include <QTextStream>

MyNewConnection::MyNewConnection(QObject *parent) : QObject(parent) {}

void MyNewConnection::myNewConn(QTcpServer *conn)
{
    // Получаем сокет вновь подключенного клиента
    QTcpSocket *socket = conn->nextPendingConnection();
    QString ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    QTextStream(stdout) << getTimeStamp() << " > New connection: " << ipString << "\n";

    MyDisconnected *myDisconnObj = new MyDisconnected;

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении

    QObject::connect(socket, &QTcpSocket::disconnected, [=] { myDisconnObj->myDist(ipString); });
};
