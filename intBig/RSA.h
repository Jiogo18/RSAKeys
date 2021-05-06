#ifndef RSA_H
#define RSA_H
//importé le 10/03/2019 à 12:17

#include <qmath.h>
#include <QMap>
#include <QRandomGenerator>
#include "intBig4.h"
#include "intBig5.h"
#include "intBig6.h"
#include "intBig.h"

class RSA : public QObject
{
    Q_OBJECT
public:
    RSA(QObject *parent);
    intBig4 chiffrer4(intBig4 msg, intBig4 d_e, intBig4 n);
    intBig5 chiffrer5(intBig5 msg, intBig5 d_e, intBig5 n);
    intBig6 chiffrer6(intBig6 msg, intBig6 d_e, intBig6 n);
    intBig chiffrer(intBig msg, intBig d_e, intBig n);

signals:
    void started(int steps);
    void progression(int remainingSteps);
};

#endif // RSA_H
