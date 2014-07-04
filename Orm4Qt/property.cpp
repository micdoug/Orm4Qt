#include "property.h"

using namespace Orm4Qt;

Property::Property(const QHash<QString, QVariant> &tags) : Reflect(tags)
{}

Property::Property(const Property &other) : Reflect(other)
{}

Property::~Property()
{}
