#include "grouplistmodel.h"

GroupListModel::GroupListModel(QList<Group*>* gr, QObject *parent)
    : QAbstractListModel(parent), source(gr)
{
}

QVariant GroupListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int GroupListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    return source->count();

    // FIXME: Implement me!
}

QVariant GroupListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return source->at(index.row())->getName();

    default:
        return QVariant();
    }
}

void GroupListModel::refreshData(QList<Group *> *gr)
{
    beginResetModel();
    source = gr;
    endResetModel();
}
