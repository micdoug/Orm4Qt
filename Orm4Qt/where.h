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

#ifndef WHERE_H
#define WHERE_H

#include <QList>
#include <QVariant>
#include <QString>

namespace Orm4Qt
{
    /**
     * This enum specify the type of operation used in the condition of the where clause
     */
    enum WhereOp
    {
        Equals,
        NotEquals,
        GreaterThan,
        GreaterEquals,
        LessThan,
        LessEquals,
        Contains,
        StartsWith,
        EndsWith,
        Like,
        In,
        NotIn,
        NoOperation
    };
    /**
     * This enum specify the type of join used to combine two where instances.
     */
    enum WhereJoin
    {
        And,
        Or,
        NoJoin
    };
    /**
     * This enum specify if is necessary to open or close a block of precedence during the creation of the where clause.
     */
    enum WherePrecede
    {
        Begin,
        End,
        NoPrecede
    };
    class Where
    {
    public:
        Where(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede=WherePrecede::NoPrecede);
        Where();
        Where(const Where &other);
        virtual ~Where();
        WhereOp operation() const;
        WhereJoin join() const;
        WherePrecede precede() const;
        Where *joined() const;
        const QString &field() const;
        const QList<QVariant> &arguments() const;
        virtual Where& And(const Where &where);
        virtual Where& And(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede=WherePrecede::NoPrecede);
        virtual Where& Or(const Where &where);
        virtual Where& Or(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede=WherePrecede::NoPrecede);
    private:
        QString m_field;
        WhereOp m_operation;
        QList<QVariant> m_arguments;
        WhereJoin m_join;
        WherePrecede m_precede;
        Where *m_joined = nullptr;
    };
}

#endif // WHERE_H
