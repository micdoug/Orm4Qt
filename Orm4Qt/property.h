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
    };
}

#endif // PROPERTY_H
