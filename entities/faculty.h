#ifndef FACULTY_H
#define FACULTY_H

#include <QObject>
#include <QHash>
#include "utils/sqlmeta.h"
#include "entities/group.h"

class Faculty : public QObject, public SqlMeta
{
    Q_OBJECT
public:
    explicit Faculty(QObject *parent = nullptr);
    Faculty(int id, QString name, QObject *parent = nullptr);
    Faculty(int id, QString name, QHash<int,Group*> gr, QObject *parent = nullptr);

    void setGroups(QHash<int, Group*> gr);
    void setName(QString name);

    QString getName();
    QList<Group*> getGroups();
    int countOfGroups();
    bool addNewGroup(Group* n_gr);
    bool deleteGroup(int id_gr);
    bool deleteGroup(QString name_gr);

private:
    QString name;
    QHash<int,Group*> gr;

signals:

};

#endif // FACULTY_H
