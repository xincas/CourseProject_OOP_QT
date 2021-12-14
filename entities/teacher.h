#ifndef TEACHER_H
#define TEACHER_H

#include <QObject>
#include <QHash>
#include <QDate>
#include "utils/sqlmeta.h"
#include "entities/person.h"

class Subject;

class Teacher : public Person, public SqlMeta
{
    Q_OBJECT
public:
    explicit Teacher(QObject *parent = nullptr);
    Teacher(int id, QString s_name, QString f_name, QString pat, QString email, QObject *parent = nullptr);
    Teacher(int id, QString s_name, QString f_name, QString pat,
            QString email, QHash<int, Subject*> subs, QObject *parent = nullptr);


    void setSubs(QHash<int, Subject*> subs);
    virtual void setEmail(QString email) override;

    QList<Subject*> getSubs();
    virtual QString getEmail() override;

    bool addNewSubject(int id_sb, Subject *const n_sb);
    bool deleteSubject(int id_sb);

    void setDOB(QDate) override;
    void setPhone(QString) override;
    QDate getDOB() override;
    QString getPhone() override;


private:
    QString email;
    QHash<int, Subject*> subs;
};

#endif // TEACHER_H
