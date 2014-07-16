#include "ormerror.h"

using namespace Orm4Qt;

OrmError::OrmError(const ErrorType &type, const QString &description, const QSqlError &error) :
    m_errorType(type), m_description(description), m_sqlError(error)
{}

ErrorType OrmError::errorType() const
{
    return m_errorType;
}
QString OrmError::description() const
{
    return m_description;
}
QSqlError OrmError::sqlError() const
{
    return m_sqlError;
}
