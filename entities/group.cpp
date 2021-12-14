#include "group.h"

Group::Group(QObject *parent) : QObject(parent), SqlMeta(-1, SqlState::Invalid)
{

}

Group::Group(int id, QString name, Faculty* fac, QObject *parent):
    QObject(parent), SqlMeta(id), name(name), fac(fac)
{

}

Group::Group(int id, QString name, Faculty* fac, QHash<int,Student*> studs, QHash<int,Subject*> subs, QObject *parent):
    QObject(parent), SqlMeta(id), name(name), fac(fac), studs(studs), subs(subs)
{

}

void Group::setStuds(QHash<int,Student*> studs)
{
    this->studs = studs;
}

void Group::setSubs(QHash<int, Subject *> subs)
{
    this->subs = subs;
}

void Group::setName(QString name)
{
    this->name = name;
}

QString Group::getName()
{
    return name;
}

Faculty *Group::getFaculty()
{
    return fac;
}

QList<Student*> Group::getStuds()
{
    return studs.values();
}

QList<Subject *> Group::getSubs()
{
    return subs.values();
}

int Group::countOfStuds()
{
    return static_cast<int>(this->studs.count());
}

bool Group::addNewStud(Student *n_stud)
{
    if (!n_stud)
        return false;

    studs.insert(n_stud->id(), n_stud);
    return true;
}

bool Group::deleteStud(int id_stud)
{
    if(id_stud < 0)
        return false;

    auto it = studs.constFind(id_stud);
    if (it == studs.constEnd())
        return false;

    it.value()->setState(SqlMeta::SqlState::Deleted);
    studs.erase(it);
    return true;
}

bool Group::addNewSubject(Subject *n_sub)
{
    if (!n_sub)
        return false;

    subs.insert(n_sub->id(), n_sub);
    return true;
}

bool Group::deleteSubject(int id_sub)
{
    if(id_sub < 0)
        return false;

    auto it = subs.constFind(id_sub);
    if (it == subs.constEnd())
        return false;

    it.value()->setState(SqlMeta::SqlState::Deleted);
    subs.erase(it);
    return true;
}
