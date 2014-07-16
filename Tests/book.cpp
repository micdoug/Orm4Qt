#include "book.h"



QDateTime Book::published() const
{
    return m_published;
}

void Book::setPublished(const QDateTime &published)
{
    m_published = published;
}

QByteArray Book::photo() const
{
    return m_photo;
}

void Book::setPhoto(const QByteArray &photo)
{
    m_photo = photo;
}

unsigned int Book::edition() const
{
    return m_edition;
}

void Book::setEdition(unsigned int edition)
{
    m_edition = edition;
}

QString Book::name() const
{
    return m_name;
}

void Book::setName(const QString &name)
{
    m_name = name;
}
