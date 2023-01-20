#include "newconnection.h"
#include "timestamp.h"
#include <QCoreApplication>
#include <QTcpServer>
#include <QTextStream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Определяем порт
    short port = 12345;

    // Начинаем слушать порт
    QTcpServer *conn = new QTcpServer();
    conn->listen(QHostAddress::AnyIPv4, port);

    // Формируем строку с ip адресом на котором работает сервер
    QString addrString = conn->serverAddress().toString();

    // Пишем в лог адрес и порт на котором работает сервер
    QTextStream(stdout) << getTimeStamp() << " > Start to listen: " << addrString << ":" << port << "\n";

    MyNewConnection *myNewConnObj = new MyNewConnection();

    // По сигналу от QTcpServer запускаем слот
    QObject::connect(conn, &QTcpServer::newConnection, [=] { myNewConnObj->myNewConn(conn); });

    return a.exec();
}
