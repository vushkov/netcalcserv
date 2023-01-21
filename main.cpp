#include "myserver.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Создаем объект класса "myserver"
    myserver server;

    // Применяем к объекту метод - старт сервера
    server.startServer();

    return a.exec();
}
