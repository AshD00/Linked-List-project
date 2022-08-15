#include "Node.h"

Node::Node(const string &Name, const int ID, const double X, const double Y) : m_name(Name), m_id(ID), m_x(X), m_y(Y)
{
}

Node::Node(void) : m_name(""), m_parentNode(), m_id(0), m_x(0), m_y(0)//, ptr(nullptr)
{
}