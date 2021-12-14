#include "facultygroupmodel.h"

FacultyGroupModel::FacultyGroupModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    root = 0;
}

FacultyGroupModel::~FacultyGroupModel()
{
    delete root;
}

void FacultyGroupModel::setRootNode(Node *node)
{
    beginResetModel();
    delete root;
    root = node;
    endResetModel();
}

QVariant FacultyGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0)
        return tr("Кафедры");
    return QVariant();
}

QModelIndex FacultyGroupModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!root || row < 0 || column < 0)
        return QModelIndex();
    Node* parentNode = nodeFromIndex(parent);
    Node* childNode = parentNode->children.value(row);
    if(!childNode)
        return QModelIndex();
    return createIndex(row, column, childNode);
}

QModelIndex FacultyGroupModel::parent(const QModelIndex &index) const
{
    Node* node = nodeFromIndex(index);
    if(!node)
        return QModelIndex();
    Node* parentNode = node->parent;
    if(!parentNode)
        return QModelIndex();
    Node* grandparentNode = parentNode->parent;
    if(!grandparentNode)
        return QModelIndex();

    int row = grandparentNode->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int FacultyGroupModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    Node* parentNode = nodeFromIndex(parent);
    if(!parentNode)
        return 0;
    return parentNode->children.count();
}

int FacultyGroupModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant FacultyGroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Node* node = nodeFromIndex(index);
    if(!node)
        return QVariant();

    if (role == Qt::DisplayRole){
        switch (node->type) {
        case Node::Faculty:
            return QString::number(static_cast<Faculty*>(node->data)->id()) + " | " + static_cast<Faculty*>(node->data)->getName();
        case Node::Group:
            return static_cast<Group*>(node->data)->getName();
        case Node::None:
            return QString("SUAI");
        }
    }

    return QVariant();
}

Node *FacultyGroupModel::nodeFromIndex(const QModelIndex &index) const
{
    if(index.isValid()){
        return static_cast<Node*>(index.internalPointer());
    } else {
        return root;
    }
}
