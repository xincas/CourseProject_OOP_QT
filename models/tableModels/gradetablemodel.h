#ifndef GRADETABLEMODEL_H
#define GRADETABLEMODEL_H

#include <QAbstractTableModel>
#include "entities/grade.h"
#include "entities/subject.h"
#include "entities/teacher.h"

class GradeTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GradeTableModel(QList<Grade*> src, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QList<Grade*> getSource();
    void refreshData(QList<Grade*> grades);
private:
    QList<Grade*> source;
};

#endif // GRADETABLEMODEL_H
