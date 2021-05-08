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
    intBig(const QString &v, const quint64 &b);

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
    intBig operator/(const intBig &denominateur) const;
    intBig *operator/=(const intBig &ib);

    qint64 operator%(const qint64 &v) const;
    intBig *operator%=(const qint64 &v);
    intBig operator%(const intBig &ib) const;
    intBig *operator%=(const intBig &ib);

    bool operator<(const intBig &ib) const;
    bool operator>(const intBig &ib) const;
    bool operator==(const intBig &ib) const;
    bool operator!=(const intBig &ib) const;
    bool operator<=(const intBig &ib) const;
    bool operator>=(const intBig &ib) const;

    void operator++(int);
    void operator--(int);
    intBig sqrt() const;
    bool isPrime() const;
    bool isPrime(intBig b) const;
    intBig operator^(quint64 v) const;
    long double toDouble() const;

    bool isEmpty() const;
    bool isNul() const;
    bool isNegative() const;

    QString toString(quint64 base) const;

protected:
    using intBigData = QList<qint64>;
    intBig(const intBigData &v);
    friend class intBigB;

    static const qint64 base = 2147483648; //2^31
    intBigData value;                      // back() (ou last) gère le signe, si c'est négatif ou non
    bool retenue_negative_en_fin = false;  //si retenue négative sur la derniere case

    void addAt(int i, qint64 v);
    void addAt(intBigData::iterator i, qint64 v);

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

inline qint64 intBig::operator%(const qint64 &v) const { return (*this - (*this / v) * v).toDouble(); }
inline intBig *intBig::operator%=(const qint64 &v) { return *this -= (*this / v) * v; }
inline intBig intBig::operator%(const intBig &ib) const { return (*this) - (*this / ib) * ib; }
inline intBig *intBig::operator%=(const intBig &ib) { return *this -= (*this / ib) * ib; }

//////////////////////////////// other inline methods ////////////////////////////////

inline bool intBig::isEmpty() const
{
    return value == intBigData(); // == { 0, 0, ... }
}

inline bool intBig::isNul() const { return isEmpty(); }

inline bool intBig::isNegative() const { return retenue_negative_en_fin; }

//////////////////////////////// intBigB ////////////////////////////////

class intBigB : public intBig
{
    //obj:
    //intBig => string en base spé
    //string en base spé => intBig
public:
    intBigB(const intBig &ib);
    intBigB(const QString &v, const qint64 &base);

    static intBig fromString(QString v, const quint64 &base);
    QString toString(qint64 base = 10) const;

private:
    static intBigData toBase(intBigData v, qint64 baseFrom, qint64 baseTo);
    static QString valueOfBase(const qint64 &v, qint64 base);
    static qint64 valueOfBase(const QString &v, qint64 base);
};

QDebug operator<<(QDebug debug, const intBig &ib);

#endif // INTBIG_H
