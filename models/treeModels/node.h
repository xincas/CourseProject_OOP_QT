#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QList>

class Node
{
public:
    enum Type {
        Faculty,
        Group,
        None
    };

    Node(Type type, QObject* const data);
    ~Node();

    Type type;
    QObject* data;
    Node* parent;
    QList<Node*> children;
};

#endif // NODE_H
