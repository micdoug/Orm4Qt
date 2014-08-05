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
        virtual std::shared_ptr<QSqlQuery> generateInsert(Class *reflect, const QList<int> &fieldsno) override;
        virtual std::shared_ptr<QSqlQuery> generateUpdate(Class *reflect, const QList<int> &fieldsno) override;
        virtual std::shared_ptr<QSqlQuery> generateDelete(Class *reflect) override;
        virtual std::shared_ptr<QSqlQuery> generateSelect(Class *reflect, const Where &where, const QList<int> &fieldsno,
                                                     const QList<QPair<QString, OrderBy>> orderby = QList<QPair<QString, OrderBy>>(),
                                                     int offset=-1, int limit = -1) override;
        virtual std::shared_ptr<QSqlQuery> generateCount(Class *reflect, const Where &where) override;
    };

} // namespace Orm4Qt

#endif // ORM4QT_STANDARDSQLPROVIDER_H
