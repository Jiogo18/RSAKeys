#include "intBig.h"
#include "debug.h"

//////////////////////////////// intBig ////////////////////////////////

intBig::intBig() : value({}) {}
intBig::intBig(const qint64 &v) : value({v}) { verifBase(); }
intBig::intBig(const intBig &ib) : value(ib.value), retenue_negative_en_fin(ib.retenue_negative_en_fin) {}
intBig::intBig(const QString &v, const quint64 &b) : intBig(intBigB::fromString(v, b)) {}

intBig *intBig::operator=(const qint64 &v)
{
    value = {v};
    verifBase();
    return this;
}
intBig *intBig::operator=(const intBig &ib)
{
    value = ib.value;
    retenue_negative_en_fin = ib.retenue_negative_en_fin;
    return this;
}

//////////////////////////////// operator + ////////////////////////////////

intBig intBig::operator+(const qint64 &v) const
{
    intBig retour = *this;
    retour.addAt(0, v);
    retour.reduceValue();
    return retour;
}
intBig *intBig::operator+=(const qint64 &v)
{
    addAt(0, v);
    reduceValue();
    return this;
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
        return 0;
    intBig retour;
    retour.value = intBigData(value.size());
    for (int i = 0; i < value.size(); i++)
        retour.value[i] = value.at(i) * v;
    retour.verifBase();
    return retour;
}
intBig *intBig::operator*=(const qint64 &v)
{
    if (v == 0) {
        value = {};
        return this;
    }
    for (int i = 0; i < value.size(); i++)
        value[i] *= v;
    verifBase();
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
intBig *intBig::operator*=(const intBig &ib) { return *this = *this * ib; }

//////////////////////////////// operator / ////////////////////////////////

intBig intBig::operator/(const qint64 &v) const
{
    Q_ASSERT(v != 0);

    intBig retour;
    if (v == base) { //optimisé
        intBigData v2 = value;
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

    intBigData t = value;
    value = {0};
    if (v == base) { //optimisé
        intBigData v2 = t;
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
intBig intBig::operator/(const intBig &denominateur) const
{
    intBig numerateur(*this);
#define coef_division 16
    // on décompose le calcul numerateur/denominateur :
    // coef_division = cd
    // mult = [ mult0 | mult1 | ... | multn ] écrit en base cd
    // denominateurs = [ den | den * cd | ... | den * cd^n ]
    // numerateur = mult0*den0 + mult1*den1 + ... + multn*denn

    // multi appartient à [0;cd[
    // et entre 2 deni il y a cd
    // donc un nombre multi*deni est unique
    // ce qui fait que :
    //  si numerateur < denin+1
    //  si numerateur >= deni
    //  alors multi est le plus grand nombre tel que :
    //   numerateur >= multi * deni
    //   (ainsi numerateur < (multi + 1) * deni)

    QList<intBig> denominateurs = {denominateur};
    while (numerateur >= denominateurs.last()) {
        denominateurs.append(denominateurs.last() * coef_division);
    }
    denominateurs.removeLast();

    //division
    intBig mult;
    qint64 current_mult;
    for (QList<intBig>::const_reverse_iterator i(denominateurs.rbegin()); i != denominateurs.rend(); i++) {
        current_mult = 0;
        while (*i <= numerateur) {
            numerateur -= *i;
            current_mult++;
        }
        mult *= coef_division; //on décale
        mult += current_mult;  // mult est une valeur en base value_coef
    }
    return mult;
}
intBig *intBig::operator/=(const intBig &ib) { return *this = *this / ib; }

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

    // attention, pdt le debug les reverse_iterator affichent une valeur "current" qui est la précédente
    // donc il faut pas s'y fier
    return std::lexicographical_compare(value.rbegin(), value.rend(), ib.value.rbegin(), ib.value.rend());
}

//////////////////////////////// other operators ////////////////////////////////

void intBig::operator++(int) { operator+=(1); }
void intBig::operator--(int) { operator-=(1); }

intBig intBig::sqrt() const
{
    quint64 start = debug::debugTime();
    if (retenue_negative_en_fin) {
        qDebug("erreur sqrt, racine d'un nombre négatif");
        debug::stat("sqrt-", start, debug::debugTime());
        return 0;
    }
    if (isEmpty()) {
        debug::stat("sqrt0", start, debug::debugTime());
        return 0; //0²=0
    }
    if (*this == 1) {
        debug::stat("sqrt1", start, debug::debugTime());
        return 1; //1²=1
    }

    intBig min(1);
    intBig retour(1);
    intBig fois = *this / 2;
#define coef_sqrt 20
    while (fois > 1) {
        while (retour * retour <= *this) { //augmentation au fur et à mesure
            min = retour;
            retour *= fois;
        }
        retour = min;
        fois /= coef_sqrt;
    }

    intBig plus(retour);
    while (plus > 0) {
        while (retour * retour <= *this) { //augmentation au fur et à mesure
            min = retour;
            retour += plus;
        }
        retour = min;
        if (plus > 1 && plus < coef_sqrt) {
            plus = 1; //+1
        } else
            plus /= coef_sqrt;
    }
    //ici retour² est == ou au dessous de this
    debug::stat("sqrt", start, debug::debugTime());
    return retour;
}

bool intBig::isPrime() const
{
    if (isEmpty()) return false;
    // on quelques nombres premiers les plus fréquents
    if (*this % 2 == 0) return false;
    if (*this % 3 == 0) return false;
    if (*this % 5 == 0) return false;
    if (*this % 7 == 0) return false;
    if (*this % 13 == 0) return false;
    if (*this % 17 == 0) return false;
    if (*this % 19 == 0) return false;
    if (*this % 23 == 0) return false;
    if (*this % 29 == 0) return false;

    // ensuite on fait le cycle en faisant un skip des nombres multiples de nombres non premiers les plus fréquents
    // cycle : de 0 à 588153930-1,   588153930 = 2*3*7*13*17*19*23*29
    intBig sqrtNb = sqrt();
    int cycle = 7;

    for (intBig i = 7; i <= sqrtNb; i += 2, (cycle += 2) %= 588153930) { //les pairs ont déja ete testé
        if (cycle % 3 * cycle % 5 * cycle % 7 * cycle % 13 * cycle % 17 * cycle % 19 * cycle % 23 * cycle % 29 && (*this % i).isEmpty()) {
            return false;
        }
    }
    return true;
}

bool intBig::isPrime(intBig b) const
{
    if (*this < b) return b.isPrime(*this);
    intBig temp, a(*this);

    /*while(nb2>0)
    {
        temp = nb2%nb1;
        nb1 = nb2;
        nb2 = temp;
    }
    return nb1 == 1;//if 1, they are prime else nb1 = PGCD*/
    //PGCD scratch

    temp = a;
    while (b > 0) {
        //while(temp>=b)
        //temp -= b;
        temp %= b; //plus rapide
        a = b;
        b = temp;
        temp = a;
    }
    return temp == 1; //temp== pgcd et si c'est 1, ils sont premiers entre eux
}

intBig intBig::operator^(quint64 v) const
{
    intBig retour = 1;
    intBig square;
    quint64 i;
    while (v > 0) {
        square = *this;
        for (i = 1; i * 2 <= v; i *= 2) {
            square *= square;
        }
        // square = this^(2i)
        retour *= square;
        v -= i;
    }
    // this^n = this * this * this ... v fois = this^1 * this^2 * this^4 * this^8 * ... * this^(2^i) * ...
    return retour;
}

long double intBig::toDouble() const
{
    long double v = 0;
    for (intBigData::const_reverse_iterator i(value.rbegin()); i != value.rend(); i++) {
        v *= base;
        v += *i;
    }
    return v;
}

//////////////////////////////// intBig temporaire ////////////////////////////////

QString intBig::toString(quint64 base) const { return intBigB(*this).toString(); }

//////////////////////////////// intBig private ////////////////////////////////

void intBig::addAt(int i, qint64 v)
{
    //return addAt(value.begin() + i, v);
    if (v == 0) return;

    Q_ASSERT(value.size() < 10000);

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

    intBigData::reference r = value[i];
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

void intBig::addAt(intBigData::iterator i, qint64 v)
{
    if (v == 0) return;
    if (!i) {
        value = {0};
        i = value.begin();
    }
    Q_ASSERT(value.size() < 10000);

    if (retenue_negative_en_fin) {
        while (value.end() <= i) {
            value.last() += base;
            value.append(-1);
        }
    } else {
        while (value.end() <= i) {
            //value.insert(value.end(), 0);
            value.append(0);
            // TODO: ça marche ça?
        }
    }

    intBigData::reference r = (*i);
    r += v;

    if (value.end() == i + 1) {
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

intBig::intBig(const intBig::intBigData &v) : value(v), retenue_negative_en_fin(!v.isEmpty() && v.last() < 0) {}

void intBig::setAt(int i, qint64 v)
{
    qint64 start = debug::debugTime();
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
        if (value.size() <= i)
            value.insert(value.size(), i - value.size() + 1, 0);
    }
    value[i] = 0;
    addAt(i, v);
    debug::stat("setAt", start, debug::debugTime());
}

void intBig::verifBase()
{
    if (value.isEmpty())
        return;
    qint64 retenue = 0;
    for (intBigData::iterator i = value.begin(); i != value.end(); i++) {
        // v = *i;
        *i += retenue;
        if (0 <= *i && *i < base) {
            retenue = 0;
            continue;
        }
        if (*i < 0) {
            if (*i == -1 && i == value.end() - 1) // si c'est -1 on ignore aussi
                continue;
            retenue = *i / base + 1;
        } else {
            retenue = *i / base;
        }
        *i -= retenue * base;       // modulo base
        if (i + 1 == value.end()) { // et qu'il y a une retenue
            value.append(0);
            i = value.end();
            i -= 2;
        }
    }
    retenue_negative_en_fin = value.back() < 0;
}
void intBig::reduceValue()
{
    while (!value.isEmpty() && value.last() == 0)
        value.removeLast();
}

//////////////////////////////// intBigB ////////////////////////////////

intBigB::intBigB(const intBig &ib) : intBig(ib) {}
intBigB::intBigB(const QString &v, const qint64 &base) : intBig(fromString(v, base)) {}

intBig intBigB::fromString(QString v, const quint64 &base)
{
    intBigData retour = {};
    bool negative = v.startsWith("-");
    if (negative)
        v.remove("-");
    if (base == 10 || base == 16 || base == 36) {
        while (!v.isEmpty()) {
            retour.append(valueOfBase(QString(v.back()), base));
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
            retour.append(valueOfBase(v2, base));
        }
    }
    retour = toBase(retour, base, intBig::base);
    if (negative) {
        if (retour.size() > 0 && retour.last() == 1)
            retour.last() = -1;
        else
            retour.append(-1);
    }

    return retour;
}

QString intBigB::toString(qint64 base) const
{
    intBigData withBase = toBase(value, intBig::base, base);
    QString d = "";
    if (value.size() > 0 && value.last() < 0) {
        d = "-"; //negative
        qint64 retenue = 0;
        int baseSize = withBase.size();
        for (int i = 0; i < withBase.size() || retenue != 0; i++) {
            if (withBase.size() <= i)
                withBase.insert(withBase.size(), i - withBase.size() + 1, 0);
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
    for (int i = withBase.size(); i > 0; i--) {
        if (i != withBase.size() && base > 10 && (base != 16 && base != 36))
            d += "|";
        d += valueOfBase(withBase.at(i - 1), base);
    }
    if (d == "") return "0";
    return d;
}
intBig::intBigData intBigB::toBase(intBigData v, qint64 baseFrom, qint64 baseTo)
{
    intBigData retour = {};
    if (v.isEmpty())
        return {};
    if (baseFrom == baseTo) //si c les meme base
        return v;

    QVector<intBig> baseCase = {1};
    for (int i = 0; i < v.size(); i++) {
        // = baseFrom^i (valeur de la case si remplie avec 1)
        baseCase.append(baseCase.last() * baseFrom);
    }
    int baseCasei = v.size() - 1;
    intBig baseCase2;

    intBigData retourPlus;
    int i2; // parcourir le retourPlus
    qint64 retenue;
    for (qint64 i = v.size() - 1; i >= 0; i--) {

        baseCase2 = baseCase.at(baseCasei);
        baseCasei--;
        retourPlus = {};
        while (baseCase2 > 0) {
            retourPlus.append(baseCase2 % baseTo); // toujours qu'une case à ajouter
            baseCase2 /= baseTo;
        }
        //on fait: intBigB baseCase(intBigB(intBig::base, base)^i);
        //on fait: retour += pow(baseintBig::base, ) * value.at(i);
        retenue = 0;
        for (i2 = 0; i2 < retourPlus.size() || retenue != 0; i2++) {
            if (retour.size() <= i2)
                retour.append(0);
            if (i2 < retourPlus.size())
                retour[i2] += retourPlus.at(i2) * v.at(i);
            retour[i2] += retenue;
            retenue = retour[i2] / baseTo;
            retour[i2] %= baseTo;
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
