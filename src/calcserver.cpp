#include "calcserver.h"
#include <QDataStream>

CalcServer::CalcServer(unsigned short port) : nextBlockSize(0)
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

        // По сигналу о новом подключении запускаем слот
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    } else {
        // Пишем в лог о неудаче
        qWarning().noquote().nospace() << getTimeStamp() << " > Error listening port " << port << ": " << server->errorString();
        // Закрываем сервер
        server->close();
    }
}
CalcServer::~CalcServer(){};

/*virtual*/ void CalcServer::newConnectionSlot()
{
    // Получаем сокет вновь подключенного клиента
    QTcpSocket *socket = server->nextPendingConnection();

    // Формируем строку с ip адресом клиента
    QString ipString = socket->peerAddress().toString();

    // Выводим в лог информацию о новом подключении
    qInfo().noquote().nospace() << getTimeStamp() << " > New connection: " << ipString;

    // Если поступили новые данные в сокет, то отправляется сигнал, по которому слот начинает эти данные вычитывать
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

    // Если соединение разорвано, сокет отправляет сигнал, по этому сигналу запускаем слот, который сообщит в лог о разорванном соединении
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectionSlot()));
}

void CalcServer::readyReadSlot()
{
    // Получаем сокет с использованием отправителя сигнала
    QTcpSocket *socket = (QTcpSocket *) sender();
    QString ipString = socket->peerAddress().toString();

    // Определяем поток данных
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);
    qDebug().noquote().nospace() << getTimeStamp() << "bytesAvailable = " << socket->bytesAvailable() << " nextBlockSize = " << nextBlockSize;
    // Бесконечный цикл
    for (;;) {
        // Если размер блока равен 0
        if (!nextBlockSize) {
            // и если размер данных не менее 2 байт
            if (socket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            qDebug().noquote().nospace() << getTimeStamp() << "bytesAvailable = " << socket->bytesAvailable() << " nextBlockSize = " << nextBlockSize;
            // То считываем размер блока
            in >> nextBlockSize;
            qDebug().noquote().nospace() << getTimeStamp() << "bytesAvailable = " << socket->bytesAvailable() << " nextBlockSize = " << nextBlockSize;
        }
        // Если размер данных больше или равен блоку, то данные считываются в переменную
        if (socket->bytesAvailable() < nextBlockSize) {
            break;
        }

        qDebug().noquote().nospace() << getTimeStamp() << "bytesAvailable = " << socket->bytesAvailable() << " nextBlockSize = " << nextBlockSize;

        QTime time;
        QString receivedData;

        // Считывание данных
        in >> time >> receivedData;

        // Выводим в лог исходное полученное выражение
        qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                    << " > Recieved: " << receivedData;

        // Обнуляем размер блока
        nextBlockSize = 0;

        // Вызываем метод для отправки данных клиенту
        sendToClientSlot(socket, receivedData);
        qDebug().noquote().nospace() << getTimeStamp() << "bytesAvailable = " << socket->bytesAvailable() << " nextBlockSize = " << nextBlockSize;
    }
}

void CalcServer::sendToClientSlot(QTcpSocket *socket, const QString &receivedData)
{
    // Создаем объект класса QByteArray
    QByteArray arrBlock;

    // Создаем объект класса QDataStream на основе arrBlock
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);

    // Записываем все данные блока, размер устанавливаем равный 0
    out << quint16(0) << QTime::currentTime() << receivedData;

    // Перемещаем указатель на начало блока
    out.device()->seek(0);

    // Вычисляем размер блока
    out << quint16(arrBlock.size() - sizeof(qint16));

    // Записываем данные в сокет
    socket->write(arrBlock);

    /*
    // Вычисляем результат выражения
    QJSEngine expression;
    double calcResult = expression.evaluate(receivedData).toNumber();

    QString ipString = socket->peerAddress().toString();

    // Выводим в лог исходное полученное выражение
    qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                << " > Recieved: " << receivedData;

    // Выводим в лог результат вычисления
    qInfo().noquote().nospace() << getTimeStamp() << "[" << ipString << "]"
                                << " > Sent result: " << calcResult;

    // Переводим double в const char
    QString calcResultString = QString::number(calcResult);
    const char *calcResultchar = calcResultString.toStdString().c_str();
*/


};

void CalcServer::disconnectionSlot()
{
    // Получаем сокет с использованием отправителя сигнала
    QTcpSocket *socket = (QTcpSocket *) sender();

    QString ipString = socket->peerAddress().toString();

    // Пишем в лог о разорванном соединении
    qInfo().noquote().nospace() << getTimeStamp() << " > Disconnected: " << ipString;

    // Удаляем сокет
    socket->deleteLater();
}
