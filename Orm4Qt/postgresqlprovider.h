#ifndef ORM4QT_POSTGRESQLPROVIDER_H
#define ORM4QT_POSTGRESQLPROVIDER_H

#include "standardsqlprovider.h"

namespace Orm4Qt {

    class PostgreSqlProvider : public StandardSqlProvider
    {
    public:
        PostgreSqlProvider(const QString &user, const QString &password, const QString &database,
                           const QString &host = QString("localhost"), int port=5432);
        virtual ~PostgreSqlProvider();

        // SqlProvider interface
        virtual std::shared_ptr<QSqlQuery> generateCreateTable(Class *reflect);
    };

} // namespace Orm4Qt

#endif // ORM4QT_POSTGRESQLPROVIDER_H
