#include "timestamp.h"
#include <QJSEngine>
#include <QTcpServer>
#include <QTcpSocket>
#include <QBuffer>

class CalcServer : QObject
{
    Q_OBJECT
public:
    explicit CalcServer(unsigned short port);
    ~CalcServer();

private:
    QTcpServer *server;

private slots:
    void newConnectionSlot();
    void readyReadSlot(QTcpSocket *socket, QBuffer *buffer);
    void disconnectionSlot();
};
