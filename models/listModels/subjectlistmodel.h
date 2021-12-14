#ifndef SUBJECTLISTMODEL_H
#define SUBJECTLISTMODEL_H

#include <QAbstractListModel>
#include "utils/utilfaculty.h"

class SubjectListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SubjectListModel(QList<Subject*> src, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QList<Subject*> getSource();
    void refreshData(QList<Subject*> subs);
private:
    QList<Subject*> source;
};

#endif // SUBJECTLISTMODEL_H
