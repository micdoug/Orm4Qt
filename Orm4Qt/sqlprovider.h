/*
 * Orm4Qt - An Object Relational Mapping Library for the Qt Framework
 * Copyright (c) 2014, Michael Dougras da Silva, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#ifndef SQLPROVIDER_H
#define SQLPROVIDER_H

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QList>
#include "where.h"
#include "class.h"
#include "ormerror.h"
#include "orderbylist.h"
#include <memory>

namespace Orm4Qt
{
    class SqlProvider
    {
    public:
        SqlProvider();
        virtual ~SqlProvider();
        QString databaseConnectionName() const;
        virtual std::shared_ptr<QSqlQuery> generateInsert(Class *reflect, const QList<int> &fieldsno) = 0;
        virtual std::shared_ptr<QSqlQuery> generateUpdate(Class *reflect, const QList<int> &fieldsno) = 0;
        virtual std::shared_ptr<QSqlQuery> generateDelete(Class *reflect) = 0;
        virtual std::shared_ptr<QSqlQuery> generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno,
                                                     const QList<QPair<QString, OrderBy>> orderby = QList<QPair<QString, OrderBy>>(),
                                                     int offset=-1, int limit = -1) = 0;
        virtual std::shared_ptr<QSqlQuery> generateCount(Class *reflect, const Where &where) = 0;
        virtual std::shared_ptr<QSqlQuery> generateCreateTable(Class *reflect) = 0;
        std::shared_ptr<OrmError> lastError() const;
    private:
        QString m_databaseConnectionName;
    protected:
        std::shared_ptr<OrmError> m_lastError;
    };
}

#endif // SQLPROVIDER_H
