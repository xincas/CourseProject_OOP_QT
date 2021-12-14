#ifndef STUDTABLEMODEL_H
#define STUDTABLEMODEL_H

#include <QAbstractTableModel>
#include "entities/student.h"
#include "entities/group.h"

class StudTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit StudTableModel(QList<Student*>* src, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QList<Student*>* getStuds();
    void refreshData(QList<Student*>* studs);

private:
    QList<Student*>* source;
};

#endif // STUDTABLEMODEL_H
