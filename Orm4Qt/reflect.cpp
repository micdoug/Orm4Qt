#include "reflect.h"
#include "log.h"

using namespace Orm4Qt;

/**
 * \class Orm4Qt::Reflect
 * Base class of the reflection system used in the Orm4Qt library. It provides a mechanism for
 * registering of metadata in the form of tags (key and value). The values stored use the QVariant
 * provided by the Qt Framework, so we can register values with different data types.
*/

/**
 * Constructor with optional list of tags initilization
 * @param tags The list of tags used for initialization
 */
Reflect::Reflect(const QHash<QString, QVariant> &tags) : m_tags(tags)
{}

/**
 * Copy constructor
 * @param other
 * The instance to be cloned
 */
Reflect::Reflect(const Reflect &other) : Reflect(other.tags())
{}

/**
 * Destructor
 */
Reflect::~Reflect()
{}

/**
 * Access the list of tags
 * @return Constant reference for the list of tags
 */
const QHash<QString, QVariant> &Reflect::tags() const
{
    return static_cast<const QHash<QString, QVariant>&>(m_tags);
}

/**
 * Add a new tag if doesn't exist a tag with the same name
 * @param name
 * The tag's name or label
 * @param value
 * The value associated with the tag
 * @return
 * True if the tag was added or false if it wasn't added (a tag with the same name already exists)
 */
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

/**
 * Replace a tag's value with the new value supplied. If doesn't exist a tag with the name provided
 * the method will fail and return false
 * @param name
 * The tag's name
 * @param value
 * The new value
 * @return
 * True if the tag's value was replaced, or false if the method failed
 */
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

/**
 * Remove a tag with the name provided
 * @param name
 * The tag's name
 * @return
 * True if the tag was removed or false if the method fail (doesn't exist a tag with the name provided)
 */
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

/**
 * Parse a string in the form "key=value, key=value" and insert the tags detected
 * @param tags
 * The string to be parsed
 */
void Reflect::addTagsFromString(const QString &tags)
{
    for (QString tup: tags.split(","))
    {
        QStringList keyvalue = tup.split("=");
        if(keyvalue.count() == 2)
        {
            this->addTag(keyvalue[0].trimmed(), keyvalue[1].replace("\"", "").trimmed());
        }
#ifdef ORM4QT_DEBUG_OL
        else
        {
            qCDebug(ORM4QT_OL()) << QString("Error on inserting token '%1'. It is invalid.").arg(tup);
        }
#endif
    }
}




