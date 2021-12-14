#include "grade.h"

Grade::Grade(QObject *parent) : QObject(parent), SqlMeta(-1)
{

}

Grade::Grade(int id, QDate date, int mark, Subject *sub, Student *stud, Teacher *teacher, QObject *parent):
    QObject(parent), SqlMeta(id), date_receive(date), mark(mark), sub(sub), stud(stud), teacher(teacher)
{

}

void Grade::setDateReceive(QDate date)
{
    this->date_receive = date;
}

void Grade::setMark(int m)
{
    this->mark = m;
}

void Grade::setSub(Subject * s)
{
    this->sub = s;
}

void Grade::setStud(Student * s)
{
    this->stud = s;
}

void Grade::setTeacher(Teacher * t)
{
    this->teacher = t;
}

QDate Grade::getDateReceive()
{
    return date_receive;
}

int Grade::getMark()
{
    return mark;
}

Subject *Grade::getSub()
{
    return sub;
}

Student *Grade::getStud()
{
    return stud;
}

Teacher *Grade::getTeacher()
{
    return teacher;
}
