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

#include "where.h"

using namespace Orm4Qt;

/**
 * \class Orm4Qt::Where
 * This class encapsulates the specification of a where clause. It permits the join between many instance of where clauses.
 * The instances of this class will be used by the sql providers to build the where clauses.
 */

/**
 * Constructor with specification of attributes.
 * @param field
 * Name of the field to be applied the condition
 * @param op
 * Type of the operation to be used in the condition
 * @param arguments
 * The list of arguments to be used to form the condition
 * @param precede
 * Specify if during the creation of this clause is necessary to open or close a block of precedence
 */
Where::Where(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede) :
    m_field(field), m_operation(op), m_arguments(arguments), m_join(NoJoin), m_precede(precede)
{}

/**
 * Constructor to specify a where clause without conditions
 */
Where::Where() : m_operation(WhereOp::NoOperation), m_join(WhereJoin::NoJoin), m_precede(WherePrecede::NoPrecede)
{}

/**
 * Copy constructor
 * @param other
 * The instance to copy
 */
Where::Where(const Where &other) : Where(other.field(), other.operation(), other.arguments(), other.precede())
{
    m_join = other.join();
    if(other.joined() != nullptr)
        m_joined = new Where(*other.joined());
}

/**
 * Virtual destructor
 */
Where::~Where()
{
    delete m_joined;
}

/**
 * Access the type of operation this where clause uses
 * @return
 * The type of operation used
 */
WhereOp Where::operation() const
{
    return m_operation;
}

/**
 * Acess the type of join this where clause uses
 * @return
 * The type of join used
 */
WhereJoin Where::join() const
{
    return m_join;
}

/**
 * Access the type of precedence used in this where clause
 * @return
 * The type of precedence used
 */
WherePrecede Where::precede() const
{
    return m_precede;
}

/**
 * Return the object that is joined with this instance
 * @return
 * The reference to the instance that is joined
 */
Where *Where::joined() const
{
    return m_joined;
}

/**
 * Access the name of the field that this whre clause apply
 * @return
 * The name of the field that this clause apply
 */
const QString &Where::field() const
{
    return m_field;
}

/**
 * Access the list of arguments used to build the condition
 * @return
 * The list of arguments used to build the condition
 */
const QList<QVariant> &Where::arguments() const
{
    return m_arguments;
}

/**
 * Join this instance with other where instance using the join of type And
 * @param where
 * The instance to be joined
 * @return
 * A reference to this instance of where clause
 */
Where &Where::And(const Where &where)
{
    if(m_joined != nullptr)
    {
        m_joined->And(where);
    }
    else
    {
        m_joined = new Where(where);
        m_join = WhereJoin::And;
    }
    return *this;
}

Where &Where::And(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede)
{
    Where where(field, op, arguments, precede);
    return And(where);
}

/**
 * Join this instance with other where instance using the join of type Or
 * @param where
 * The instance to be joined
 * @return
 * A referece to this instance of where clause
 */
Where &Where::Or(const Where &where)
{
    if(m_joined != nullptr)
    {
        m_joined->Or(where);
    }
    else
    {
        m_joined = new Where(where);
        m_join = WhereJoin::Or;
    }
    return *this;
}

Where &Where::Or(const QString &field, WhereOp op, const QList<QVariant> &arguments, WherePrecede precede)
{
    Where where(field, op, arguments, precede);
    return Or(where);
}

