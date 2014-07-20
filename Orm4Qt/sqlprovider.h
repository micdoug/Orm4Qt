#ifndef SQLPROVIDER_H
#define SQLPROVIDER_H

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QList>
#include "where.h"
#include "class.h"
#include "ormerror.h"
#include <memory>

namespace Orm4Qt
{
    enum OrderBy
    {
        Asc,
        Desc
    };
    class SqlProvider
    {
    public:
        SqlProvider();
        virtual ~SqlProvider();
        QString databaseConnectionName() const;
        virtual shared_ptr<QSqlQuery> generateInsert(Class *reflect, const QList<int> &fieldsno) = 0;
        virtual shared_ptr<QSqlQuery> generateUpdate(Class *reflect, const QList<int> &fieldsno) = 0;
        virtual shared_ptr<QSqlQuery> generateDelete(Class *reflect) = 0;
        virtual shared_ptr<QSqlQuery> generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno,
                                                     const QList<QPair<QString, OrderBy>> orderby = QList<QPair<QString, OrderBy>>(),
                                                     int offset=-1, int limit = -1) = 0;
        virtual shared_ptr<QSqlQuery> generateCreateTable(Class *reflect) = 0;
        shared_ptr<OrmError> lastError() const;
    private:
        QString m_databaseConnectionName;
    protected:
        shared_ptr<OrmError> m_lastError;
    };
}

#endif // SQLPROVIDER_H
