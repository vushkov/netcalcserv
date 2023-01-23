#include <QTcpServer>
#include "timestamp.h"
#include <QJSEngine>
#include <QString>
#include <QTcpSocket>
#include <QTextStream>

class myserver : public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

public slots:
    void startServer(short port);

private slots:
    void newConnectionSlot();
    void readyReadSlot(QTcpSocket *socket, QString ipString);
    void disconnectionSlot(QString ipString);
};
