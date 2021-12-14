#include "faculty.h"

Faculty::Faculty(QObject *parent) : QObject(parent), SqlMeta(-1, SqlState::Invalid)
{

}

Faculty::Faculty(int id, QString name, QObject *parent):
    QObject(parent), SqlMeta(id), name(name)
{

}

Faculty::Faculty(int id, QString name, QHash<int, Group *> gr, QObject *parent):
    QObject(parent), SqlMeta(id), name(name), gr(gr)
{

}

void Faculty::setGroups(QHash<int, Group *> gr)
{
    this->gr = gr;
}

void Faculty::setName(QString name)
{
    this->name = name;
}

QString Faculty::getName()
{
    return name;
}

int Faculty::countOfGroups()
{
    return static_cast<int>(this->gr.size());
}

QList<Group *> Faculty::getGroups()
{
    return gr.values();
}

bool Faculty::addNewGroup(Group *n_gr)
{
    if (!n_gr)
        return false;

    //if(gr.insert(n_gr->getName(), n_gr))
    gr.insert(n_gr->id(), n_gr);
    return true;
}

bool Faculty::deleteGroup(int id_gr)
{
    if(id_gr < 0)
        return false;

    auto f = gr.constFind(id_gr);
    if(f == gr.constEnd())
        return false;

    f.value()->setState(SqlState::Deleted);
    gr.erase(f);
    return true;
}

bool Faculty::deleteGroup(QString name_gr)
{
    if(name_gr.isEmpty())
        return false;

    QList<Group*> gr_list = gr.values();

    for(auto gr_o: gr_list) {
        if (gr_o->getName() == name_gr) {
            auto it = gr.constFind(gr_o->id());
            it.value()->setState(SqlState::Deleted);
            gr.erase(it);

            return true;
        }
    }
    return false;
}
