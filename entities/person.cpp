#include "person.h"

Person::Person(QObject *parent) : QObject(parent)
{
    this->f_name = QString("");
    this->s_name = QString("");
    this->pat = QString("");
}

Person::Person(QString s_name, QString f_name, QString pat, QObject *parent):
    QObject(parent), s_name(s_name), f_name(f_name), pat(pat)
{

}

void Person::setFirstName(QString f_name)
{
    this->f_name = f_name;
}

void Person::setSecondName(QString s_name)
{
    this->s_name = s_name;
}

void Person::setPatName(QString pat)
{
    this->pat = pat;
}

QString Person::getFullName()
{
    return QString(s_name + ' ' + f_name + ' ' + pat);
}

QString Person::getFirstName()
{
    return this->f_name;
}

QString Person::getSecondName()
{
    return this->s_name;
}

QString Person::getPatName()
{
    return this->pat;
}

