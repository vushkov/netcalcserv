#include "calcserver.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Создаем объект класса myserver, в конструктор передаем порт
    CalcServer newServer(12345);

    return a.exec();
}
