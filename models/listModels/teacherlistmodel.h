#ifndef TEACHERLISTMODEL_H
#define TEACHERLISTMODEL_H

#include <QAbstractListModel>
#include "entities/teacher.h"

class TeacherListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TeacherListModel(QList<Teacher*> src, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QList<Teacher*> getSource();
    void refreshData(QList<Teacher*> tchs);
private:
    QList<Teacher*> source;
};

#endif // TEACHERLISTMODEL_H
