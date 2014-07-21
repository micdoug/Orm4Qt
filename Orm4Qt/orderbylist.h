#ifndef ORM4QT_ORDERBYLIST_H
#define ORM4QT_ORDERBYLIST_H

#include <QList>
#include <QPair>
#include <QString>

namespace Orm4Qt
{
    enum OrderBy
    {
        Asc,
        Desc
    };

    class OrderByList : public QList<QPair<QString, OrderBy>>
    {
    public:
        OrderByList() = default;
        OrderByList(const QString &field, OrderBy order);
        OrderByList& add(const QString &field, OrderBy order);
        virtual ~OrderByList();
    };
} // namespace Orm4Qt

#endif // ORM4QT_ORDERBYLIST_H
