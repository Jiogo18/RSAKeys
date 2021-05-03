//  /!\ ce fichier contient des debugs uniquement pour le test, vous risquez d'etre spammé
#include "intBig4Parent.h"
#include "debug.h"

intBig4Parent::intBig4Parent(quint64 value)
{
    baseP=2;//on sait pas quoi mettre d'autre
    operator =(value);
}
intBig4Parent::intBig4Parent(quint64 baseV, quint64 value)
{
    baseP=baseV;
    operator =(value);
}
intBig4Parent::intBig4Parent(QString value, quint64 base)
{baseP=base; negatifP=false; operator =(fromText(value, base));}
intBig4Parent::intBig4Parent(QVector<quint64> value, quint64 base)
{baseP=base; negatifP=false; valeurP=value; resize();}
intBig4Parent::intBig4Parent(const intBig4Parent &value)
{operator =(value);}
intBig4Parent::intBig4Parent(const intBig4Parent *value)
{operator =(*value);}

void intBig4Parent::resize()
{
    if(baseP < 2)
    {
        qDebug("intBig4Parent.cpp : base à 0 ou 1 resize");
        setBase(2);
    }
    //actualise les cases
    quint64 i;
    for(i=size(); i>0; i--)
    {
        if(at(i-1) != 0)
            break;
        valeurP.removeAt(i-1);//si c'est 0 (vide)
    }

    i=0;
    quint64 retenue=0;
    while(i<sizeNNul() || retenue>0)
    {
        while(i >= size())
            valeurP.append(0);
        operator [](i) +=retenue;
        retenue = at(i)/base();
        operator [](i) %= base();
        i++;
    }
}

intBig4Parent intBig4Parent::fromText(QString value, quint64 base)//from text
{
    intBig4Parent retour(base, 0);
    if(value.startsWith("-"))
    {
        retour.setNegative(true);
        value.remove("-");
    }
    else
        retour.setNegative(false);
    if(base == 10 || base==16 || base==64 || base==100)
    {
        for(int i=0; i<value.size(); i++)
            retour[i] += strOfBase(base).indexOf(value[value.size()-i-1]);
    }
    else//en ascii, continuer si besoin de faire avec les séparation points
        for(int i=0; i<value.size(); i++)
            retour[i] += value.at(i).unicode();

    retour.resize();
    return retour;
}
intBig4Parent intBig4Parent::fromVector(QVector<quint64> value, quint64 base)
{
    intBig4Parent retour(base, 0);
    retour.setValue(value, base);
    return retour;
}

void intBig4Parent::setNegative(bool negative) {negatifP=negative;}
void intBig4Parent::setValue(QVector<quint64> value, quint64 base)
{
    valeurP=value;
    baseP=base;
    resize();
}
void intBig4Parent::setBase(quint64 base) {operator =(toBase(base));}

QString intBig4Parent::toString(quint64 base) const
{
    QString retour="";
    intBig4Parent me2(toBase(base));
    if(base==10 || base==16 || base==64 || base==100)
    {
        for(quint64 i=me2.size(); i>0; i--)
            retour.append(strOfBase(base).at(me2.at(i-1)));
    }
    else
        for(quint64 i=0; i<me2.size(); i++)
            retour.replace(i, 1, QString::number(me2.at(me2.size()-i-1), base));
            //en ascii (normalement base ne sert a rien mais sécurité)
    if(retour.isEmpty())
        retour="0";
    if(isNegative())
        retour.push_front("-");
    return retour;
}
QVector<quint64> intBig4Parent::toVector(quint64 base) const
{
    if(base==baseP)
        return valeurP;
    intBig4Parent retour(toBase(base));
    if(base != retour.base())
        qDebug("intBig4Parent.cpp : erreur toBase (toString)");
    return retour.toVector(base);//rappel de la fonction mais juste pour donner valeurP
}
intBig4Parent intBig4Parent::toBase(quint64 base) const
{//conversion de base
    intBig4Parent retour(base, 0);
    if(intBig4Parent::base() == base)//si c les meme bases
        return intBig4Parent(valeurP, base);
    if(isEmpty())//si c'est vide
        return retour;

    intBig4Parent valueCase(base, 0);
    intBig4Parent baseCase(base, 0);
    for(quint64 i=sizeNNul(); i>0; i--)
    {
        valueCase=at(i-1);//trop lent avec le pow()
        if(!valueCase.isEmpty())
        {//si c'est 0 ça sert a rien
            baseCase=intBig4Parent::base();
            baseCase = baseCase^(i-1);
            retour += baseCase * valueCase;
        }
    }
    retour.resize();
    return retour;
}
bool intBig4Parent::isEmpty() const {return sizeNNul() == 0;}

quint64 intBig4Parent::sizeNNul() const
{
    quint64 size=intBig4Parent::size();
    while(size>0 && at(size-1) == 0)
        size--;//si il reste des cases et que celle en size-1 est nulle
    return size;
}


const quint64 &intBig4Parent::at(const quint64 i) const
{
    if(i>=size())//si existe pas, c'est 0
        return retourNul;
    return valeurP.at(i);
}
quint64 &intBig4Parent::operator[](const quint64 i)
{
    while(i >= size())//si existe pas, on agrandit
        valeurP.append(0);//pour quand on set la case avec une new valeur
    return valeurP[i];
}
void intBig4Parent::operator ++(int) {operator +=(1);}
void intBig4Parent::operator --(int) {operator -=(1);}
intBig4Parent intBig4Parent::sqrt() const
{
    if(isNegative())
    {
        qDebug("erreur sqrt, racine d'un nombre négatif");
        return intBig4Parent(base(), 0);
    }
    if(isEmpty())
        return intBig4Parent(base(), 0);//0²=0
    if(operator ==(1))
        return intBig4Parent(base(), 1);//1²=1

    intBig4Parent min(base(), 1);
    intBig4Parent retour(min);//commencer par la
    intBig4Parent fois(operator /(2));
    while(fois>1)
    {
        while(operator >=(retour*retour))
        {//augmentation au fur et à mesure
            min=retour;
            retour=retour*fois;
        }
        retour=min;
        fois /= 20;
    }

    intBig4Parent plus(retour);
    while(plus > 0)
    {
        while(operator >=(retour*retour))
        {//augmentation au fur et à mesure
            min=retour;
            retour += plus;
        }
        retour=min;
        if(plus < 20 && plus>1)
        {
            plus=1;//+1
        }
        else
            plus /= 20;
    }
    //ici retour² est == ou au dessous de this
    return retour;
}
bool intBig4Parent::isPrime(QProgressBar *ch) const
{
    if(toVector(2).at(0)==0)
        return false;
    intBig4Parent sqrtNb=sqrt();
    ch->setMinimum(1);
    ch->setMaximum(sqrtNb.toString().size());
    ch->setValue(1);
    for(intBig4Parent i=3; i<=sqrtNb; i+=2)
    {//les pairs ont déja ete testé
        if(operator %(i) == 0)
            return false;
        ch->setValue(i.toString().size());
        QCoreApplication::processEvents();
    }
    ch->setValue(ch->maximum());
    return true;
}


intBig4Parent intBig4Parent::operator ^(intBig4Parent v) const
{
    intBig4Parent retour(base(), 1);
    intBig4Parent retour2(*this);
    while(v>0)
    {
        retour *= retour2;//arranger avec prog calculette (coder-decoder)
        v--;
    }
    return retour;
}
intBig4Parent intBig4Parent::operator ^(quint64 v) const
{
    intBig4Parent retour(base(), 1);
    intBig4Parent retour2(*this);
    while(v>0)
    {
        retour *= retour2;//arranger avec prog calculette (coder-decoder)
        v--;
    }
    return retour;
}
intBig4Parent intBig4Parent::operator %(intBig4Parent v) const
{
    intBig4Parent retour(base(), 0);
    if(v==1 || v==0)//ça donnera 0 (avec v==0 normalement c'est non défini mais c'est généralement %0.1)
        return retour;
    intBig4Parent v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    retour=operator -( (operator /(v2))*v2 );
    while(retour >= v2)//compense l'opération en haut, normalement pas utile
    {
        qDebug()<<"intBig : erreur %"<<toString()<<v2.toString()<<"donne"<<retour.toString();
        retour -= v2;//(algo de modulo de base mais trop lent donc juste pour resize correctement)
    }
    while(retour.isNegative() && retour<0)
        retour += v2;//si il est encore négatif, on le remonte d'un (ex:-9%4 = -1 => 3)
    retour.resize();
    return retour;
}
intBig4Parent intBig4Parent::operator *(const intBig4Parent &v) const
{
    intBig4Parent retour(base(), 0);
    intBig4Parent v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    for(quint64 i=0; i<size(); i++)
    {
        for(quint64 i2=0; i2<v2.size(); i2++)
        {
            quint64 mult=at(i)*v2.at(i2);
            quint64 retenueCase=0;
            while(mult>0)
            {
                retour[i+i2+retenueCase] += mult % base();
                mult /= base();//restant
                retenueCase++;
            }
        }
    }
    retour.setNegative(isNegative() != v2.isNegative());
    retour.resize();
    return retour;
}
intBig4Parent intBig4Parent::operator *(quint64 v) const
{
    //convertir en base v et ensuite ajouter une case
    QVector<quint64> retour = toVector(v);
    retour.insert(0, 0);//ajoute au début
    intBig4Parent retour2(retour, v);//on le repasse en intBig4Parent
    retour2.setNegative(isNegative());//il y a qu moi qui peut etre -
    retour2.setBase(base());
    return retour2;
}
intBig4Parent intBig4Parent::operator /(const intBig4Parent &v) const
{//division
    intBig4Parent retour(base(), 0);
    if(isEmpty())//forcement 0
        return retour;
    if(v.isEmpty())//1/0
    {
        qDebug(QString("erreur division, le dénominateur vaut 0 "+v.toString()).toStdString().c_str());
        return retour;
    }

    intBig4Parent v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    quint64 i=sizeNNul();//le plus grand quint64
    quint64 i2=v2.sizeNNul();//le plus grand quint64
    if(operator <(v2))
    {//si c plus grand la division fera 0.xxx
        //si seulement un seul est -, c'est -
        retour.setNegative(isNegative() != v2.isNegative());
        return retour;
    }

    intBig4Parent plus(base(), 0);
    intBig4Parent min(base(), 0);
    plus[sizeNNul()-1] = at(sizeNNul()-1);
    quint64 start=QDateTime::currentMSecsSinceEpoch();
    retour=min;
    while(plus > 0)
    {
        intBig4Parent retour2(retour*v2);
        intBig4Parent plus2(v2*plus);//plus optimisé
        while(operator >=(retour2))
        {
            min=retour;
            //retour[plus.sizeNNul()-1] += plus[plus.sizeNNul()-1];
            retour += plus;
            retour2 += plus2;
        }
        retour=min;

        if(plus.sizeNNul() > 1 || plus >= 20)
        {
            quint64 i3=plus.sizeNNul();
            if(plus[i3-1] >= 20 || i3==1)
            {
                plus[i3-1] /= 20;//divise juste le quint64 de la case
            }
            else
            {
                plus[i3-2] += (base()*plus[i3-1]) / 20;
                plus[i3-1] = 0;
            }
        }
        else if(plus==1)
        {
            plus=0;//si on est déja en +1 on s'arrete
        }
        else//faire du +1
            plus=1;
    }
    //si seulement un seul est -, c'est -
    retour.setNegative(isNegative() != v2.isNegative());
    debug::stat("/", start, QDateTime::currentMSecsSinceEpoch());
    return retour;
}
intBig4Parent intBig4Parent::operator /(quint64 v) const
{
    //convertir en base v et ensutie retirer la premiere case
    QVector<quint64> retour = toVector(v);
    retour.pop_front();//retire le premier
    intBig4Parent retour2(retour, v);//on le repasse en intBig4Parent

    retour2.setNegative(isNegative());//il y a qu moi qui peut etre -
    retour2.setBase(base());

    return retour2;
}
intBig4Parent intBig4Parent::operator +(intBig4Parent v) const
{
    if(isNegative() && v.isNegative())
    {// - "+" - = -(+ "+" +)
        intBig4Parent t(valeurP, base());
        t.setNegative(!isNegative());
        v.setNegative(!v.isNegative());
        t+=v;
        t.setNegative(!t.isNegative());//change d'etat
        return t;
    }
    if(isNegative())//- "+" + = + "-" +
    {
        intBig4Parent t(valeurP, base());
        t.setNegative(!isNegative());
        return v - t;
    }
    if(v.isNegative())//+ "+" - = + "-" +
    {
        v.setNegative(!v.isNegative());
        return operator -(v);
    }
    intBig4Parent v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    intBig4Parent retour(base(), 0);
    for(quint64 i=0; i<sizeNNul() || i<v2.sizeNNul() || i<retour.sizeNNul(); i++)
    {
        retour[i] += at(i) + v2.at(i);
        if(retour[i] >= base())//si c trop grand
        {
            retour[i+1] += retour.at(i)/base();//retenue
            retour[i] %= base();//retire les retenues
        }
    }
    return retour;
}
intBig4Parent intBig4Parent::operator +(quint64 v) const
{
    intBig4Parent retour(this);
    retour[0] += v;
    retour.resize();
    return retour;
}
intBig4Parent intBig4Parent::operator -(intBig4Parent v) const
{
    if(isNegative() || (isNegative() && v.isNegative()))
    {//inverser ou - "-" - = -(+ "-" +)
        intBig4Parent t(valeurP, base());
        t.setNegative(!isNegative());
        v.setNegative(!v.isNegative());//on le retourne
        t-=v;
        t.setNegative(!t.isNegative());
        return t;
    }
    if(v.isNegative())
    {//+ "-" - = + "+" +
        v.setNegative(!v.isNegative());
        return operator +(v);
    }

    intBig4Parent v2(v);
    if(base() != v.base())
        v2=v.toBase(base());
    if(operator <(v2))
    {//si v est plus grand  + "-" + = -(- "-" -) =-(- "+" +)= -(+"-"+)
        intBig4Parent t(valeurP, base());
        t=v2-t;
        t.setNegative(!t.isNegative());
        return t;
    }
    intBig4Parent retour(valeurP, base());
    quint64 retenue=0;
    for(quint64 i=0; i<sizeNNul() ; i++)
    {//je suis forcement plus grand donc size() est le max
        if(retour.at(i) < v2.at(i)+retenue)
        {
            quint64 retenueT=0;
            while(retour.at(i) < v2.at(i)+retenue)
            {
                retour[i] += base();
                retenueT++;
            }
            retour[i] -= (v2.at(i) + retenue);
            retenue = retenueT;
        }
        else
        {
            retour[i] -= (v2.at(i) + retenue);
            retenue=0;
        }
    }
    return retour;
}
intBig4Parent intBig4Parent::operator -(quint64 v) const
{
    if(isNegative())
    {//si je suis negatif
        intBig4Parent t(valeurP, base());//     - "-" +
        t.setNegative(!isNegative());        //=> -(+ "+" -)
        t=t-v;                          //=> -(+ "-" +)
        t.setNegative(!t.isNegative());
        return t;
    }
    intBig4Parent retour(valeurP, base());
    quint64 retenue=0;
    for(quint64 i=0; i<size() ; i--)
    {
        if(retour.at(i) < v+retenue)
        {
            quint64 retenueT=0;
            while(retour.at(i) < v+retenue)
            {
                retour[i] += base();
                retenueT++;
            }
            retour[i] -= (v + retenue);
            v=0;//ici un quint64 on a du le vider entièrement
            retenue = retenueT;
        }
        else
        {
            retour[i] -= (v + retenue);
            retenue=0;
            v=0;//pareil
            break;//si on arrive la c'est surrement parce que il y a plus rien
        }
    }
    return retour;
}


void intBig4Parent::operator %=(const intBig4Parent &v) {operator =(operator %(v));}
void intBig4Parent::operator %=(quint64 v) {operator =(operator %(v));}
void intBig4Parent::operator *=(const intBig4Parent &v) {operator =(operator *(v));}
void intBig4Parent::operator *=(quint64 v) {operator =(operator *(v));}
void intBig4Parent::operator /=(const intBig4Parent &v) {operator = (operator /(v));}
void intBig4Parent::operator /=(quint64 v) {operator = (operator /(v));}
void intBig4Parent::operator +=(const intBig4Parent &v) {operator =(operator +(v));}
void intBig4Parent::operator +=(quint64 v) {operator =(operator +(v));}
void intBig4Parent::operator -=(const intBig4Parent &v) {operator =(operator -(v));}
void intBig4Parent::operator -=(quint64 v) {operator =(operator -(v));}

bool intBig4Parent::operator !=(const intBig4Parent &v) const
{
    return !operator ==(v);
    /*if(isNegative() != v.isNegative())
        return true;
    intBig4Parent v2(v);
    if(base() != v.base())
        v2.setBase(base());
    if(sizeNNul() != v2.sizeNNul())
        return true;
    for(quint64 i=0; i<sizeNNul(); i++)
        if(at(i) != v2.at(i))//si ils sont diff à un moment, c'est bon
            return true;
    return false;//si ils sont diff nul part, ils sont pareils*/
}
bool intBig4Parent::operator !=(quint64 v) const {return operator !=(intBig4Parent(base(), v));}
bool intBig4Parent::operator ==(const intBig4Parent &v) const
{
    if(isNegative()!=v.isNegative())
        return false;
    intBig4Parent v2(v);
    if(base() != v.base())
        v2.setBase(base());
    if(sizeNNul() != v2.sizeNNul())
        return false;
    for(quint64 i=0; i<sizeNNul(); i++)
        if(at(i) != v2.at(i))
            return false;
    return true;
}
bool intBig4Parent::operator ==(quint64 v) const {return operator ==(intBig4Parent(base(), v));}
bool intBig4Parent::operator <(const intBig4Parent &v) const
{
    if(isNegative() != v.isNegative())
    {
        return isNegative() && !v.isNegative();
    }
    intBig4Parent v2(v);
    if(base() != v.base())
        v2.setBase(base());
    if(sizeNNul() != v2.sizeNNul())
    {
        if(isNegative() && v2.isNegative())
        {
            return !(sizeNNul() < v2.sizeNNul());
        }
        else return sizeNNul() < v2.sizeNNul();
    }

    for(quint64 i=sizeNNul(); i>0; i--)
    {
        if(at(i-1) != v2.at(i-1))
        {
            if(isNegative() && v2.isNegative())
            {
                return !(at(i-1) < v2.at(i-1));
            }
            else return at(i-1) < v2.at(i-1);
        }
    }
    return false;//est egal
}
bool intBig4Parent::operator <(quint64 v) const {return operator <(intBig4Parent(base(), v));}
bool intBig4Parent::operator <=(const intBig4Parent &v) const {return operator <(v) || operator ==(v);}
bool intBig4Parent::operator <=(quint64 v) const {return operator <=(intBig4Parent(base(), v));}
bool intBig4Parent::operator >(const intBig4Parent &v) const {return !operator <=(v);}
bool intBig4Parent::operator >(quint64 v) const {return operator >(intBig4Parent(base(), v));}
bool intBig4Parent::operator >=(const intBig4Parent &v) const {return !operator <(v);}
bool intBig4Parent::operator >=(quint64 v) const {return operator >=(intBig4Parent(base(), v));}

void intBig4Parent::operator =(const intBig4Parent &v)
{
    baseP=v.base();
    if(baseP < 2)
    {
        qDebug("intBig4Parent : base à 0 ou 1 (operator =)");
        baseP=2;
    }
    valeurP=v.toVector(baseP);
    negatifP=v.isNegative();
    resize();
}
void intBig4Parent::operator =(quint64 v)
{
    valeurP.clear();
    negatifP=false;
    operator [](0) = v;
    resize();
}

const QString intBig4Parent::strOfBase(quint64 base)
{
    switch (base) {
    case 10:
        return "0123456789";
    case 16:
        return "0123456789abcdef";
    case 64:
        return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ&";
    case 100:
        return"☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼ !e#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[f]^_`abcd";
    default:
        return "";
    }
}

QString intBig4Parent::debug() const
{
    QString retour="Base:"+QString::number(base())+", ";
    if(isNegative())
        retour += "-";
    retour += "{";
    for(quint64 i=0; i<size(); i++)
    {
        retour += QString::number(at(i));
        if(i+1 < size())
            retour += ", ";
    }
    return "intBig4Parent { "+retour+"} }";
}
