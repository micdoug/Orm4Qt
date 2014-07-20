#ifndef ORM4QT_POSTGRESQLPROVIDER_H
#define ORM4QT_POSTGRESQLPROVIDER_H

#include "standardsqlprovider.h"

namespace Orm4Qt {

    class PostgreSqlProvider : public StandardSqlProvider
    {
    public:
        PostgreSqlProvider(const QString &user, const QString &password, const QString &database,
                           const QString &host = QString("localhost"), int port=5432, const QString &connectionName = QString("PostgreSql"));
        PostgreSqlProvider(const PostgreSqlProvider &other);
        virtual ~PostgreSqlProvider();

        // SqlProvider interface
        virtual shared_ptr<QSqlQuery> generateCreateTable(Class *reflect);
        virtual SqlProvider *clone();
    };

} // namespace Orm4Qt

#endif // ORM4QT_POSTGRESQLPROVIDER_H
