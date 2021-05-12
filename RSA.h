#ifndef RSA_H
#define RSA_H

#include <qmath.h>
#include <QMap>
#include <QTime>
#include <time.h>
#include <chrono>
#include <QRandomGenerator>
#include "intBig.h"
#include <QProgressBar>
#include <QCoreApplication>

class RSA : public QObject
{
public:
    RSA();

    struct geneOutput
    {
        QString message = "Erreur 1"; //message (d'erreur) n'est pas chargé
        QString termine = "0";
        QString n = "Erreur 2"; //n n'est pas chargé
        QString e = "Erreur 3"; //e n'est pas chargé
        QString d = "Erreur 4"; //d n'est pas chargé
        QString nbPremier1;
        QString nbPremier2;
    };
    static geneOutput generer(QString nbPremier1, QString nbPremier2, QProgressBar *ch1, QProgressBar *ch2, QProgressBar *ch3);

    static void debug(QString str);
    static quint64 random64(quint32 min, quint32 max);
    static intBig InverseBModuloN(intBig b, intBig n);
    static intBig chiffrer(intBig msg, intBig d_e, intBig n);

    static qint64 debugTime()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
};

#endif // RSA_H
