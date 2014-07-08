#ifndef PROPERTY_H
#define PROPERTY_H

#include "reflect.h"

namespace Orm4Qt
{
    class Property : public Reflect
    {
    public:
        //Constructors and destructor
        explicit Property(const QHash<QString, QVariant> &tags = QHash<QString, QVariant>());
        Property(const Property &other);
        virtual ~Property();

        //Abstract methods for access and adjust value of properties
        virtual const QVariant value() const = 0;
        virtual void setValue(const QVariant &value) = 0;
        virtual int type() const = 0;
        virtual bool isPointer() const = 0;

        //Extending management tags methods to generate debug messages
#ifdef ORM4QT_DEBUG_OL
        virtual bool addTag(const QString &name, const QVariant &value) override;
        virtual bool replaceTag(const QString &name, const QVariant &value) override;
        virtual bool removeTag(const QString &name) override;
#endif
    };
}

#endif // PROPERTY_H
