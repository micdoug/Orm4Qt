#include "reflect.h"

using namespace Orm4Qt;

Reflect::Reflect(const QHash<QString, QVariant> &tags) : m_tags(tags)
{}

Reflect::Reflect(const Reflect &other) : Reflect(other.tags())
{}

Reflect::~Reflect()
{}

const QHash<QString, QVariant> &Reflect::tags() const
{
    return static_cast<const QHash<QString, QVariant>&>(m_tags);
}

bool Reflect::addTag(const QString &name, const QVariant &value)
{
    if(m_tags.contains(name))
    {
        return false;
    }
    else
    {
        m_tags.insert(name, value);
        return true;
    }
}

bool Reflect::replaceTag(const QString &name, const QVariant &value)
{
    if(m_tags.contains(name))
    {
        m_tags.insert(name, value);
        return true;
    }
    else
    {
        return false;
    }
}

bool Reflect::removeTag(const QString &name)
{
    if(m_tags.contains(name))
    {
        m_tags.remove(name);
        return true;
    }
    else
    {
        return false;
    }
}

void Reflect::addTagsFromString(const QString &tags)
{
    for (QString tup: tags.split(","))
    {
        QStringList keyvalue = tup.split("=");
        if(keyvalue.count() == 2)
        {
            this->addTag(keyvalue[0].trimmed(), keyvalue[1].replace("\"", "").trimmed());
        }
        /*else
        {
            qDebug() << QString("Error on inserting token '%1'").arg(tup);
        }*/
    }
}




