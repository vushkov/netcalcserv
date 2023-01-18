#include <QDate>
#include <QString>
#include <QTime>

// Функция, которая формирует строку текущих даты и времени
QString getTimeStamp()
{
    QDate cd = QDate::currentDate();
    QTime ct = QTime::currentTime();

    QString timeStamp = cd.toString("dd.MM.yyyy") + "-" + ct.toString("hh:mm:ss.zzz");

    return timeStamp;
}
