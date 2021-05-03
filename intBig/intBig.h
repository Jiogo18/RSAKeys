#ifndef INTBIG_H
#define INTBIG_H

#include <QList>
#include <QtMath>
#include <QDebug>

class intBig
{
public:
    intBig();
    intBig(qint64 v);
    intBig(const intBig &ib);

    intBig *operator=(qint64 v);
    intBig *operator=(const intBig &ib);

    intBig *operator+=(qint64 v);
    intBig *operator-=(qint64 v);
    intBig *operator*=(qint64 v) { return operator=(operator*(v)); }
    intBig *operator/=(qint64 v) { return operator=(operator/(v)); }
    intBig *operator%=(qint64 v);
    intBig operator+(qint64 v) const
    {
        intBig retour;
        retour += v;
        return retour;
    }
    intBig operator-(qint64 v) const
    {
        intBig retour;
        retour -= v;
        return retour;
    }
    intBig operator*(qint64 v) const;
    intBig operator/(qint64 v) const;
    intBig operator%(qint64 v) const
    {
        intBig retour;
        retour %= v;
        return retour;
    }

    intBig *operator+=(const intBig &ib);
    intBig *operator-=(const intBig &ib);
    intBig *operator*=(const intBig &ib) { return operator=(operator*(ib)); }
    intBig *operator/=(const intBig &ib) { return operator=(operator/(ib)); }
    intBig *operator%=(const intBig &ib);
    intBig operator+(const intBig &ib) const
    {
        intBig retour(*this);
        retour += ib;
        return retour;
    }
    intBig operator-(const intBig &ib) const
    {
        intBig retour(*this);
        retour -= ib;
        return retour;
    }
    intBig operator*(const intBig &ib) const;
    intBig operator/(intBig denominateur) const;
    intBig operator%(const intBig &ib) const
    {
        intBig retour(*this);
        retour %= ib;
        return retour;
    }

    bool operator<(const intBig &ib) const;
    bool operator>(const intBig &ib) const;
    bool operator==(const intBig &ib) const;
    bool operator!=(const intBig &ib) const { return !operator==(ib); }
    bool operator<=(const intBig &ib) const { return operator<(ib) || operator==(ib); }
    bool operator>=(const intBig &ib) const { return operator>(ib) || operator==(ib); }

    intBig operator^(qint64 v) const;

    bool isEmpty() const;

protected:
    static const qint64 base = 2147483648; //2^31
    QList<qint64> value;
    bool retenue = false; //si retenue négative sur la derniere case
    void addAt(int i, qint64 v);

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
    intBigB(QString v, qint64 base);

    QString toString(qint64 base = 10) const;

private:
    static QList<qint64> toBase(QList<qint64> v, qint64 baseFrom, qint64 baseTo);
    static QString valueOfBase(qint64 v, qint64 base);
    static qint64 valueOfBase(QString v, qint64 base);
};

QDebug operator<<(QDebug debug, const intBig &ib);

#endif // INTBIG_H
