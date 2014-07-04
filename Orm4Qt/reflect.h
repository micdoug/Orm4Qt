#ifndef REFLECT_H
#define REFLECT_H

#include <QHash>
#include <QString>
#include <QVariant>

namespace Orm4Qt
{
    class Reflect
    {
    public:
        //Constructors and destructors
        explicit Reflect(const QHash<QString, QVariant> &tags=QHash<QString, QVariant>());
        Reflect (const Reflect &other);
        virtual ~Reflect();

        //Management of tags
        virtual const QHash<QString, QVariant> &tags() const;
        virtual bool addTag(const QString &name, const QVariant &value);
        virtual bool replaceTag(const QString &name, const QVariant &value);
        virtual bool removeTag(const QString &name);
        virtual void addTagsFromString(const QString &tags);

    private:
        //Internal properties
        QHash<QString, QVariant> m_tags;
    };
}

#endif // REFLECT_H
