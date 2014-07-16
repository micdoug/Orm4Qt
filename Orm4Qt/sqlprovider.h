#ifndef SQLPROVIDER_H
#define SQLPROVIDER_H

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "where.h"
#include "class.h"
#include "ormerror.h"
#include <memory>

namespace Orm4Qt
{
    class SqlProvider
    {
    public:
        SqlProvider(const QString &connectionName);
        SqlProvider(const SqlProvider &other);
        virtual ~SqlProvider();
        QString databaseConnectionName() const;
        void setDatabaseConnectionName(const QString &databaseConnectionName);
        virtual shared_ptr<QSqlQuery> generateInsert(Class *reflect, const QList<int> &fieldsno) = 0;
        virtual shared_ptr<QSqlQuery> generateUpdate(Class *reflect, const QList<int> &fieldsno) = 0;
        virtual shared_ptr<QSqlQuery> generateDelete(Class *reflect) = 0;
        virtual shared_ptr<QSqlQuery> generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno) = 0;
        virtual shared_ptr<QSqlQuery> generateCreateTable(Class *reflect) = 0;
        virtual SqlProvider *clone() = 0;
        shared_ptr<OrmError> lastError() const;
    private:
        QString m_databaseConnectionName;
    protected:
        shared_ptr<OrmError> m_lastError;
    };
}

#endif // SQLPROVIDER_H
