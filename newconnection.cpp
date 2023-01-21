#include "newconnection.h"
#include "mydisconnected.h"
#include "timestamp.h"
#include <QJSEngine>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>

QTcpSocket *socket;
QString ipString;

MyNewConnection::MyNewConnection(QObject *parent) : QObject(parent) {}

void MyNewConnection::myNewConn(QTcpServer *conn)
{
    // Получаем сокет вновь подключенного клиента
    socket = conn->nextPendingConnection();
    ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    QTextStream(stdout) << getTimeStamp() << " > New connection: " << ipString << "\n";

    MyDisconnected *myDisconnObj = new MyDisconnected;

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении

    QObject::connect(socket, &QTcpSocket::disconnected, [=] { myDisconnObj->myDist(ipString); });

    QObject::connect(socket, &QTcpSocket::readyRead, [=] { this->readyRead(); });
};

void MyNewConnection::readyRead()
{
    // Читаем полученные данные
    QString recievedData = socket->readAll();

    // Вычислем результат выражения
    QJSEngine expression;
    double calcResult = expression.evaluate(recievedData).toNumber();

    // Выводим в лог исходное полученное выражение
    QTextStream(stdout) << "[" << getTimeStamp() << "]"
                        << "[" << ipString
                        << "]"
                           " > "
                        << "Recieved: " << recievedData << "\n";

    // Выводим в лог результат вычисления
    QTextStream(stdout) << "[" << getTimeStamp() << "]"
                        << "[" << ipString << "]"
                        << " > Result: " << calcResult << "\n";

    QString calcResultString = QString::number(calcResult);

    const char *calcResultchar = calcResultString.toStdString().c_str();
    socket->write(calcResultchar);
};
