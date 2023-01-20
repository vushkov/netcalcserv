#include <QObject>
#include <QTcpServer>

class MyNewConnection : public QObject
{
    Q_OBJECT
public:
    explicit MyNewConnection(QObject *parent = nullptr);

signals:

public slots:
    void myNewConn(QTcpServer *conn);
    void readyRead();
};
