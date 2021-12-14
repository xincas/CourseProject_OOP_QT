#ifndef GRADE_H
#define GRADE_H

#include <QObject>
#include <QDate>
#include "utils/sqlmeta.h"

class Subject;
class Student;
class Teacher;

class Grade : public QObject, public SqlMeta
{
    Q_OBJECT
public:
    explicit Grade(QObject *parent = nullptr);
    Grade(int id, QDate date, int mark, Subject* sub, Student* stud, Teacher* teacher, QObject *parent = nullptr);

    void setDateReceive(QDate);
    void setMark(int);
    void setSub(Subject*);
    void setStud(Student*);
    void setTeacher(Teacher*);

    QDate getDateReceive();
    int getMark();
    Subject* getSub();
    Student* getStud();
    Teacher* getTeacher();

private:
    QDate date_receive;
    int mark;
    Subject* sub;
    Student* stud;
    Teacher* teacher;
};

#endif // GRADE_H
