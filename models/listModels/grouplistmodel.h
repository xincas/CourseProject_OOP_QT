#ifndef GROUPLISTMODEL_H
#define GROUPLISTMODEL_H

#include <QAbstractListModel>
#include "entities/group.h"

class GroupListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GroupListModel(QList<Group*>* gr, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void refreshData(QList<Group*>* gr);

private:
    QList<Group*>* source;

};

#endif // GROUPLISTMODEL_H
