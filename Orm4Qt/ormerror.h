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

#ifndef ORMERROR_H
#define ORMERROR_H

#include <QString>
#include <QSqlError>

namespace Orm4Qt
{
    enum ErrorType
    {
        NoError = 0,
        DatabaseError = 1,
        VersionError = 2,
        InvalidRequest = 3,
        ConfigurationError = 4,
        OperationNotSupported = 5
    };
    class OrmError
    {
    public:
        OrmError(const ErrorType &type, const QString &description, const QSqlError &error=QSqlError());
        ErrorType errorType() const;
        QString description() const;
        QSqlError sqlError() const;

    private:
        ErrorType m_errorType;
        QString m_description;
        QSqlError m_sqlError;
    };
}

#endif // ORMERROR_H
