#ifndef FACULTYGROUPMODEL_H
#define FACULTYGROUPMODEL_H

#include <QAbstractItemModel>
#include "models/treeModels/node.h"
#include "entities/faculty.h"
#include "entities/group.h"

class FacultyGroupModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FacultyGroupModel(QObject *parent = nullptr);
    ~FacultyGroupModel();

    void setRootNode(Node* node);


    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    Node *nodeFromIndex(const QModelIndex &index) const;

    Node* root;
};

#endif // FACULTYGROUPMODEL_H
