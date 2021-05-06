#ifndef INTBIG_H
#define INTBIG_H

#include <QList>
#include <QtMath>
#include <QDebug>

//////////////////////////////// intBig ////////////////////////////////

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

    intBig operator^(quint64 v) const;
    long double toDouble() const;

    bool isEmpty() const;

protected:
    static const qint64 base = 2147483648; //2^31
    QList<qint64> value;                   // back() (ou last) gère le signe, si c'est négatif ou non
    bool retenue_negative_en_fin = false;  //si retenue négative sur la derniere case
    void addAt(int i, qint64 v);
    void addAt(QList<qint64>::iterator i, qint64 v);

private:
    void setAt(int i, qint64 v);
    void verifBase();
    void reduceValue();

    friend QDebug operator<<(QDebug debug, const intBig &ib);
};

//////////////////////////////// inline comparison operator ////////////////////////////////

inline bool intBig::operator>(const intBig &ib) const { return ib < *this; }
inline bool intBig::operator==(const intBig &ib) const { return value == ib.value; }
inline bool intBig::operator!=(const intBig &ib) const { return !(*this == ib); }
inline bool intBig::operator<=(const intBig &ib) const { return !(ib < *this); }
inline bool intBig::operator>=(const intBig &ib) const { return !(*this < ib); }

//////////////////////////////// inline operator % ////////////////////////////////

inline intBig intBig::operator%(const qint64 &v) const { return *this - (*this / v) * v; }
inline intBig *intBig::operator%=(const qint64 &v) { return *this -= (*this / v) * v; }
inline intBig intBig::operator%(const intBig &ib) const { return (*this) - (*this / ib) * ib; }
inline intBig *intBig::operator%=(const intBig &ib) { return *this -= (*this / ib) * ib; }

//////////////////////////////// other inline methods ////////////////////////////////

inline bool intBig::isEmpty() const
{
    return value == QList<qint64>(); // == { 0, 0, ... }
}

//////////////////////////////// intBigB ////////////////////////////////

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
