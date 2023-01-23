#include <QTcpServer>
#include "timestamp.h"
#include <QJSEngine>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>

class myserver: QObject
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

public slots:
    void startServer(short port);

private slots:
    void newConnectionSlot(QTcpServer *server);
    void readyReadSlot(QString ipString, QTcpSocket *socket);
    void disconnectionSlot(QString ipString, QTcpSocket *socket);
};
