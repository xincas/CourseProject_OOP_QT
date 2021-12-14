#include "studentfiltermodel.h"

StudentFilterModel::StudentFilterModel()
{

}

void StudentFilterModel::updateFilterEnable(bool check)
{
    this->filter_by_gr = check;
    invalidateFilter();
}

void StudentFilterModel::updateFilterByGroup(QString gr)
{
    this->f_gr_ = gr;
    invalidateFilter();
}

void StudentFilterModel::updateFilterByName(QString fullName)
{
    this->f_name_ = fullName;
    invalidateFilter();
}

bool StudentFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index_gr = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex index_name = sourceModel()->index(source_row, 2, source_parent);
    QVariant gr_var = sourceModel()->data(index_gr, Qt::DisplayRole);
    QVariant name_var = sourceModel()->data(index_name, Qt::DisplayRole);

    if (gr_var.isValid() && name_var.isValid()) {
        QString gr = gr_var.value<QString>();
        QString name = name_var.value<QString>();

        qDebug() << "[TAB2][StudentFilterModel] source_row: " << source_row << "group: " << gr << "name: " << name;

        if (filter_by_gr) {
            if ((gr == f_gr_ && name.contains(f_name_, Qt::CaseInsensitive)))
                return true;
            else
                return false;
        } else {
            if(name.contains(f_name_, Qt::CaseInsensitive))
                return true;
            else
                return false;
        }
    }
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}
