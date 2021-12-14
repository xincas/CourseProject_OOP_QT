#include "student.h"

Student::Student(QObject *parent) : Person(parent), SqlMeta(-1, SqlState::Invalid)
{
    this->dob = QDate::currentDate();
    this->phone = QString("");
    this->email = QString("");
    this->group = nullptr;
}

Student::Student(int id, QString s_name, QString f_name, QString pat, Group* const group, QDate dob, QString phone, QString email, QObject *parent):
    Person(s_name, f_name, pat, parent), SqlMeta(id), group(group), dob(dob), phone(phone), email(email)
{

}

Student::Student(int id, QString s_name, QString f_name, QString pat,Group * const group,
                 QDate dob, QString phone, QString email, QHash<int, Grade*> gr, QObject *parent):
    Person(s_name, f_name, pat, parent), SqlMeta(id), group(group), dob(dob), phone(phone), email(email), grades(gr)
{

}

void Student::setGrades(QHash<int, Grade *> gr)
{
    this->grades = gr;
}

void Student::setGroup(Group * const n_gr)
{
    group = n_gr;
}

QDate Student::getDOB()
{
    return dob;
}

QString Student::getPhone()
{
    return phone;
}

QString Student::getEmail()
{
    return email;
}

bool Student::addGrade(int id, Grade *n_gr)
{
    if(!n_gr || id < 0)
        return false;

    grades.insert(id, n_gr);
    return true;
}

bool Student::deleteGrade(int gr)
{
    if(gr < 0)
        return false;

    auto g = grades.constFind(gr);
    if(g == grades.constEnd())
        return false;

    g.value()->setState(SqlState::Deleted);
    grades.erase(g);
    return true;
}

void Student::setDOB(QDate dob)
{
    this->dob = dob;
}

void Student::setPhone(QString phone)
{
    this->phone = phone;
}

void Student::setEmail(QString email)
{
    this->email = email;
}

QList<Grade *> Student::getGrades()
{
    return grades.values();
}

Group *Student::getGroup()
{
    return group;
}
