#include "timestamp.h"
#include <QJSEngine>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>

class CalcServer : QObject
{
    Q_OBJECT
public:
    explicit CalcServer(unsigned short port);
    ~CalcServer();

private:
    QTcpServer *server;
    quint16 nextBlockSize;

public slots:
   // void startServer(unsigned short port);

private slots:
    virtual void newConnectionSlot();
    void readyReadSlot();
    void disconnectionSlot();
    void sendToClientSlot(QTcpSocket *socket, const QString& receivedData);
};
