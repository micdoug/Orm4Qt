#ifndef ORM4QT_SQLITEPROVIDER_H
#define ORM4QT_SQLITEPROVIDER_H

#include "standardsqlprovider.h"

namespace Orm4Qt {

    class SqliteProvider : public StandardSqlProvider
    {
    public:
        SqliteProvider(const QString &dbFileName);
        virtual ~SqliteProvider();

        // SqlProvider interface
        virtual std::shared_ptr<QSqlQuery> generateCreateTable(Class *reflect) override;

    };

} // namespace Orm4Qt

#endif // ORM4QT_SQLITEPROVIDER_H
