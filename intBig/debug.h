#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include <QApplication>
#include <QtWidgets>
#include <time.h>
#include <chrono>

class debug
{
public:
    static void d(const QString &str, bool important = false);
    static QWidget *findFenetre();
    static void stat(const QString &type, quint64 start, quint64 stop);

    static qint64 time()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
};

#endif // DEBUG_H
