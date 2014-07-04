#ifndef ANNOTATIONS_H
#define ANNOTATIONS_H

#include "propertyconcrete.h"
#include "class.h"
#include <memory>

#define ORM4QT_BEGIN \
    public:\
    shared_ptr<Orm4Qt::Class> reflection()\
    {\
        shared_ptr<Orm4Qt::Class> c(new Orm4Qt::Class());\
        Orm4Qt::Property *p = nullptr;

#define ORM4QT_END \
        return c;\
    }

#define CLASS(...) c->addTagsFromString(#__VA_ARGS__);

#define PROPERTY(NAME, ...) \
    p = new Orm4Qt::PropertyConcrete<decltype(NAME)>(std::bind(std::mem_fn(&std::remove_pointer<decltype(this)>::type::NAME), this)); \
    p->addTagsFromString(#__VA_ARGS__); \
    p->addTag("name", #NAME); \
    c->addProperty(p);

#endif // ANNOTATIONS_H
