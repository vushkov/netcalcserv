#include "myserver.h"
#include "timestamp.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    myserver server;
    server.startServer();

    return a.exec();
}
