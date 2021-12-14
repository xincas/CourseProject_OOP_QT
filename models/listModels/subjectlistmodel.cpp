#include "subjectlistmodel.h"

SubjectListModel::SubjectListModel(QList<Subject*> src, QObject *parent)
    : QAbstractListModel(parent), source(src)
{
}

QVariant SubjectListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int SubjectListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    return source.count();

    // FIXME: Implement me!
}

QVariant SubjectListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Subject* sb = source.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return sb->getName() + " | " + UtilFaculty::checkTypeToStr(sb->getCheckType());
    default:
        return QVariant();
    }
}

QList<Subject *> SubjectListModel::getSource()
{
    return source;
}

void SubjectListModel::refreshData(QList<Subject *> subs)
{
    beginResetModel();
    source = subs;
    endResetModel();
}
