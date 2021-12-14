#include "teacher.h"

Teacher::Teacher(QObject *parent):
    Person(parent), SqlMeta(-1)
{

}

Teacher::Teacher(int id, QString s_name, QString f_name, QString pat, QString email, QObject *parent):
    Person(s_name, f_name, pat, parent), SqlMeta(id), email(email)
{

}

Teacher::Teacher(int id, QString s_name, QString f_name, QString pat, QString email, QHash<int, Subject *> subs, QObject *parent):
    Person(s_name, f_name, pat, parent), SqlMeta(id), email(email), subs(subs)
{

}

void Teacher::setSubs(QHash<int, Subject *> subs)
{
    this->subs = subs;
}

void Teacher::setEmail(QString email)
{
    this->email = email;
}

QList<Subject *> Teacher::getSubs()
{
    return subs.values();
}

QString Teacher::getEmail()
{
    return email;
}

bool Teacher::addNewSubject(int id_sb, Subject * const n_sb)
{
    if (!n_sb || id_sb < 0)
        return false;

    //call signal
    subs.insert(id_sb, n_sb);
    return true;
}

bool Teacher::deleteSubject(int id_sb)
{
    if(id_sb < 0)
        return false;

    auto it = subs.constFind(id_sb);
    if (it == subs.constEnd())
        return false;

    //call signal
    subs.erase(it);
    return true;
}

void Teacher::setDOB(QDate /*date*/)
{
    return;
}

void Teacher::setPhone(QString)
{
    return;
}

QDate Teacher::getDOB()
{
    return QDate::currentDate();
}

QString Teacher::getPhone()
{
    return QString("");
}
