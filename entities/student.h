#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>
#include <QDate>
#include "utils/sqlmeta.h"
#include "entities/person.h"
#include "entities/grade.h"

class Group;

class Student : public Person, public SqlMeta
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);
    Student(int id, QString s_name, QString f_name, QString pat,
            Group* const group, QDate dob, QString phone,
            QString email, QObject *parent = nullptr);
    Student(int id, QString s_name, QString f_name, QString pat,
            Group* const group, QDate dob, QString phone,
            QString email, QHash<int, Grade*> gr, QObject *parent = nullptr);

    void setGrades(QHash<int, Grade*> gr);
    void setGroup(Group *const n_gr);
    virtual void setDOB(QDate dob) override;
    virtual void setPhone(QString phone) override;
    virtual void setEmail(QString email) override;

    QList<Grade*> getGrades();
    Group* getGroup();
    virtual QDate getDOB() override;
    virtual QString getPhone() override;
    virtual QString getEmail() override;

    bool addGrade(int id, Grade *n_gr);
    bool deleteGrade(int gr);
private:
    Group* group;
    QDate dob;
    QString phone;
    QString email;
    QHash<int, Grade*> grades;
};

#endif // STUDENT_H
