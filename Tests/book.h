#ifndef BOOK_H
#define BOOK_H

#include "annotations.h"
#include <QString>
#include <QDateTime>

class Book
{
private:
    QString m_name;
    QDateTime m_published;
    QByteArray m_photo;
    unsigned int m_edition;

    ORM4QT_BEGIN
        CLASS(name="Book", table="Books", autoid="Id", versioncolumn="Version")
        PROPERTY(m_name, name="Name", column="Name", unique=true)
        PROPERTY(m_published, name="Published", column="Published")
        PROPERTY(m_photo, name="Photo", column="Photo")
        PROPERTY(m_edition, name="Edition", column="Edition")
    ORM4QT_END
    QString name() const;
    void setName(const QString &name);
    QDateTime published() const;
    void setPublished(const QDateTime &published);
    QByteArray photo() const;
    void setPhoto(const QByteArray &photo);
    unsigned int edition() const;
    void setEdition(unsigned int edition);
};

#endif // BOOK_H
