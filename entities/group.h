#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include "entities/student.h"
#include "entities/subject.h"

class Faculty;

class Group : public QObject, public SqlMeta
{
    Q_OBJECT
public:
    explicit Group(QObject *parent = nullptr);
    Group(int id, QString name, Faculty* fac, QObject *parent = nullptr);
    Group(int id, QString name, Faculty* fac, QHash<int,Student*> studs, QHash<int,Subject*> subs, QObject *parent = nullptr);

    void setStuds(QHash<int,Student*> studs);
    void setSubs(QHash<int,Subject*> subs);
    void setName(QString name);

    QString getName();
    Faculty* getFaculty();
    QList<Student*> getStuds();
    QList<Subject*> getSubs();
    int countOfStuds();
    bool addNewStud(Student* n_stud);
    bool deleteStud(int id_stud = -1);
    bool addNewSubject(Subject* n_sub);
    bool deleteSubject(int id_sub = -1);

private:
    QString name;
    Faculty* fac;
    QHash<int, Student*> studs;
    QHash<int, Subject*> subs;
};

#endif // GROUP_H
