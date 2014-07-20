#ifndef ORM4QT_STANDARDSQLPROVIDER_H
#define ORM4QT_STANDARDSQLPROVIDER_H

#include "sqlprovider.h"

namespace Orm4Qt
{
    class StandardSqlProvider : public SqlProvider
    {
    public:
        StandardSqlProvider();
        virtual ~StandardSqlProvider();

        // SqlProvider interface
        virtual shared_ptr<QSqlQuery> generateInsert(Class *reflect, const QList<int> &fieldsno) override;
        virtual shared_ptr<QSqlQuery> generateUpdate(Class *reflect, const QList<int> &fieldsno) override;
        virtual shared_ptr<QSqlQuery> generateDelete(Class *reflect) override;
        virtual shared_ptr<QSqlQuery> generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno,
                                                     const QList<QPair<QString, OrderBy>> orderby = QList<QPair<QString, OrderBy>>(),
                                                     int offset=-1, int limit = -1) override;
    };

} // namespace Orm4Qt

#endif // ORM4QT_STANDARDSQLPROVIDER_H
