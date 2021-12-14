#include "teacherlistmodel.h"

TeacherListModel::TeacherListModel(QList<Teacher*> src,QObject *parent)
    : QAbstractListModel(parent), source(src)
{
}

QVariant TeacherListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int TeacherListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    return source.count();

    // FIXME: Implement me!
}

QVariant TeacherListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return source.at(index.row())->getFullName();

    default:
        return QVariant();
    }
}

QList<Teacher *> TeacherListModel::getSource()
{
    return source;
}

void TeacherListModel::refreshData(QList<Teacher *> tchs)
{
    beginResetModel();
    source = tchs;
    endResetModel();
}
