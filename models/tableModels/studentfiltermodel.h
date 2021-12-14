#ifndef STUDENTFILTERMODEL_H
#define STUDENTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <entities/group.h>

class StudentFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    StudentFilterModel();

private:
    bool filter_by_gr = false;
    QString f_gr_;
    QString f_name_;

public slots:
    void updateFilterEnable(bool check);
    void updateFilterByGroup(QString gr);
    void updateFilterByName(QString fullName);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // STUDENTFILTERMODEL_H
