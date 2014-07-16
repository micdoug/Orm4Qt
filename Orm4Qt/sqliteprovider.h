#ifndef ORM4QT_SQLITEPROVIDER_H
#define ORM4QT_SQLITEPROVIDER_H

#include "sqlprovider.h"

namespace Orm4Qt {

    class SqliteProvider : public SqlProvider
    {
    public:
        SqliteProvider(const QString &dbFileName, const QString &connectionName = QString("sqlite"));
        SqliteProvider(const SqliteProvider &other);
        virtual ~SqliteProvider();

        // SqlProvider interface
    public:
        virtual shared_ptr<QSqlQuery> generateInsert(Class *reflect, const QList<int> &fieldsno) override;
        virtual shared_ptr<QSqlQuery> generateUpdate(Class *reflect, const QList<int> &fieldsno) override;
        virtual shared_ptr<QSqlQuery> generateDelete(Class *reflect) override;
        virtual shared_ptr<QSqlQuery> generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno) override;
        virtual shared_ptr<QSqlQuery> generateCreateTable(Class *reflect) override;
        virtual SqlProvider *clone() override;

    };

} // namespace Orm4Qt

#endif // ORM4QT_SQLITEPROVIDER_H
