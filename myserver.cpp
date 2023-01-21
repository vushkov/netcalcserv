#include "myserver.h"
#include "timestamp.h"
#include <QJSEngine>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>

myserver::myserver(){};
myserver::~myserver(){};

void myserver::startServer()
{
    // Определяем порт
    short port = 12345;

    // Начинаем слушать порт
    this->listen(QHostAddress::AnyIPv4, port);

    // Формируем строку с ip адресом на котором работает сервер
    QString addrString = this->serverAddress().toString();

    // Пишем в лог адрес и порт на котором работает сервер
    QTextStream(stdout) << getTimeStamp() << " > Start to listen: " << addrString << ":" << port
                        << "\n";

    // По сигналу от QTcpServer запускаем слот
    QObject::connect(this, &QTcpServer::newConnection, [=] { this->newConnection(); });
};

void myserver::newConnection()
{
    // Получаем сокет вновь подключенного клиента
    QTcpSocket *socket = this->nextPendingConnection();

    QString ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    QTextStream(stdout) << getTimeStamp() << " > New connection: " << ipString << "\n";

    // Если поступили новые данные в сокет, то отправляется сигнал, по которому слот начинает эти данные вычитывать
    QObject::connect(socket, &QTcpSocket::readyRead, [=] { this->readyRead(socket, ipString); });

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении
    QObject::connect(socket, &QTcpSocket::disconnected, [=] { this->newDisconnection(ipString); });
};

void myserver::readyRead(QTcpSocket *socket, QString ipString)
{
    // Читаем полученные данные
    QString recievedData = socket->readAll();

    // Вычисляем результат выражения
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
}

void myserver::newDisconnection(QString ipString)
{
    // Пишем в лог о разорванном соединении
    QTextStream(stdout) << getTimeStamp() << " > Disconnected: " << ipString << "\n";
}
