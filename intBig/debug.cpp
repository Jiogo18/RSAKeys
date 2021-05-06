#include "debug.h"
#include "fenetre.h"

fenetre *fen = nullptr;

void debug::d(const QString &str, bool important)
{
    if (!fen) fen = static_cast<fenetre *>(findFenetre());
    if (fen) fen->debug(str, important);
}

QWidget *debug::findFenetre()
{
    QWidgetList l = QApplication::allWidgets();
    int i = 0;
    while (i < l.size()) {
        if (l.at(i)->accessibleName() == "fenetre")
            return l.at(i);
        i++;
    }
    qDebug("\"fenetre\" pas trouvé");
    return 0;
}

void debug::stat(const QString &type, quint64 start, quint64 stop)
{
    if (!fen) fen = static_cast<fenetre *>(findFenetre());
    if (fen) fen->addStat(type, stop - start);
}
