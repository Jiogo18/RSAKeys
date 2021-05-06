//importé le 10/03/2019 à 12:17
#include "RSA.h"
#include "debug.h"

RSA::RSA(fenetre *parent) : QObject(parent)
{
    //connect(this, SIGNAL(debugS(QString)), parent, SLOT(debugSlot(QString)));//envoie les debugs de rsa vers fenetre
} //fonctionne

intBig4 RSA::chiffrer4(intBig4 msg, intBig4 d_e, intBig4 n)
{
    if (msg >= n) {
        debug::d("le message est trop grand par rapport à N, stop", true);
        return msg;
    }
    /*msg^d_e % n
     * =((msg%n+msg)%n+msg)%n
     * =((msg%n) + (msg%n)) %n+...
     * =(msg%n)²%n+...*/
    //plus efficace
    quint64 startChiffre = debug::time();
    debug::d("début d'un chiffrement:" + msg.toString() + " ^ " + d_e.toString() + " % " + n.toString() + " à " + QString::number(startChiffre));
    intBig4 retour(1);
    intBig4 msg2;
    intBig4 puiss;
    intBig4 d_e2;
    emit started(d_e.toString().size()); //la longeur de d_e
    QCoreApplication::processEvents();
    while (!d_e.isEmpty()) { // d_e != 0
        puiss = 1;
        msg2 = msg;

        //quint64 startPow=debug::time();
        d_e2 = d_e / 2; //pour pas faire d_e/2 tout le temps
        while (puiss <= d_e2) {
            puiss *= 2; //on incrémente de 2
            msg2 = (msg2 * msg2) % n;
        }
        //for(quint64 i=0; i<d_e.toVector(2).size()-1; i++)
        //    msg2 = (msg2*msg2) % n;
        //debug::stat("pow2", startPow, debug::time());
        //quint64 startPow2=debug::time();
        d_e %= puiss; //plus besoin de ces puissances
        //QVector<quint64> d_e2(d_e.toVector(2));
        //d_e2[d_e2.size()-1]--;
        //d_e = intBig4d_e2, 2);
        retour = (retour * msg2) % n;
        //debug::stat("pow2_2", startPow2, debug::time());
        emit progression(d_e.toString().size());
    }
    emit progression(0);
    quint64 stopChiffre = debug::time();
    debug::d("fin de " + QString::number(startChiffre) + " : " + retour.toString() + " en " + QString::number(stopChiffre - startChiffre) + " ns");
    debug::stat("chiffrement4", startChiffre, stopChiffre);
    return retour;
}

intBig5 RSA::chiffrer5(intBig5 msg, intBig5 d_e, intBig5 n)
{
    if (msg >= n) {
        debug::d("le message est trop grand par rapport à N, stop", true);
        return msg;
    }
    /*msg^d_e % n
     * =((msg%n+msg)%n+msg)%n
     * =((msg%n) + (msg%n)) %n+...
     * =(msg%n)²%n+...*/
    //plus efficace
    quint64 startChiffre = debug::time();
    debug::d("début d'un chiffrement:" + msg.toString() + " ^ " + d_e.toString() + " % " + n.toString() + " à " + QString::number(startChiffre));
    intBig5 retour(1);
    intBig5 msg2;
    intBig5 puiss;
    intBig5 d_e2;
    emit started(d_e.toString().size()); //la longeur de d_e
    QCoreApplication::processEvents();
    while (!d_e.isEmpty()) { // d_e != 0
        puiss = 1;
        msg2 = msg;

        //quint64 startPow=debug::time();
        d_e2 = d_e / 2; //pour pas faire d_e/2 tout le temps
        while (puiss <= d_e2) {
            puiss *= 2; //on incrémente de 2
            msg2 = (msg2 * msg2) % n;
        }
        d_e %= puiss; //plus besoin de ces puissances
        retour = (retour * msg2) % n;
        emit progression(d_e.toString().size());
    }
    emit progression(0);
    quint64 stopChiffre = debug::time();
    debug::d("fin de " + QString::number(startChiffre) + " : " + retour.toString() + " en " + QString::number(stopChiffre - startChiffre) + " ns");
    debug::stat("chiffrement5", startChiffre, stopChiffre);
    return retour;
}

intBig6 RSA::chiffrer6(intBig6 msg, intBig6 d_e, intBig6 n)
{
    if (msg >= n) {
        debug::d("le message est trop grand par rapport à N, stop", true);
        return msg;
    }
    /*msg^d_e % n
     * =((msg%n+msg)%n+msg)%n
     * =((msg%n) + (msg%n)) %n+...
     * =(msg%n)²%n+...*/
    //plus efficace
    quint64 startChiffre = debug::time();
    debug::d("début d'un chiffrement:" + intBig6B(msg).toString() + " ^ " + intBig6B(d_e).toString() + " % " + intBig6B(n).toString() + " à " + QString::number(startChiffre));
    intBig6 retour(1);
    intBig6 msg2;
    intBig6 puiss;
    emit started(intBig6B(d_e).toString().size()); //la longeur de d_e
    QCoreApplication::processEvents();
    while (!d_e.isEmpty()) { // d_e != 0
        puiss = 1;
        msg2 = msg;

        while (puiss * 2 <= d_e) {
            puiss *= 2; //on incrémente de 2
            msg2 = (msg2 * msg2) % n;
        }
        d_e %= puiss; //plus besoin de ces puissances
        retour = (retour * msg2) % n;
        emit progression(intBig6B(d_e).toString().size());
    }
    emit progression(0);
    quint64 stopChiffre = debug::time();
    debug::d("fin de " + QString::number(startChiffre) + " : " + intBig6B(retour).toString() + " en " + QString::number(stopChiffre - startChiffre) + " ns");
    debug::stat("chiffrement6", startChiffre, stopChiffre);
    return retour;
}

intBig RSA::chiffrer(intBig msg, intBig d_e, intBig n)
{
    if (msg >= n) {
        debug::d("le message est trop grand par rapport à N, stop", true);
        return msg;
    }
    /*msg^d_e % n
     * =((msg%n+msg)%n+msg)%n
     * =((msg%n) + (msg%n)) %n+...
     * =(msg%n)²%n+...*/
    //plus efficace
    quint64 startChiffre = debug::time();
    debug::d("début d'un chiffrement:" + intBigB(msg).toString() + " ^ " + intBigB(d_e).toString() + " % " + intBigB(n).toString() + " à " + QString::number(startChiffre));
    intBig retour(1);
    intBig msg2;
    intBig puiss;
    emit started(intBigB(d_e).toString().size()); //la longeur de d_e
    while (!d_e.isEmpty()) {                      // d_e != 0
        puiss = 1;
        msg2 = msg;

        while (puiss * 2 <= d_e) {
            puiss *= 2; //on incrémente de 2
            msg2 *= msg2;
            msg2 %= n;
        }
        d_e %= puiss; //plus besoin de ces puissances
        retour *= msg2;
        retour %= n;
        emit progression(intBigB(d_e).toString().size());
    }
    emit progression(0);
    quint64 stopChiffre = debug::time();
    debug::d("fin de " + QString::number(startChiffre) + " : " + intBigB(retour).toString() + " en " + QString::number(stopChiffre - startChiffre) + " ns");
    debug::stat("chiffrement6", startChiffre, stopChiffre);
    return retour;
}
