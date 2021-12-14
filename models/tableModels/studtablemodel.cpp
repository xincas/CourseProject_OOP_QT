#include "studtablemodel.h"

StudTableModel::StudTableModel(QList<Student*>* src, QObject *parent)
    : QAbstractTableModel(parent), source(src)
{
}

QVariant StudTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("ID");
        } else if (section == 1) {
            return tr("Группа");
        } else if (section == 2) {
            return tr("ФИО");
        } else {
            return QVariant();
        }
    }
    return QVariant();
}

int StudTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return source->count();
}

int StudTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant StudTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    Student* data_ = source->at(index.row());
    if (!data_)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return data_->id();
        case 1:
            return data_->getGroup()->getName();
        case 2:
            return data_->getFullName();
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QList<Student *> *StudTableModel::getStuds()
{
    return this->source;
}

void StudTableModel::refreshData(QList<Student *> *studs)
{
    beginResetModel();
    source = studs;
    endResetModel();
}
