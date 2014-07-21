#include "orderbylist.h"

using namespace Orm4Qt;

OrderByList::OrderByList(const QString &field, OrderBy order)
{
    append(QPair<QString, OrderBy>(field, order));
}

OrderByList &OrderByList::add(const QString &field, OrderBy order)
{
    append(QPair<QString, OrderBy>(field, order));
    return *this;
}

OrderByList::~OrderByList()
{}
