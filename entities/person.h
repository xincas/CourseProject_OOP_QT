#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class Person : public QObject
{
    Q_OBJECT
public:
    explicit Person(QObject *parent = nullptr);
    Person(QString s_name, QString f_name, QString pat, QObject *parent = nullptr);

    void setFirstName(QString f_name);
    void setSecondName(QString s_name);
    void setPatName(QString pat);

    QString getFullName();
    QString getFirstName();
    QString getSecondName();
    QString getPatName();

    virtual void setDOB(QDate) = 0;
    virtual void setPhone(QString) = 0;
    virtual void setEmail(QString) = 0;
    virtual QDate getDOB() = 0;
    virtual QString getPhone() = 0;
    virtual QString getEmail() = 0;

private:
    QString s_name;
    QString f_name;
    QString pat;
};

#endif // PERSON_H
