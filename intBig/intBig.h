#ifndef INTBIG_H
#define INTBIG_H

#include <QList>
#include <QtMath>
#include <QDebug>

class intBig
{
public:
    intBig();
    intBig(const qint64 &v);
    intBig(const intBig &ib);

    intBig *operator=(const qint64 &v);
    intBig *operator=(const intBig &ib);

    intBig operator+(const qint64 &v) const;
    intBig *operator+=(const qint64 &v);
    intBig operator+(const intBig &ib) const;
    intBig *operator+=(const intBig &ib);

    intBig operator-(const qint64 &v) const;
    intBig *operator-=(const qint64 &v);
    intBig operator-(const intBig &ib) const;
    intBig *operator-=(const intBig &ib);

    intBig operator*(const qint64 &v) const;
    intBig *operator*=(const qint64 &v);
    intBig operator*(const intBig &ib) const;
    intBig *operator*=(const intBig &ib);

    intBig operator/(const qint64 &v) const;
    intBig *operator/=(const qint64 &v);
    intBig operator/(intBig denominateur) const;
    intBig *operator/=(const intBig &ib);

    intBig operator%(const qint64 &v) const;
    intBig *operator%=(const qint64 &v);
    intBig operator%(const intBig &ib) const;
    intBig *operator%=(const intBig &ib);

    bool operator<(const intBig &ib) const;
    bool operator>(const intBig &ib) const;
    bool operator==(const intBig &ib) const;
    bool operator!=(const intBig &ib) const;
    bool operator<=(const intBig &ib) const;
    bool operator>=(const intBig &ib) const;

    intBig operator^(qint64 v) const;

    bool isEmpty() const;

protected:
    static const qint64 base = 2147483648; //2^31
    QList<qint64> value;                   // back() (ou last) gère le signe, si c'est négatif ou non
    bool retenue_negative_en_fin = false;  //si retenue négative sur la derniere case
    void addAt(const int &i, qint64 v);

private:
    void setAt(int i, qint64 v);
    void reduceValue();

    friend QDebug operator<<(QDebug debug, const intBig &ib);
};

class intBigB : public intBig
{
    //obj:
    //intBig => string en base spé
    //string en base spé => intBig
public:
    intBigB(const intBig &ib);
    intBigB(QString v, const qint64 &base);

    QString toString(qint64 base = 10) const;

private:
    static QList<qint64> toBase(QList<qint64> v, qint64 baseFrom, qint64 baseTo);
    static QString valueOfBase(const qint64 &v, qint64 base);
    static qint64 valueOfBase(const QString &v, qint64 base);
};

QDebug operator<<(QDebug debug, const intBig &ib);

#endif // INTBIG_H
