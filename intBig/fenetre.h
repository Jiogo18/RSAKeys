#ifndef FENETRE_H
#define FENETRE_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMap>
#include "console.h"
#include "debug.h"

struct timeStat
{
    quint64 total;
    quint64 nb_appel;
    quint64 min;
    quint64 max;
};

namespace Ui {
    class fenetre;
}

class fenetre : public QWidget
{
    Q_OBJECT
public:
    explicit fenetre(QWidget *parent = 0);
    fenetre(const fenetre &fen);
    ~fenetre();
    void closeEvent(QCloseEvent *event);
    void calcAll();
    void calcOnly();
    void calc(int nb);
    void debug(QString str, bool important = false);
    void addStat(QString type, quint64 time);
    void finCalc();
    void ouvrirFichier(int i); //i est la méthode de calcul
    void fermerFichier();
    void parcourir();
    void ecrireStats();
    void calcInfo1();

    void calcConsole();

private slots:
    void onRSAStarted(int steps);
    void onRSAProgress(int remainingSteps);

private:
    Ui::fenetre *ui;
    bool calcOn;
    QFile *fSave;
    QFile *fStats;
    QMap<QString, timeStat> mapStats; //temps total, nb d'appel, temps min, temps max
    Console console;
};

#endif // FENETRE_H
