#include "gradetablemodel.h"

GradeTableModel::GradeTableModel(QList<Grade*> src,QObject *parent)
    : QAbstractTableModel(parent), source(src)
{
}

QVariant GradeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("ID");
        } else if (section == 1) {
            return tr("Оценка");
        } else if (section == 2) {
            return tr("Дата получения");
        } else if (section == 3) {
            return tr("Дисциплина");
        } else if (section == 4) {
            return tr("Преподаватель");
        } else {
            return QVariant();
        }
    }
    return QVariant();
}

int GradeTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return source.count();
}

int GradeTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}

QVariant GradeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    Grade* data_ = source.at(index.row());
    if (!data_)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return data_->id();
        case 1:
            return data_->getMark();
        case 2:
            return data_->getDateReceive().toString(Qt::ISODate);
        case 3:
            return data_->getSub()->getName();
        case 4:
            return data_->getTeacher()->getFullName();
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QList<Grade*> GradeTableModel::getSource()
{
    return source;
}

void GradeTableModel::refreshData(QList<Grade *> grades)
{
    beginResetModel();
    source = grades;
    endResetModel();
}
