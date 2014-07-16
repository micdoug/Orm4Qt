#include "usuario.h"

Usuario::Usuario()
{

}

QString Usuario::nome() const
{
    return m_nome;
}

void Usuario::setNome(const QString &nome)
{
    m_nome = nome;
}
QString Usuario::email() const
{
    return m_email;
}

void Usuario::setEmail(const QString &email)
{
    m_email = email;
}
qlonglong Usuario::codigo() const
{
    return m_codigo;
}

void Usuario::setCodigo(const qlonglong &codigo)
{
    m_codigo = codigo;
}
qreal Usuario::pontos() const
{
    return m_pontos;
}

void Usuario::setPontos(const qreal &pontos)
{
    m_pontos = pontos;
}
QDate Usuario::inscrito() const
{
    return m_inscrito;
}

void Usuario::setInscrito(const QDate &inscrito)
{
    m_inscrito = inscrito;
}





