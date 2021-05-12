#include "fenetre.h"
#include "ui_fenetre.h"

fenetre::fenetre(QWidget *parent) : QWidget(parent), ui(new Ui::fenetre)
{
    ui->setupUi(this);
    //ui->GenvoyerCode->setDisabled(true);
    connect(ui->GgenererCode, &QPushButton::clicked, this, &fenetre::generer);
    connect(ui->G_btest, &QPushButton::clicked, this, &fenetre::test);
    connect(ui->GrandomButton, &QPushButton::clicked, this, &fenetre::genererRandom);
    connect(ui->GenvoyerCode, &QPushButton::clicked, this, &fenetre::envoyerGACD);
    //ui->GMax->hide();
    //ui->GMin->hide();
    //ui->GMaxL->hide();
    //ui->GMinL->hide();
    //ui->GrandomButton->setText("10 nombres premier aléatoires entre 0 et 2^31");
    connect(ui->CD_bCoderD, &QPushButton::clicked, this, &fenetre::chiffrerD);
    connect(ui->CD_bCoderE, &QPushButton::clicked, this, &fenetre::chiffrerE);
    connect(ui->CD_bCoderDP, &QPushButton::clicked, this, &fenetre::chiffrerDP);
    connect(ui->CD_bCoderEP, &QPushButton::clicked, this, &fenetre::chiffrerEP);
    //actuT=new QTimer;
    //connect(actuT, &QTimer::timeout, this, &actuWindow);
    //actuT->start(50);
    //actu = new actuTask();
    //QThreadPool::globalInstance()->start(actu);

    connect(ui->BM_bCraquer, &QPushButton::clicked, this, &fenetre::crackerMsg);

    show();
}

fenetre::~fenetre()
{
    delete ui;
}

void fenetre::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    qApp->quit();
    deleteLater();
    exit(0);
}

void fenetre::stopGenerer(QString resultat)
{
    ui->GgenererCode->setEnabled(true);
    ui->Gresultat->setText(resultat);
    ui->GenvoyerCode->setEnabled(true);
}

void fenetre::generer()
{
    ui->GgenererCode->setDisabled(true);
    ui->GenvoyerCode->setDisabled(true);
    ui->Gresultat->setText("Calcul en cours...");
    //qDebug() << "generer2 from thread" << QThread::currentThread();
    //RSA::geneOutput retour(actu->generer(ui->Gpremier1->text(), ui->Gpremier2->text(), ui->GProgressPremiers, ui->GProgressED));
    RSA::geneOutput retour(RSA::generer(ui->Gpremier1->text(), ui->Gpremier2->text(), ui->GProgressPremiers, ui->GProgressED, ui->GProgressChiffrement));

    stopGenerer(retour.message);
    ui->GN->setText(retour.n);
    ui->GE->setText(retour.e);
    ui->GD->setText(retour.d);
}

void fenetre::test()
{
    //intBig a=intBig::fromText(ui->Gpremier1->text());
    //intBig b=intBig::fromText(ui->Gpremier2->text());
    //qDebug() << a.toString() << b.toString() << a.toString(2) << b.toString(2) << a.toString(16) << b.toString(16);
    //j'ai rajouté le toString base 16, faudra aussi test le base 2 qui bug  ("\r" et "\u0011")
    //qDebug() << a.toString() << b.toString() << a.toString(20) << b.toString(20) << a.toString(90) << b.toString(90);
    //qDebug(RSA::InverseBModuloN(a, b).toString().toStdString().c_str());

    intBig a = RSA::chiffrer(123, 217691, 432739);
    intBig b = RSA::chiffrer(a, 217691, 432739);
    debug(a.toString()); //chiffré 1 fois
    debug(b.toString()); //chiffré 2 fois
    debug(RSA::chiffrer(123, intBig(217691) ^ 2, 432739).toString());
}

void fenetre::genererRandom()
{
    ui->GrandomButton->setDisabled(true);
    int i = 0;
    intBig min(ui->GMin->text(), 10);
    if (min < 3) //on skip meme 2, de toute facon pas besoin
        min = 3;
    intBig r(min);
    if (r % 2 == 0) r++; //pair
    ui->GrandomText->clear();
    QCoreApplication::processEvents();
    while (i < 4) {
        if (r.isPrime()) {
            i++;
            ui->GrandomText->append(r.toString());
            QCoreApplication::processEvents();
        }
        r += 2;
    }
    ui->GrandomButton->setEnabled(true);
}

void fenetre::envoyerGACD()
{
    ui->GenvoyerCode->setDisabled(true);
    ui->CD_N->setText(ui->GN->text());
    ui->CD_D->setText(ui->GD->text());
    ui->CD_E->setText(ui->GE->text());
    ui->RSAInterface->setCurrentWidget(ui->chiffrer);
    ui->GenvoyerCode->setEnabled(true);
}

void fenetre::chiffrerD()
{
    ui->CD_bCoderD->setDisabled(true);
    debug("Début du chiffrement avec D.");
    qint64 start = RSA::debugTime();
    intBig msg(ui->CD_M->text(), 10);
    intBig d(ui->CD_D->text(), 10);
    intBig n(ui->CD_N->text(), 10);
    if (msg >= n) {
        ui->CD_Resultat->setText("Le message doit etre plus petit que N");
        ui->CD_MChiffre->clear();
        ui->CD_bCoderD->setEnabled(true);
        return;
    } else
        ui->CD_Resultat->setText("Chiffrement avec D...");
    ui->CD_MChiffre->setText(RSA::chiffrer(msg, d, n).toString(10));
    qint64 temps = RSA::debugTime() - start;
    ui->CD_Resultat->setText("Chiffré avec D en " + QString::number(temps / 1000000) + " msec.");
    debug(QString("Chiffré avec D en " + QString::number(temps / 1000000) + " msec."));
    ui->CD_bCoderD->setEnabled(true);
}

void fenetre::chiffrerE()
{
    ui->CD_bCoderE->setDisabled(true);
    debug("Début du chiffrement avec E.");
    qint64 start = RSA::debugTime();
    intBig msg(ui->CD_M->text(), 10);
    intBig e(ui->CD_E->text(), 10);
    intBig n(ui->CD_N->text(), 10);
    if (msg >= n) {
        ui->CD_Resultat->setText("Le message doit etre plus petit que N");
        ui->CD_MChiffre->clear();
        ui->CD_bCoderE->setEnabled(true);
        return;
    } else
        ui->CD_Resultat->setText("Chiffrement avec E...");
    ui->CD_MChiffre->setText(RSA::chiffrer(msg, e, n).toString(10));
    qint64 temps = RSA::debugTime() - start;
    ui->CD_Resultat->setText("Chiffré avec E en " + QString::number(temps / 1000000) + " msec.");
    debug(QString("Chiffré avec E en " + QString::number(temps / 1000000) + " msec."));
    ui->CD_bCoderE->setEnabled(true);
}

void fenetre::chiffrerDP()
{
    ui->CD_bCoderDP->setDisabled(true);
    chiffrerD();
    if (!ui->CD_MChiffre->text().isEmpty()) {
        ui->CD_Resultat->setText(ui->CD_Resultat->text().left(ui->CD_Resultat->text().size() - 1) + ", M à été actualisé.");
        ui->CD_M->setText(ui->CD_MChiffre->text());
        debug(QString("M à été actualisé.")); //Retire le point et rajoute le reste
    }
    ui->CD_bCoderDP->setEnabled(true);
}

void fenetre::chiffrerEP()
{
    ui->CD_bCoderEP->setDisabled(true);
    chiffrerE();
    if (!ui->CD_MChiffre->text().isEmpty()) {
        ui->CD_Resultat->setText(ui->CD_Resultat->text().left(ui->CD_Resultat->text().size() - 1) + ", M à été actualisé.");
        ui->CD_M->setText(ui->CD_MChiffre->text());
        debug(QString("M à été actualisé."));
    }
    ui->CD_bCoderEP->setEnabled(true);
}

void fenetre::debug(QString str)
{
    qDebug().nospace() << "[" << QTime::currentTime().toString("hh:mm:ss") << "] " << str;
}

void fenetre::crackerMsg()
{
    ui->BM_bCraquer->setDisabled(true);
    ui->BMProgressChiffrement->setValue(0);
    ui->BMProgressTab->setValue(0);
    ui->BMProgressEstime->setValue(0);
    ui->BM_log->clear();
    qint64 startCrack = RSA::debugTime();
    QVector<intBig> tab = QVector<intBig>(ui->BM_TabTaille->value());
    ui->BMProgressTab->setMaximum(tab.size());
    intBig newM;
    intBig M(ui->BM_M->text(), 10);
    intBig N(ui->BM_N->text(), 10);
    intBig D(ui->BM_D->text(), 10);
    intBig D2 = D ^ tab.size(); //augmente plus vite pour la partie 2
    if (D2.toDouble() > 1e4932L) {
        ui->BM_log->append("La taille du tableau est trop grande, réduisez la ou abandonnez");
        ui->BM_bCraquer->setEnabled(true);
        return;
    }
    tab[0] = M;  //la premiere case
    intBig iOpe; // pas utilisé pour le tableau car pas opti, mais utile après
    int iLoopActu = 0;

    bool trouve = false;
    qint64 start = RSA::debugTime(), end, timeRSA = 0, timeNoRSA = 0;
    ui->BMProgressEstime->setMaximum(100);
    for (int iTab = 1; iTab < tab.size(); iTab++) {
        end = RSA::debugTime();
        timeNoRSA += end - start;
        start = end;
        newM = RSA::chiffrer(M, D, N);
        end = RSA::debugTime();
        timeRSA += end - start;
        ui->BM_log->append(QString::number(iTab) + ": " + M.toString() + " >> " + newM.toString() + " (en " + QString::number((end - start) / 1000000) + " msec)");
        start = end;
        if ((++iLoopActu %= 100) == 0) {
            ui->BMProgressTab->setValue(iTab);
            QCoreApplication::processEvents();
        }
        if (tab.contains(newM)) { //on l'a trouvé car le cycle est plus petit que le tableau
            trouve = true;
            if (tab.first() == newM) { // newM correspond au tout premier
                iTab--;
            } else { //si il y a eu une boucle, que c'est pas vraiment le premier
                iTab = tab.indexOf(newM) - 1;
                M = tab.at(iTab);
            }
            newM = M; //newM est affiché au final
            iOpe = iTab;
            break;
        } else {
            tab[iTab] = newM;
            M = newM;
        }
    }
    ui->BMProgressTab->setValue(ui->BMProgressTab->maximum());

    qDebug().nospace() << "[Cracker Partie 1] Temps hors chiffrement : " << timeNoRSA << " ns, Temps chiffrement : " << timeRSA << " ns";
    timeNoRSA = 0;
    timeRSA = 0;
    if (!trouve) {
        ui->BM_log->append("passage à la vitesse supérieur");
        iOpe = tab.size() - 1;
    }
    while (!trouve) //continue, sans agrandir le tableau, avec un D plus grand
    {
        end = RSA::debugTime();
        timeNoRSA += end - start;
        start = end;
        newM = RSA::chiffrer(M, D2, N);
        iOpe += tab.size();
        end = RSA::debugTime();
        timeRSA += end - start;
        ui->BM_log->append(iOpe.toString() + ": " + M.toString() + " >> " + newM.toString() + " (en " + QString::number((end - start) / 1000000) + " msec)");
        start = end;
        if ((++iLoopActu %= 100) == 0) {
            ui->BMProgressEstime->setValue((iOpe * 1000 / N).toULongLong());
            QCoreApplication::processEvents();
        }
        if (tab.contains(newM)) {
            int iTab = tab.indexOf(newM);
            iOpe -= iTab - 1; //on l'a trouvé
            newM = RSA::chiffrer(M, D ^ (tab.size() - iTab - 1), N);
            trouve = true;
        } else {
            M = newM;
        }
    }
    //msg trouvé !
    qint64 temps = RSA::debugTime() - startCrack;
    ui->BM_MCasse->setText(newM.toString());
    ui->BM_log->append("Message trouvé en " + QString::number(temps / 1000000) + " msec");
    ui->BMProgressEstime->setValue(ui->BMProgressEstime->maximum());
    qDebug().nospace() << "Message trouvé en " << temps / 1000000 << " ms (taille=" << tab.size() << ") N=" << N << " opérations=" << iOpe;
    qDebug().nospace() << "[Cracker Partie 2] Temps hors chiffrement : " << timeNoRSA << " ns, Temps chiffrement : " << timeRSA << " ns";

    ui->BM_bCraquer->setEnabled(true);
}
