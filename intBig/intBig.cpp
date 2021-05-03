#include "intBig.h"
#include "debug.h"

//////////////////////////////// intBig ////////////////////////////////

intBig::intBig() : value({}) {}
intBig::intBig(const qint64 &v) : value({}) { addAt(0, v); }
intBig::intBig(const intBig &ib) : value(ib.value), retenue_negative_en_fin(ib.retenue_negative_en_fin) {}

intBig *intBig::operator=(const qint64 &v)
{
    value = {};
    addAt(0, v);
    return this;
}
intBig *intBig::operator=(const intBig &ib)
{
    value = ib.value;
    retenue_negative_en_fin = ib.retenue_negative_en_fin;
    return this;
}

//////////////////////////////// operator + ////////////////////////////////

intBig *intBig::operator+=(const qint64 &v)
{
    addAt(0, v);
    reduceValue();
    return this;
}
intBig intBig::operator+(const qint64 &v) const
{
    intBig retour;
    retour.addAt(0, v);
    retour.reduceValue();
    return retour;
}
intBig intBig::operator+(const intBig &ib) const
{
    intBig retour(*this);
    for (int i = 0; i < ib.value.size(); i++)
        retour.addAt(i, ib.value.at(i));
    retour.reduceValue();
    return retour;
}
intBig *intBig::operator+=(const intBig &ib)
{
    for (int i = 0; i < ib.value.size(); i++)
        addAt(i, ib.value.at(i));
    reduceValue();
    return this;
}

//////////////////////////////// operator - ////////////////////////////////

intBig intBig::operator-(const qint64 &v) const
{
    intBig retour(*this);
    retour.addAt(0, -v);
    retour.reduceValue();
    return retour;
}
intBig *intBig::operator-=(const qint64 &v)
{
    addAt(0, -v);
    reduceValue();
    return this;
}
intBig intBig::operator-(const intBig &ib) const
{
    intBig retour(*this);
    for (int i = 0; i < ib.value.size(); i++)
        retour.addAt(i, -ib.value.at(i));
    retour.reduceValue();
    return retour;
}
intBig *intBig::operator-=(const intBig &ib)
{
    for (int i = 0; i < ib.value.size(); i++)
        addAt(i, -ib.value.at(i));
    reduceValue();
    return this;
}

//////////////////////////////// operator * ////////////////////////////////

intBig intBig::operator*(const qint64 &v) const
{
    if (v == 0)
        return 0; // 0
    intBig retour;
    for (int i = 0; i < value.size(); i++)
        retour.addAt(i, value.at(i) * v);
    return retour;
}
intBig *intBig::operator*=(const qint64 &v)
{
    if (v == 0) {
        value = {};
        return this;
    }
    QList<qint64> t = value;
    value = QList<qint64>(value.size());
    for (int i = 0; i < t.size(); i++)
        addAt(i, t.at(i) * v);
    return this;
}
intBig intBig::operator*(const intBig &ib) const
{
    intBig retour;
    for (int i = 0, i2; i < ib.value.size(); i++)
        for (i2 = 0; i2 < value.size(); i2++)
            retour.addAt(i + i2, ib.value.at(i) * value.at(i2));
    retour.reduceValue();
    return retour;
}
intBig *intBig::operator*=(const intBig &ib)
{
    if (isEmpty() || ib.isEmpty()) {
        value = {};
        return this;
    }
    QList<qint64> t = value;
    value = QList<qint64>(value.size());
    for (int i = 0, i2; i < ib.value.size(); i++)
        for (i2 = 0; i2 < t.size(); i2++)
            addAt(i + i2, ib.value.at(i) * t.at(i2));
    return this;
}

//////////////////////////////// operator / ////////////////////////////////

intBig intBig::operator/(const qint64 &v) const
{
    Q_ASSERT(v != 0);

    intBig retour;
    if (v == base) { //optimisé
        QList<qint64> v2 = value;
        v2.removeFirst();
        retour.value = v2;
        return retour;
    }
    qint64 currentV = 0;
    for (int i = value.size() - 1; i >= 0; i--) {
        currentV += value.at(i);
        retour.addAt(i, currentV / v);
        currentV %= v;
        currentV *= base;
    }
    retour.reduceValue();
    return retour;
}
intBig *intBig::operator/=(const qint64 &v)
{
    Q_ASSERT(v != 0);

    QList<qint64> t = value;
    value = {0};
    if (v == base) { //optimisé
        QList<qint64> v2 = t;
        v2.removeFirst();
        value = v2;
        return this;
    }
    qint64 currentV = 0;
    for (int i = t.size() - 1; i >= 0; i--) {
        currentV += t.at(i);
        addAt(i, currentV / v);
        currentV %= v;
        currentV *= base;
    }
    reduceValue();
    return this;
}
intBig intBig::operator/(intBig denominateur) const
{
    qint64 start = debug::time();
    if (operator<(denominateur))
        return {};
    if (denominateur == 1)
        return *this;
    intBig mult;
    intBig numerateur(*this);
    int exp = 0;
    int value_coef = 8;
    //(7<vc<15) obj: etre <170 voir <160 (moyenne:165)
    //min: 10:150 9:153 8:148 7:156 6:157
    //=> le plus opti est un base 8 (peut etre car *2)
    intBig denominateur2(denominateur * value_coef);
    while (numerateur >= denominateur2) {
        denominateur = denominateur2;
        denominateur2 *= value_coef;
        exp++;
    }
    //division
    int current_mult;
    while (exp >= 0) {
        current_mult = 0;
        denominateur2 = denominateur;
        while (denominateur2 <= numerateur) {
            denominateur2 += denominateur;
            current_mult++;
        }
        numerateur -= denominateur * current_mult; //il y a pas plus opti
        mult *= value_coef;                        //on décale
        mult.addAt(0, current_mult);
        denominateur /= value_coef;
        exp--;
    }
    debug::stat("/intBig", start, debug::time());
    return mult;
}
intBig *intBig::operator/=(const intBig &ib) { return operator=(operator/(ib)); }

//////////////////////////////// operator % ////////////////////////////////

intBig intBig::operator%(const qint64 &v) const
{
    intBig retour;
    retour %= v;
    return retour;
}
intBig *intBig::operator%=(const qint64 &v) { return operator-=(operator/(v) * v); }
intBig intBig::operator%(const intBig &ib) const
{
    return (*this) - operator/(ib) * ib;
}
intBig *intBig::operator%=(const intBig &ib)
{
    qint64 start = debug::time();
    operator-=(operator/(ib) * ib);
    debug::stat("%=intBig", start, debug::time());
    return this;
}

//////////////////////////////// comparison operators ////////////////////////////////

bool intBig::operator<(const intBig &ib) const
{
    if (retenue_negative_en_fin && ib.retenue_negative_en_fin) { //tous les 2 -
        if (value.size() != ib.value.size())
            return value.size() > ib.value.size();
    } else if (retenue_negative_en_fin || ib.retenue_negative_en_fin) { //l'un des 2 -
        return retenue_negative_en_fin;                                 //this < 0 < ib sinon c'est l'inverse
    } else {                                                            //tous les 2 +
        if (value.size() != ib.value.size())
            return value.size() < ib.value.size();
    }
    for (int i = value.size() - 1; i >= 0; i--)
        if (value.at(i) != ib.value.at(i)) {
            return value.at(i) < ib.value.at(i);
        }
    return false; // c'est ==
}
bool intBig::operator>(const intBig &ib) const
{
    if (retenue_negative_en_fin && ib.retenue_negative_en_fin) { //tous les 2 -
        if (value.size() != ib.value.size())
            return value.size() < ib.value.size();
    } else if (retenue_negative_en_fin || ib.retenue_negative_en_fin) { //l'un des 2 -
        return !retenue_negative_en_fin;                                //this > 0 > ib sinon c'est l'inverse
    } else {                                                            //tous les 2 +
        if (value.size() != ib.value.size())
            return value.size() > ib.value.size();
    }
    for (int i = value.size() - 1; i >= 0; i--)
        if (value.at(i) != ib.value.at(i)) {
            return value.at(i) > ib.value.at(i);
        }
    return false; // c'est ==
}
bool intBig::operator==(const intBig &ib) const
{
    if (value.size() == 0 && ib.value.size() == 0) return true;
    if (value.size() != ib.value.size())
        return false;
    for (int i = value.size() - 1; i >= 0; i--)
        if (value.at(i) != ib.value.at(i))
            return false;
    return true;
}

bool intBig::operator!=(const intBig &ib) const
{
    if (value.size() == 0 && ib.value.size() == 0) return false;
    if (value.size() != ib.value.size())
        return true;
    for (int i = value.size() - 1; i >= 0; i--)
        if (value.at(i) != ib.value.at(i))
            return true;
    return false;
}

bool intBig::operator<=(const intBig &ib) const
{
    if (retenue_negative_en_fin && ib.retenue_negative_en_fin) { //tous les 2 -
        if (value.size() != ib.value.size())
            return value.size() > ib.value.size();
    } else if (retenue_negative_en_fin || ib.retenue_negative_en_fin) { //l'un des 2 -
        return retenue_negative_en_fin;                                 //this < 0 < ib sinon c'est l'inverse
    } else {                                                            //tous les 2 +
        if (value.size() != ib.value.size())
            return value.size() < ib.value.size();
    }
    for (int i = value.size() - 1; i >= 0; i--)
        if (value.at(i) != ib.value.at(i)) {
            return value.at(i) <= ib.value.at(i);
        }
    return true; //c'est ==
}

bool intBig::operator>=(const intBig &ib) const
{
    if (retenue_negative_en_fin && ib.retenue_negative_en_fin) { //tous les 2 -
        if (value.size() != ib.value.size())
            return value.size() < ib.value.size();

    } else if (retenue_negative_en_fin || ib.retenue_negative_en_fin) { //l'un des 2 -
        return !retenue_negative_en_fin;                                //this > 0 > ib sinon c'est l'inverse
    } else {                                                            //tous les 2 +
        if (value.size() != ib.value.size())
            return value.size() > ib.value.size();
    }
    for (int i = value.size() - 1; i >= 0; i--)
        if (value.at(i) != ib.value.at(i)) {
            return value.at(i) >= ib.value.at(i);
        }
    return true; // c'est ==
}

intBig intBig::operator^(qint64 v) const
{
    Q_ASSERT(v >= 0);
    intBig retour;
    intBig square;
    int i;
    while (v > 0) {
        square = *this;
        for (i = 0; i < v; i *= 2) {
            square *= square;
        }
        retour += square;
        v -= i / 2;
    }
    return retour;
}

bool intBig::isEmpty() const
{
    for (int i = 0; i < value.size(); i++)
        if (value.at(i) != 0)
            return false;
    return true;
}

//////////////////////////////// intBig private ////////////////////////////////

void intBig::addAt(const int &i, qint64 v)
{
    if (value.size() >= 4000) {
        qDebug() << "TROP GRAND" << value.size();
    }

    if (retenue_negative_en_fin) {
        while (value.size() <= i) {
            value.last() += base;
            value.append(-1);
        }
    } else {
        if (value.size() <= i) {
            value.insert(value.size(), i - value.size() + 1, 0);
        }
    }

    QList<qint64>::reference r = value[i];
    r += v;

    if (value.size() - 1 == i) {
        // dernière valeur
        if (r < 0) {
            // et on est à -1
            retenue_negative_en_fin = true;
            return; // pas de retenue à passer
        } else {
            // même si on a une retenue on sera forcément positif après
            retenue_negative_en_fin = false;
        }
    }

    qint64 retenue = qFloor((double)r / base);
    r -= retenue * base; // modulo
    if (retenue)
        addAt(i + 1, retenue);
}
void intBig::setAt(int i, qint64 v)
{
    qint64 start = debug::time();
    if (value.size() <= i) {
        if (retenue_negative_en_fin) {
            // TODO : alternative ?
            // en tout cas setAt est tellement pas appelé que c'est dificile de test
            //int nb_de_base = - value.last() / base;
            //qint64 last_target = value.last() + nb_de_base * base;
            //int v_target = v - nb_de_base;

            while (value.last() < 0) {
                v -= 1;
                value.last() += base;
            }

            retenue_negative_en_fin = false;
        }
        value.insert(value.size(), i - value.size() + 1, 0);
    }
    value[i] = 0;
    addAt(i, v);
    debug::stat("setAt", start, debug::time());
}
void intBig::reduceValue()
{
    while (!value.isEmpty() && value.last() == 0)
        value.removeLast();
}

//////////////////////////////// intBigB ////////////////////////////////

intBigB::intBigB(const intBig &ib) : intBig(ib) {}
intBigB::intBigB(QString v, const qint64 &base) : intBig(0)
{
    QList<qint64> retour = {};
    bool negative = v.startsWith("-");
    if (negative)
        v.remove("-");
    if (base == 10 || base == 16 || base == 36) {
        while (!v.isEmpty()) {
            retour.push_back(valueOfBase(QString(v.back()), base));
            v.remove(v.size() - 1, 1);
        }
    } else {
        QString v2 = "";
        while (!v.isEmpty()) {
            while (!v.isEmpty() && v.back() != '|') {
                v2 += v.back();
                v.remove(v.size() - 1, 1);
            }
            if (!v.isEmpty()) //le |
                v.remove(v.size() - 1, 1);
            retour.push_back(valueOfBase(v2, base));
        }
    }
    retour = toBase(retour, base, intBig::base);
    if (negative) {
        if (retour.size() > 0 && retour.last() == 1)
            retour.last() = -1;
        else
            retour.push_back(-1);
    }
    value = retour;
}

QString intBigB::toString(qint64 base) const
{
    QList<qint64> withBase = toBase(value, intBig::base, base);
    QString d = "";
    if (value.size() > 0 && value.back() < 0) {
        d = "-"; //negative
        qint64 retenue = 0;
        int baseSize = withBase.size();
        for (int i = 0; i < withBase.size() || retenue != 0; i++) {
            if (withBase.size() <= i)
                withBase.insert(i, 0);
            else
                withBase[i] *= -1;
            withBase[i] += retenue;
            if (withBase.at(i) < 0) {
                withBase[i] += base;
                retenue--;
                if (i + 100 > baseSize) { //bug on a ajouté trop de cases
                    qDebug() << "intBigB::toString ERROR it is a positive array ?";
                    break;
                }
            }
        }
    }
    for (int i = withBase.size() - 1; i >= 0; i--) {
        if (i != withBase.size() - 1 && base > 10 && !QList<int>({16, 36}).contains(base))
            d += "|";
        d += valueOfBase(withBase.at(i), base);
    }
    return d;
}
QList<qint64> intBigB::toBase(QList<qint64> v, qint64 baseFrom, qint64 baseTo)
{
    QList<qint64> retour = {};
    if (v.isEmpty())
        return {};
    if (baseFrom == baseTo) //si c les meme base
        return v;
    for (qint64 i = v.size() - 1; i >= 0; i--) {

        qint64 baseCase = qPow(baseFrom, i); //TODO: à terme il faudrait une liste
        QList<qint64> retourPlus = {};
        int i2 = 0;
        while (baseCase > 0) {
            while (retourPlus.size() <= i2)
                retourPlus.insert(i2, 0);
            retourPlus[i2] += baseCase;
            baseCase = retourPlus[i2] / baseTo;
            retourPlus[i2] = retourPlus[i2] % baseTo;
            i2++;
        }
        //on fait: intBigB baseCase(intBigB(intBig::base, base)^i);
        //on fait: retour += pow(baseintBig::base, ) * value.at(i);
        qint64 retenue = 0;
        for (int i2 = 0; i2 < retourPlus.size() || retenue != 0; i2++) {
            while (retour.size() <= i2)
                retour.insert(i2, 0);
            if (i2 < retourPlus.size())
                retour[i2] += retourPlus.at(i2) * v.at(i);
            retour[i2] += retenue;
            retenue = retour[i2] / baseTo;
            retour[i2] = retour[i2] % baseTo;
        }
    }

    return retour;
}

QString intBigB::valueOfBase(const qint64 &v, qint64 base)
{
    try {
        switch (base) {
        case 16:
            return QString("0123456789ABCDEF").at(v);
        case 36:
            return QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").at(v);
        }
    } catch (const std::exception &error) {
    }
    return QString::number(v);
}
qint64 intBigB::valueOfBase(const QString &v, qint64 base)
{
    int retour = -1;
    try {
        switch (base) {
        case 16:
            retour = QString("0123456789ABCDEF").indexOf(v);
            break;
        case 36:
            retour = QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").indexOf(v);
            break;
        }
    } catch (const std::exception &error) {
    }
    if (retour != -1)
        return retour;
    return v.toLongLong();
}

QDebug operator<<(QDebug debug, const intBig &ib)
{
    debug << intBigB(ib).toString().toStdString().c_str();
    return debug;
}
