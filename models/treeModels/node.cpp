#include "node.h"

Node::Node(Type type, QObject* const data)
{
    this->type = type;
    this->data = data;
    this->parent = 0;
}

Node::~Node()
{
    qDeleteAll(children);
}
