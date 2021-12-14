#include "facultylistmodel.h"

FacultyListModel::FacultyListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

FacultyListModel::FacultyListModel(QList<Faculty *> *faculties, QObject *parent):
    QAbstractListModel(parent), faculties(faculties)
{

}

QVariant FacultyListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int FacultyListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    return faculties->count();

    // FIXME: Implement me!
}

QVariant FacultyListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return QString::number(faculties->at(index.row())->id()) + " | " + faculties->at(index.row())->getName();

    default:
        return QVariant();
    }
}

QList<Faculty *> *FacultyListModel::getFaculties()
{
    return this->faculties;
}

void FacultyListModel::refreshData(QList<Faculty *> *fac)
{
    beginResetModel();
    faculties = fac;
    endResetModel();
}
