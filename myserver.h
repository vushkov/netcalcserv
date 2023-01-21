#include <QTcpServer>

class myserver : public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

public slots:
    void startServer();

private slots:
    void newConnectionSlot();
    void readyReadSlot(QTcpSocket *socket, QString ipString);
    void disconnectionSlot(QString ipString);
};
