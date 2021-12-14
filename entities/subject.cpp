#include "subject.h"

Subject::Subject(QObject *parent) : QObject(parent), SqlMeta(-1)
{
    
}

Subject::Subject(int id, QString name, int hours, CheckType ch_type, QObject *parent) :
    QObject(parent), SqlMeta(id), name(name), hours(hours), ch_type(ch_type)
{

}

Subject::Subject(int id, QString name, int hours, CheckType ch_type, QHash<int, Teacher *> teachers, QObject *parent):
    QObject(parent), SqlMeta(id), name(name), hours(hours), ch_type(ch_type), teachers(teachers)
{

}

void Subject::setName(QString name)
{
    this->name = name;
}

void Subject::setHours(int h)
{
    this->hours = h;
}

void Subject::setCheckType(CheckType t)
{
    this->ch_type = t;
}

void Subject::setTeachers(QHash<int, Teacher*> tech)
{
    this->teachers = tech;
}

QString Subject::getName()
{
    return name;
}

int Subject::getHours()
{
    return hours;
}

Subject::CheckType Subject::getCheckType()
{
    return ch_type;
}

QList<Teacher *> Subject::getTeachers()
{
    return teachers.values();
}

bool Subject::addNewTeacher(int id_th, Teacher *const n_th)
{
    if (!n_th || id_th < 0)
        return false;

    //call signal
    teachers.insert(id_th, n_th);
    return true;
}

bool Subject::deleteTeacher(int id_th)
{
    if(id_th < 0)
        return false;

    auto it = teachers.constFind(id_th);
    if (it == teachers.constEnd())
        return false;

    //call signal
    teachers.erase(it);
    return true;
}
