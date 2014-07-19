#ifndef ORM4QT_SQLITEPROVIDER_H
#define ORM4QT_SQLITEPROVIDER_H

#include "standardsqlprovider.h"

namespace Orm4Qt {

    class SqliteProvider : public StandardSqlProvider
    {
    public:
        SqliteProvider(const QString &dbFileName, const QString &connectionName = QString("sqlite"));
        SqliteProvider(const SqliteProvider &other);
        virtual ~SqliteProvider();

        // SqlProvider interface
        virtual shared_ptr<QSqlQuery> generateCreateTable(Class *reflect) override;
        virtual SqlProvider *clone() override;

    };

} // namespace Orm4Qt

#endif // ORM4QT_SQLITEPROVIDER_H
