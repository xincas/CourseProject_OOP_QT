#ifndef SUBJECT_H
#define SUBJECT_H

#include <QObject>
#include <QHash>
#include "utils/sqlmeta.h"

class Teacher;

class Subject : public QObject, public SqlMeta
{
    Q_OBJECT
public:
    enum CheckType {
        Credit,
        Exam,
        CourseProject
    };

    explicit Subject(QObject *parent = nullptr);
    Subject(int id, QString name, int hours, CheckType ch_type, QObject *parent = nullptr);
    Subject(int id, QString name, int hours, CheckType ch_type, QHash<int, Teacher*> teachers, QObject *parent = nullptr);

    void setName(QString name);
    void setHours(int h);
    void setCheckType(CheckType t);
    void setTeachers(QHash<int, Teacher*> tech);

    QString  getName();
    int getHours();
    CheckType getCheckType();
    QList<Teacher*> getTeachers();

    bool addNewTeacher(int id_th, Teacher *const n_th);
    bool deleteTeacher(int id_th);

private:
    QString name;
    int hours;
    CheckType ch_type;
    QHash<int, Teacher*> teachers;
};

#endif // SUBJECT_H
