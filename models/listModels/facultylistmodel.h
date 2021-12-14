#ifndef FACULTYLISTMODEL_H
#define FACULTYLISTMODEL_H

#include <QAbstractListModel>
#include "entities/faculty.h"

class FacultyListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FacultyListModel(QObject *parent = nullptr);
    FacultyListModel(QList<Faculty*>* faculties, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QList<Faculty*>* getFaculties();
    void refreshData(QList<Faculty*>* fac);

private:
    QList<Faculty*>* faculties;

};

#endif // FACULTYLISTMODEL_H
