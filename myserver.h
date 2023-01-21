#include <QTcpServer>

class myserver : public QTcpServer
{
    Q_OBJECT
public:
    myserver();
    ~myserver();

public slots:
    void startServer();
    void newConnection();
    void readyRead(QTcpSocket *socket, QString ipString);
    void newDisconnection(QString ipString);
};
