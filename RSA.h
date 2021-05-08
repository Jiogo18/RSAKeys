#ifndef RSA_H
#define RSA_H

#include <qmath.h>
#include <QMap>
#include <QDateTime>
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
    static geneOutput generer(QString nbPremier1, QString nbPremier2, QProgressBar *ch1 = new QProgressBar, QProgressBar *ch2 = new QProgressBar, QProgressBar *ch3 = new QProgressBar);

    static void debug(QString str);
    static quint64 random64(quint32 min, quint32 max);
    static intBig InverseBModuloN(intBig b, intBig n);
    static intBig chiffrer(intBig msg, intBig d_e, intBig n, QProgressBar *ch = new QProgressBar);
};

#endif // RSA_H
