#ifndef ANNOTATIONS_H
#define ANNOTATIONS_H

#include "propertyconcrete.h"
#include "class.h"
#include <memory>
#include "log.h"

#ifdef ORM4QT_DEBUG_OL
#define DEBUG_CREATE_CLASS qCDebug(ORM4QT_OL) << QString("Started creating a new reflection for Class defined in the file '%1' .").arg(__FILE__);
#define DEBUG_FINISH_CLASS qCDebug(ORM4QT_OL) << QString("Finished creating a new reflection for Class defined in the file '%1' .").arg(__FILE__);
#define DEBUG_CREATE_PROPERTY(NAME) qCDebug(ORM4QT_OL) << QString("Started creating a new reflection for the Property '%1' defined in the file '%2' .").arg(#NAME).arg(__FILE__);
#define DEBUG_FINISH_PROPERTY(NAME) qCDebug(ORM4QT_OL) << QString("Finished creating a new reflection for the Property '%1' defined in the file '%2' .").arg(#NAME).arg(__FILE__);
#else
#define DEBUG_CREATE_CLASS
#define DEBUG_FINISH_CLASS
#define DEBUG_CREATE_PROPERTY(NAME)
#define DEBUG_FINISH_PROPERTY(NAME)
#endif

#define ORM4QT_BEGIN \
    private: \
    shared_ptr<Orm4Qt::Class> m_reflectionObject =  nullptr;\
    public:\
    shared_ptr<Orm4Qt::Class> reflection()\
    {\
        if(m_reflectionObject == nullptr) {\
            DEBUG_CREATE_CLASS \
            shared_ptr<Orm4Qt::Class> c(new Orm4Qt::Class());\
            Orm4Qt::Property *p = nullptr;

#define ORM4QT_END \
        DEBUG_FINISH_CLASS \
        m_reflectionObject = c;\
        }\
        return m_reflectionObject;\
    }

#define CLASS(...) c->addTagsFromString(#__VA_ARGS__);

#define PROPERTY(NAME, ...) \
    DEBUG_CREATE_PROPERTY(NAME) \
    p = new Orm4Qt::PropertyConcrete<decltype(NAME)>(std::bind(std::mem_fn(&std::remove_pointer<decltype(this)>::type::NAME), this)); \
    p->addTagsFromString(#__VA_ARGS__); \
    p->addTag("name", #NAME); \
    c->addProperty(p);\
    DEBUG_FINISH_PROPERTY(NAME)

#endif // ANNOTATIONS_H
