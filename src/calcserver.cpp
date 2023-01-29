#include "calcserver.h"

using namespace timestamp;

CalcServer::CalcServer(unsigned short port)
{
    // Определяем объект сервера с родителем
    server = new QTcpServer(this);

    // Устанавливаем максимальное количество ожидающих принятых подключений
    server->setMaxPendingConnections(1);

    // Начинаем слушать порт
    if (server->listen(QHostAddress::AnyIPv4, port)) {
        // Формируем строку с ip адресом на котором работает сервер
        QString srvAddrStr = server->serverAddress().toString();

        // Пишем в лог адрес и порт на котором работает сервер
        qInfo().noquote().nospace() << getTimeStamp() << " > Start listening: " << srvAddrStr << ":" << port;

        // Соединяем новые подключения со слотом
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    } else {
        // Если неудача - пишем в лог
        qWarning().noquote().nospace() << getTimeStamp() << " > Error listening port " << port << ": " << server->errorString();
        // Закрываем сервер - не предпринимаем попыток слушать порт
        server->close();
    }
}
CalcServer::~CalcServer(){};

void CalcServer::newConnectionSlot()
{
    // Получаем сокет вновь подключенного клиента
    QTcpSocket *socket = server->nextPendingConnection();

    // Создаем для этого сокета внутренний буфер
    QBuffer *buffer = new QBuffer(socket);

    // Формируем строку с ip адресом клиента
    QString ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    qInfo().noquote().nospace() << getTimeStamp() << " > New connection: " << ipString;

    // Если поступили новые данные в сокет, то отправляется сигнал, по которому слот начинает эти данные вычитывать
    connect(socket, &QTcpSocket::readyRead, [=] { this->readyReadSlot(socket, buffer); });

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectionSlot()));
}

void CalcServer::readyReadSlot(QTcpSocket *socket, QBuffer *buffer)
{
    // Вводим массив байт, в котором будет содержаться последние полученные от клиента данные
    QByteArray curRecievedData = socket->readAll();

    // Открываем буфер, если он не открыт
    if(!buffer->isOpen()){
        buffer->open(QBuffer::ReadWrite);
    }

    // Записываем в буфер последние полученные данные
    buffer->write(curRecievedData);

    // Считываем весь буфер
    QByteArray allReceivedData = buffer->buffer();

    QString ipString = socket->peerAddress().toString();

    // Пишем в дебаг текущее содержимое буфера
    qDebug().noquote().nospace() << getTimeStamp() << "[" << ipString << "]" << " > Данные буфера: " << allReceivedData;

    // Проверяем все ли данные мы получили путем поиска символа \n, который добавляет клиент в конец основных данных
    if(allReceivedData.endsWith('\n')){

        // Если найден символ конца данных, то:
        // Чистим буфер
        buffer->buffer().clear();

        // Устанавливаем позицию в 0
        buffer->seek(0);

        // Удаляем последний символ \n из данных
        allReceivedData.chop(1);

        // Вычисляем результат выражения
        QJSEngine expression;
        double calcResult = expression.evaluate(allReceivedData).toNumber();

        // Выводим в лог исходное полученное выражение
        qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                    << " > Recieved: " << allReceivedData;

        // Выводим в лог результат вычисления
        qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                    << " > Sent result: " << calcResult;

        // Переводим double в const char
        QString calcResultString = QString::number(calcResult);
        const char *calcResultchar = calcResultString.toStdString().c_str();

        // Записываем данные в сокет
        socket->write(calcResultchar);
    }
}

void CalcServer::disconnectionSlot()
{
    // Получаем сокет с использованием отправителя сигнала
    QTcpSocket *socket = (QTcpSocket *) sender();

    QString ipString = socket->peerAddress().toString();

    // Пишем в лог о разорванном соединении
    qInfo().noquote().nospace() << getTimeStamp() << " > Disconnected: " << ipString;

    // Удаляем сокет, вместе с ним удалится и внутренний буфер buffer
    socket->deleteLater();
}
