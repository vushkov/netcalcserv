#include "calcserver.h"
#include <QDataStream>

myserver::myserver(){};
myserver::~myserver(){};

void myserver::startServer(unsigned short port)
{
    QTcpServer *server = new QTcpServer;

    // Начинаем слушать порт
    if (server->listen(QHostAddress::AnyIPv4, port)) {
        // Формируем строку с ip адресом на котором работает сервер
        QString srvAddrStr = server->serverAddress().toString();

        // Пишем в лог адрес и порт на котором работает сервер
        qInfo().noquote().nospace() << getTimeStamp() << " > Start listening: " << srvAddrStr << ":" << port;

        // По сигналу о новом подключении запускаем слот
        connect(server, &QTcpServer::newConnection, [=] { this->newConnectionSlot(server); });
    } else {
        // Пишем в лог о неудаче
        qWarning().noquote().nospace() << getTimeStamp() << " > Error listening " << port << " port";
    }
};

void myserver::newConnectionSlot(QTcpServer *server)
{
    // Получаем сокет вновь подключенного клиента
    QTcpSocket *socket = server->nextPendingConnection();

    // Устанавливаем максимальное количество ожидающих принятых подключений
    server->setMaxPendingConnections(30);

    // Формируем строку с ip адресом клиента
    QString ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    qInfo().noquote().nospace() << getTimeStamp() << " > New connection: " << ipString;

    // Если поступили новые данные в сокет, то отправляется сигнал, по которому слот начинает эти данные вычитывать
    connect(socket, &QTcpSocket::readyRead, [=] { this->readyReadSlot(ipString, socket); });

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении
    connect(socket, &QTcpSocket::disconnected, [=] { this->disconnectionSlot(ipString, socket); });
};

void myserver::readyReadSlot(QString ipString, QTcpSocket *socket)
{
    // Вводим массив, в котором будет содержаться полученные от клиента данные
    QByteArray recievedData = socket->readAll();

    // Проверяем все ли данные мы получили путем поиска символа \n, который добавляет клиент в конец основным данным
    while (!recievedData.endsWith('\n')) {
        recievedData += socket->readAll();
        qDebug().noquote().nospace() << getTimeStamp() << " CurrentRecievedData: " << recievedData;
    }
    // Если проверка прошла успешно, то удаляем этот последний символ \n
    recievedData.chop(1);

//    QDataStream in;
//    QString recievedData;

//    in.setDevice(socket);
//    in.setVersion(QDataStream::Qt_5_12);

//    in.startTransaction();
//    in >> recievedData;

//    in.commitTransaction();

    // Вычисляем результат выражения
    QJSEngine expression;
    double calcResult = expression.evaluate(recievedData).toNumber();

    // Выводим в лог исходное полученное выражение
    qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                << " > Recieved: " << recievedData;

    // Выводим в лог результат вычисления
    qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                << " > Sent result: " << calcResult;

    // Переводим double в const char
    QString calcResultString = QString::number(calcResult);
    const char *calcResultchar = calcResultString.toStdString().c_str();

    // Записываем данные в сокет
    socket->write(calcResultchar);
}

void myserver::disconnectionSlot(QString ipString, QTcpSocket *socket)
{
    // Пишем в лог о разорванном соединении
    qInfo().noquote().nospace() << getTimeStamp() << " > Disconnected: " << ipString;
    socket->deleteLater();
    //socket->~QTcpSocket();
}
