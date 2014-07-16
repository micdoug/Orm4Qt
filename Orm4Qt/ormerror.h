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
