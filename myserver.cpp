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
    if (this->listen(QHostAddress::AnyIPv4, port)) {
        // Формируем строку с ip адресом на котором работает сервер
        QString srvAddrStr = this->serverAddress().toString();

        // Пишем в лог адрес и порт на котором работает сервер
        QTextStream(stdout) << "[" << getTimeStamp() << "]"
                            << " > Start listening: " << srvAddrStr << ":" << port << "\n";

        // По сигналу о новом подключении запускаем слот
        connect(this, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    } else {
        // Пишем в лог о неудаче
        QTextStream(stdout) << "[" << getTimeStamp() << "]"
                            << " > Error listening " << port << " port \n";
    }
};

void myserver::newConnectionSlot()
{
    // Получаем сокет вновь подключенного клиента
    QTcpSocket *socket = this->nextPendingConnection();

    // Формируем строку с ip адресом клиента
    QString ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    QTextStream(stdout) << "[" << getTimeStamp() << "]"
                        << " > New connection: " << ipString << "\n";

    // Если поступили новые данные в сокет, то отправляется сигнал, по которому слот начинает эти данные вычитывать
    connect(socket, &QTcpSocket::readyRead, [=] { this->readyReadSlot(socket, ipString); });

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении
    connect(socket, &QTcpSocket::disconnected, [=] { this->disconnectionSlot(ipString); });
};

void myserver::readyReadSlot(QTcpSocket *socket, QString ipString)
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
                        << " > Returned result: " << calcResult << "\n";

    // Переводим double в const char
    QString calcResultString = QString::number(calcResult);
    const char *calcResultchar = calcResultString.toStdString().c_str();

    // Записываем данные в сокет
    socket->write(calcResultchar);
}

void myserver::disconnectionSlot(QString ipString)
{
    // Пишем в лог о разорванном соединении
    QTextStream(stdout) << "[" << getTimeStamp() << "]"
                        << " > Disconnected: " << ipString << "\n";
}
