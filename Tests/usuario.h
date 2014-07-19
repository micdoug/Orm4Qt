#ifndef USUARIO_H
#define USUARIO_H

#include <QDate>
#include <QString>
#include "annotations.h"

class Usuario
{
public:
    Usuario();
    Usuario(const Usuario &other) = default;

    QString nome() const;
    void setNome(const QString &nome);

    QString email() const;
    void setEmail(const QString &email);

    qlonglong codigo() const;
    void setCodigo(const qlonglong &codigo);

    qreal pontos() const;
    void setPontos(const qreal &pontos);

    QDate inscrito() const;
    void setInscrito(const QDate &inscrito);

private:
    QString m_nome;
    QString m_email;
    qlonglong m_codigo;
    qreal m_pontos;
    QDate m_inscrito;

    ORM4QT_BEGIN
        CLASS(name="Usuario", table="Usuarios", autocolumn="Codigo")
        PROPERTY(m_nome, name="Nome", column="name")
        PROPERTY(m_email, name="Email", column="email", unique=true)
        PROPERTY(m_codigo, name="Codigo", key=true, column="code")
        PROPERTY(m_pontos, name="Pontos", column="points")
        PROPERTY(m_inscrito, name="Inscrito", column="subscribe")
    ORM4QT_END
};

#endif // USUARIO_H
