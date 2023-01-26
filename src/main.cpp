#include "calcserver.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Создаем объект класса "myserver"
    CalcServer newServer(12345);

    // Применяем к объекту метод - старт сервера
    //newServer.startServer(12345);

    return a.exec();
}
